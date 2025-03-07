#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

uint32_t lksv3_level_change(struct ssd *ssd, lksv3_level *from, lksv3_level *to, lksv3_level *target) {
    lksv3_level **src_ptr=NULL, **des_ptr=NULL;
    des_ptr=&lksv_lsm->disk[to->idx];

    if(from!=NULL){ 
        src_ptr=&lksv_lsm->disk[from->idx];

        int m_num = from->m_num;

        *(src_ptr)=lksv3_level_init(m_num,from->idx);
        lksv3_free_level(lksv_lsm, from);
    }

    (*des_ptr)=target;
    lksv3_free_level(lksv_lsm, to);

    if (lksv_lsm->bottom_level < target->idx)
        lksv_lsm->bottom_level = target->idx;

    // Double check level list entries caching
    if (target->idx == lksv_lsm->bottom_level && lksv_lsm->lsm_cache->levels[cache_level(LEVEL_LIST_ENTRY, target->idx)].n != target->n_num) {
        for (int i = 0; i < target->n_num; i++) {
            if (target->level_data[i]->cache[LEVEL_LIST_ENTRY]) {
                continue;
            }
            uint32_t entry_size = target->level_data[i]->smallest.len + (LEVELLIST_HASH_BYTES * PG_N) + 20;
            kv_cache_insert(lksv_lsm->lsm_cache, &target->level_data[i]->cache[LEVEL_LIST_ENTRY], entry_size, cache_level(LEVEL_LIST_ENTRY, target->idx), KV_CACHE_WITHOUT_FLAGS);
            if (!target->level_data[i]->cache[LEVEL_LIST_ENTRY]) {
                break;
            }
        }
    }
    // We don't need to query membership to the last level's entry.
    if (target->idx < lksv_lsm->bottom_level) {
        for (int i = 0; i < target->n_num - 1; i++) {
            int n = 0;
            for (int j = 0; j < PG_N; j++) {
                n += target->level_data[i]->hash_lists[j].n;
            }
            uint32_t entry_size = (n * HASH_BYTES) + 20;
            kv_cache_insert(lksv_lsm->lsm_cache, &target->level_data[i]->cache[HASH_LIST], entry_size, cache_level(HASH_LIST, target->idx), KV_CACHE_WITHOUT_FLAGS);
            if (!target->level_data[i]->cache[HASH_LIST]) {
                break;
            }
        }
    }

    if (target->idx > 0) {
        kv_debug("[Level: %d] n_num: %d, m_num: %d\n", target->idx + 1, target->n_num, target->m_num);
    }

    return 1;
}

uint32_t lksv3_leveling(struct ssd *ssd, lksv3_level *from, lksv3_level *to, leveling_node *l_node){
    int m_num = to->m_num;
    lksv3_level *target = lksv3_level_init(m_num, to->idx);
    lksv_level_list_entry *entry = NULL;

    if (from) {
        // TODO: LEVEL_COMP_READ_DELAY
        do_lksv3_compaction2(ssd, from->idx, to->idx, NULL, target);
    } else {
        // TODO: LEVEL_COMP_READ_DELAY
        lksv3_read_run_delay_comp(ssd, to);
        do_lksv3_compaction2(ssd, -1, to->idx, l_node, target);
    }

    if (entry) FREE(entry);
    uint32_t res = lksv3_level_change(ssd, from, to, target);
    check_473(ssd);
    lksv_lsm->c_level = NULL;

    if(target->idx == LSM_LEVELN-1){
        kv_debug("last level %d/%d (n:f)\n",target->n_num,target->m_num);
    }
    return res;
}

