#include <pthread.h>
#include "hw/femu/kvssd/pink/pink_ftl.h"
#include "hw/femu/kvssd/pink/skiplist.h"

void compaction_subprocessing(struct ssd *ssd, struct kv_skiplist *top, struct pink_level_list_entry** src, struct pink_level_list_entry** org, struct pink_level *des){
    merger(ssd, top,src,org,des);

    kv_key key,end;
    pink_level_list_entry* target=NULL;

    int run_idx = 0;
    while((target=cutter(pink_lsm,top,des,&key,&end))){
        insert_run(ssd,des,target);
        run_idx++;
        free_run(pink_lsm, target);
        FREE(target);
    }

    kv_assert(pink_lsm->c_level == NULL);
    pink_lsm->c_level = des;

    // Critical section - level data will be changed.
    if (src) {
        for(int i=0; src[i]!=NULL; i++){
            pink_level_list_entry *temp=src[i];
            meta_segment_read_postproc(ssd, temp);
        }
        FREE(src);
    }
    for(int i=0; org[i]!=NULL; i++){
        pink_level_list_entry *temp=org[i];
        meta_segment_read_postproc(ssd, temp);
    }
    FREE(org);

    bool cache_full = false;
    for (int i = 0; i < des->n_num; i++) {
        pink_level_list_entry *temp = des->level_data[i];

        if (!cache_full) {
            kv_cache_insert(pink_lsm->lsm_cache, &temp->cache[META_SEGMENT], PAGESIZE, cache_level(META_SEGMENT, des->idx), KV_CACHE_FLUSH_EVICTED);
            if (!temp->cache[META_SEGMENT]) {
                cache_full = true;
            } else {
                continue;
            }
        }

        if (pink_should_meta_gc_high(ssd)) {
            gc_meta_femu(ssd);
        }

        kv_assert(temp->ppa.ppa == UNMAPPED_PPA);
        temp->ppa = compaction_meta_segment_write_femu(ssd, (char *) temp->buffer);
        temp->buffer = NULL;
    }

    //wait_delay(ssd, true);
}

bool meta_segment_read_preproc(pink_level_list_entry *r){
    if (r->buffer) {
        return true;
    }
    return false;
}

void meta_segment_read_postproc(struct ssd *ssd, pink_level_list_entry *r){
    if (r->ppa.ppa != UNMAPPED_PPA) {
        // data will be freed when marking page invalid.
        kv_assert(get_pg(ssd, &r->ppa)->data == r->buffer);
        mark_page_invalid(ssd, &r->ppa);
        r->buffer = NULL;
        r->ppa.ppa = UNMAPPED_PPA;
    }
}

