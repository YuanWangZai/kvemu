#include "hw/femu/kvssd/pink/pink_ftl.h"
#include "hw/femu/kvssd/pink/skiplist.h"
#include <math.h>

struct pink_lsmtree *pink_lsm;

void pink_lsm_adjust_level_multiplier(void)
{
    struct pink_level *floor = pink_lsm->disk[LSM_LEVELN-1];

    if (floor->n_num > floor->m_num * 9 / 10) {
        kv_debug("increase level multiplier %.2f to %.2f\n",
                pink_lsm->opts->level_multiplier,
                pink_lsm->opts->level_multiplier + 1);
        pink_lsm->opts->level_multiplier += 1;
    }
}

void pink_lsm_create(struct ssd *ssd)
{
    pink_lsm->ssd = ssd;

    float m_num = 1;
    uint64_t all_header_num = 0;
    pink_lsm->disk = (pink_level**) malloc(sizeof(pink_level*) * LSM_LEVELN);
    kv_debug("|-----LSMTREE params ---------\n");
    // TODO: apply last level to different size factor
    for (int i = 0; i < LSM_LEVELN; i++) {
        pink_lsm->disk[i] = level_init(i);
        kv_debug("| [%d] noe:%d\n", i, pink_lsm->disk[i]->m_num);
        all_header_num += pink_lsm->disk[i]->m_num;
        m_num *= pink_lsm->opts->level_multiplier;
    }

    kv_debug("| level:%d sizefactor:%lf\n",LSM_LEVELN, pink_lsm->opts->level_multiplier);
    kv_debug("| -------- algorithm_log END\n\n");

    kv_debug("SHOWINGSIZE(GB) :%lu HEADERSEG:%d DATASEG:%d\n", ((unsigned long) ssd->sp.tt_pgs * 9 / 10 * PAGESIZE) / G, ssd->sp.meta_lines, ssd->sp.data_lines);
    kv_debug("LEVELN:%d\n", LSM_LEVELN); 

    pink_lsm->lsm_cache = kv_cache_init(pink_lsm->opts->cache_memory_size, LSM_LEVELN * CACHE_TYPES);
    pink_lsm->lsm_cache->flush_callback = pink_flush_cache_when_evicted;
}

uint8_t lsm_find_run(struct ssd *ssd, kv_key key, pink_level_list_entry **entry, keyset **found, int *level, NvmeRequest *req) {
    pink_level_list_entry *entries=NULL;

    for(int i = *level; i < LSM_LEVELN; i++){
        entries = find_run(pink_lsm->disk[i], key, ssd, req);
        if(!entries) {
            continue;
        }

        if (!entries->buffer && entries->ppa.ppa == UNMAPPED_PPA) {
            entries = find_run2(pink_lsm->c_level, key, ssd, req);
            if (level) {
                *level = i = pink_lsm->c_level->idx;
            }
            if (!entries) {
                continue;
            }
            *entry = entries;
            return COMP_FOUND;
        }

        /*
         * Run is cached.
         */
        if (entries->buffer) {
            keyset *find = find_keyset(entries->buffer, key);
            if (find) {
                *found = find;
                if (level) *level = i;
                // TODO: someone need to free(find)
                return CACHING;
            }
        } else {
            /*
             * Run is founded in not pinned levels.
             */
            if (level) *level = i;
            *entry = entries;
            return FOUND;
        }

        continue;
    }
    return NOTFOUND;
}
