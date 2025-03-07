#include <pthread.h>
#include "hw/femu/kvssd/pink/pink_ftl.h"
#include "hw/femu/kvssd/pink/skiplist.h"

static void compaction_selector(struct ssd *ssd, pink_level *a, pink_level *b, leveling_node *lnode){
    if (a)
        kv_set_compaction_info(&pink_lsm->comp_ctx, a->idx, b->idx);
    else
        kv_set_compaction_info(&pink_lsm->comp_ctx, -1, b->idx);

    leveling(ssd, a, b, lnode);

    if (b->idx == LSM_LEVELN - 1)
        pink_lsm_adjust_level_multiplier();
    if (b->idx > 0) // We don't want too many calling adjust_lines().
        pink_adjust_lines(ssd);

    kv_reset_compaction_info(&pink_lsm->comp_ctx);
}

bool compaction_init(struct ssd *ssd) {
    QTAILQ_INIT(&pink_lsm->compaction_queue);
    return true;
}

static void compaction_assign(struct pink_lsmtree *LSM, compR* req){
    QTAILQ_INSERT_TAIL(&LSM->compaction_queue, req, entry);
}

void compaction_free(struct pink_lsmtree *LSM){
    while (!QTAILQ_EMPTY(&LSM->compaction_queue)) {
        compR *req = QTAILQ_FIRST(&LSM->compaction_queue);
        QTAILQ_REMOVE(&LSM->compaction_queue, req, entry);
    }
}

static void compaction_cascading(struct ssd *ssd) {
    int start_level = 0, des_level;
    while (should_compact(pink_lsm->disk[start_level])) {
        if (start_level < LSM_LEVELN - 3)
            des_level = start_level + 1;
        else
            break;
        compaction_selector(ssd, pink_lsm->disk[start_level], pink_lsm->disk[des_level], NULL);
        start_level++;
    }

    /*
       L0 - don't care.
       L1 - if L2 should be compacted, then compact after L2 compaction.
       L2 - compact only if L1 should be compacted.
       L3 - (last level).
     */
    if (should_compact(pink_lsm->disk[LSM_LEVELN - 3])) {
        if (should_compact(pink_lsm->disk[LSM_LEVELN - 2])) {
            compaction_selector(ssd, pink_lsm->disk[LSM_LEVELN - 2], pink_lsm->disk[LSM_LEVELN - 1], NULL);
        }
        compaction_selector(ssd, pink_lsm->disk[LSM_LEVELN - 3], pink_lsm->disk[LSM_LEVELN - 2], NULL);
    }
}

static void
do_gc(struct ssd *ssd)
{
    while (pink_should_data_gc_high(ssd)) {
        int n = ssd->lm.data.lines / 10;
        if (n < 10)
            n = 10;

        int gc_pick_err = 0;
        int gc_pick_err_threshold = n / 3;
        for (int i = 0; i < n; i ++) {
            switch (gc_data_femu(ssd)) {
                case 0:
                    break;
                case -2:
                    gc_pick_err++;
                    break;
                default:
                    kv_log("unknown return code\n");
                    abort();
            }
        }
        if (gc_pick_err > gc_pick_err_threshold) {
            kv_log("gc_pick_err exceeds threshold: %d times\n", gc_pick_err);
            print_level_summary(pink_lsm);
            kv_log("line_partition meta: %d lines, %d frees, %d victims, %d fulls, %ld age\n", ssd->lm.meta.lines, ssd->lm.meta.free_line_cnt, ssd->lm.meta.victim_line_cnt, ssd->lm.meta.full_line_cnt, ssd->lm.meta.age);
            kv_log("line_partition data: %d lines, %d frees, %d victims, %d fulls, %ld age\n", ssd->lm.data.lines, ssd->lm.data.free_line_cnt, ssd->lm.data.victim_line_cnt, ssd->lm.data.full_line_cnt, ssd->lm.data.age);
            abort();
        }
    }
    while (pink_should_meta_gc_high(ssd)) {
        int n = ssd->lm.meta.lines / 10;
        if (n < 2)
            n = 2;
        for (int i = 0; i < n; i ++) {
            gc_meta_femu(ssd);
        }
    }
}

void pink_do_compaction(struct ssd *ssd)
{
    if (!QTAILQ_EMPTY(&pink_lsm->compaction_queue)) {
        compR *req = QTAILQ_FIRST(&pink_lsm->compaction_queue);
        QTAILQ_REMOVE(&pink_lsm->compaction_queue, req, entry);
        leveling_node lnode;

        if (kv_skiplist_approximate_memory_usage(pink_lsm->memtable) >= WRITE_BUFFER_SIZE)
        {
            compaction_data_write(ssd, pink_lsm->memtable);

            kv_skiplist_free(pink_lsm->memtable);
            pink_lsm->memtable = kv_skiplist_init();

            do_gc(ssd);
        }

        if (kv_skiplist_approximate_memory_usage(pink_lsm->kmemtable) >= KEY_ONLY_WRITE_BUFFER_SIZE)
        {
            // TODO: make temptable immutable.
            pink_lsm->temptable = pink_lsm->kmemtable;
            pink_lsm->kmemtable = kv_skiplist_init();

            bool done = false;
            while (!done)
            {
                kv_skiplist *tmp = pink_skiplist_cutting_header(pink_lsm->temptable);
                done = (tmp == pink_lsm->temptable);

                kv_skiplist_get_start_end_key(tmp, &lnode.start, &lnode.end);
                lnode.mem = tmp;
                compaction_selector(ssd, NULL, pink_lsm->disk[0], &lnode);
                compaction_cascading(ssd);

                FREE(lnode.start.key);
                FREE(lnode.end.key);

                kv_skiplist_free(tmp);

                do_gc(ssd);
            }

            pink_lsm->temptable = NULL;
        }

        FREE(req);

        if (rand() % 1000 == 0) {
            kv_debug("write_cnt %lu\n", pink_lsm->num_data_written);
            kv_debug("[META] free line cnt: %d\n", ssd->lm.meta.free_line_cnt);
            kv_debug("[META] full line cnt: %d\n", ssd->lm.meta.full_line_cnt);
            kv_debug("[META] victim line cnt: %d\n", ssd->lm.meta.victim_line_cnt);
            kv_debug("[DATA] free line cnt: %d\n", ssd->lm.data.free_line_cnt);
            kv_debug("[DATA] full line cnt: %d\n", ssd->lm.data.full_line_cnt);
            kv_debug("[DATA] victim line cnt: %d\n", ssd->lm.data.victim_line_cnt);
            print_level_summary(pink_lsm);
        }
    }
}

void compaction_check(struct ssd *ssd) {
    if (kv_skiplist_approximate_memory_usage(pink_lsm->memtable) < WRITE_BUFFER_SIZE)
        return;

    compR *req = (compR*)malloc(sizeof(compR));
    compaction_assign(pink_lsm, req);
}

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

