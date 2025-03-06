#include "hw/femu/kvssd/pink/pink_ftl.h"

uint32_t level_change(struct pink_lsmtree *LSM, pink_level *from, pink_level *to, pink_level *target) {
    pink_level **src_ptr=NULL, **des_ptr=NULL;
    des_ptr=&LSM->disk[to->idx];

    if(from!=NULL){
        src_ptr=&LSM->disk[from->idx];
        *(src_ptr)=level_init(from->idx);
        free_level(LSM, from);
    }

    (*des_ptr)=target;
    free_level(LSM, to);

    return 1;
}

uint32_t leveling(struct ssd *ssd, pink_level *from, pink_level *to, leveling_node *l_node){
    pink_level *target_origin = to;
    pink_level *target = level_init(to->idx);
    pink_level_list_entry *entry = NULL;

    // TODO: LEVEL_COMP_READ_DELAY
    read_run_delay_comp(ssd, to);
    partial_leveling(ssd, target,target_origin,l_node,from);

    if (entry) FREE(entry);
    uint32_t res = level_change(pink_lsm, from, to, target);
    pink_lsm->c_level = NULL;
    if (from == NULL) {
        kv_assert(l_node->mem == pink_lsm->temptable[0]);
        for (int z = 0; z < pink_lsm->temp_n; z++) {
            kv_skiplist_free(pink_lsm->temptable[z]);
            pink_lsm->temptable[z] = NULL;
        }
        pink_lsm->temp_n = 0;
    }

    if(target->idx == LSM_LEVELN-1){
        kv_debug("last level %d/%d (n:f)\n",target->n_num,target->m_num);
    }
    return res;
}

uint32_t partial_leveling(struct ssd *ssd, pink_level* t, pink_level *origin, leveling_node *lnode, pink_level* upper){
    kv_key start=kv_key_min;
    kv_key end=kv_key_max;
    pink_level_list_entry **target_s=NULL;
    pink_level_list_entry **data=NULL;
    kv_skiplist *skip=lnode?lnode->mem:kv_skiplist_init();

    if(!upper){
        range_find_compaction(origin,start,end,&target_s);

        for(int j=0; target_s[j]!=NULL; j++){
            compaction_meta_segment_read_femu(ssd, target_s[j]);
        }

        compaction_subprocessing(ssd, skip,NULL,target_s,t);
    } else {
        int src_num, des_num; //for stream compaction
        des_num=range_find_compaction(origin,start,end,&target_s);//for stream compaction
        src_num=range_find_compaction(upper,start,end,&data);

        for(int i=0; i < des_num; i++){
            kv_assert(target_s[i]);
            pink_level_list_entry *temp=target_s[i];
            compaction_meta_segment_read_femu(ssd, temp);
        }

        for(int i=0; i < src_num; i++){
            kv_assert(data[i]);
            pink_level_list_entry *temp=data[i];
            compaction_meta_segment_read_femu(ssd, temp);
        }
        //wait_delay(ssd, true);
        compaction_subprocessing(ssd, NULL,data,target_s,t);
    }
    if(!lnode) kv_skiplist_free(skip);
    return 1;
}

