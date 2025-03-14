#include "hw/femu/kvssd/lksv/lksv3_ftl.h"
#include <math.h>
#include "hw/femu/kvssd/lksv/skiplist.h"

struct lksv3_lsmtree *lksv_lsm;

void lksv3_lsm_create(struct ssd *ssd)
{
    lksv_lsm->ssd = ssd;

    lksv_lsm_setup_params(ssd);

    float m_num = 1;
    uint64_t all_header_num = 0;
    lksv_lsm->disk = (lksv3_level**) malloc(sizeof(lksv3_level*) * LSM_LEVELN);
    kv_debug("|-----LSMTREE params ---------\n");
    // TODO: apply last level to different size factor
    for (int i = 0; i < LSM_LEVELN; i++) {
        int m = ceil(m_num * lksv_lsm->opts->level_multiplier);

        lksv_lsm->disk[i] = lksv3_level_init(m, i);
        kv_debug("| [%d] noe:%d\n", i, lksv_lsm->disk[i]->m_num);
        all_header_num += lksv_lsm->disk[i]->m_num;
        m_num *= lksv_lsm->opts->level_multiplier;
    }

    kv_debug("| level:%d sizefactor:%lf\n",LSM_LEVELN, lksv_lsm->opts->level_multiplier);
    //uint64_t level_bytes = all_header_num * lsp->ONESEGMENT;
    //kv_debug("| all level size:%lu(MB), %lf(GB)", level_bytes / M, (double) level_bytes / G);
    // TODO: all level header size
    //kv_debug(" target size: %lf(GB)\n",(double) ssd->sp.tt_pgs * PAGESIZE / G);
    //kv_debug("| level cache :%luMB(%lu page)%.2f(%%)\n",lsp->pin_memory/M,lsp->pin_memory/PAGESIZE,(float)lsp->pin_memory/lsp->total_memory*100);
    //kv_debug("| entry cache :%luMB(%lu page)%.2f(%%)\n",lsp->cache_memory/M,lsp->cache_memory/PAGESIZE,(float)lsp->cache_memory/lsp->total_memory*100);
    kv_debug("| -------- algorithm_log END\n\n");

    kv_debug("SHOWINGSIZE(GB) :%lu HEADERSEG:%d DATASEG:%d\n", ((unsigned long) ssd->sp.tt_pgs * 9 / 10 * PAGESIZE) / G, ssd->sp.meta_lines, ssd->sp.data_lines);
    kv_debug("LEVELN:%d\n", LSM_LEVELN);

    lksv_lsm->lsm_cache = kv_cache_init(lksv_lsm->opts->cache_memory_size, LSM_LEVELN * CACHE_TYPES);
    lksv_lsm->avg_value_bytes = 1024;
    lksv_lsm->avg_key_bytes = 32;
    lksv_lsm->sum_value_bytes = 0;
    lksv_lsm->sum_key_bytes = 0;
    lksv_lsm->samples_count = 0;
}

uint8_t lksv3_lsm_find_run(struct ssd *ssd, kv_key key, lksv_level_list_entry **entry, keyset **found, int *level, NvmeRequest *req) {
    lksv_level_list_entry *entries=NULL;

    uint32_t hash;
    hash = XXH32(key.key, key.len, 0);

    for(int i = *level; i < LSM_LEVELN; i++){
        entries = lksv3_find_run(lksv_lsm->disk[i], key, ssd, req);
        if(!entries) {
            continue;
        }

        if (entries->ppa.ppa == UNMAPPED_PPA) {
            entries = lksv3_find_run(lksv_lsm->c_level, key, ssd, req);
            if (level) {
                *level = i = lksv_lsm->c_level->idx;
            }
            if (!entries) {
                continue;
            }

            keyset *find = lksv3_find_keyset(ssd, req, entries, key, hash, i);
            if (find) {
                *found = find;
                if (level) *level = i;
                *entry = entries;
                // TODO: someone need to free(find)
                if (find->ppa.ppa != UNMAPPED_PPA) {
                    struct nand_cmd srd;
                    srd.type = USER_IO;
                    srd.cmd = NAND_READ;
                    srd.stime = req->etime;
                    req->flash_access_count++;
                    uint64_t sublat = lksv3_ssd_advance_status(ssd, &find->ppa, &srd); 
                    req->etime += sublat;

                    kv_assert(check_voffset(ssd, &find->ppa, find->voff, find->hash));
                }
                return COMP_FOUND;
            } else {
                continue;
            }
        }

        keyset *find = lksv3_find_keyset(ssd, req, entries, key, hash, i);
        bool flash_access_for_caching = false;
        // We don't need to query membership to the last level's entry.
        if (i < lksv_lsm->bottom_level) {
            if (kv_is_cached(lksv_lsm->lsm_cache, entries->cache[HASH_LIST])) {
                lksv_lsm->cache_hit++;
                if (lksv_lsm->cache_hit % 1000000 == 0)
                    kv_debug("cache hit ratio: %lu\n", lksv_lsm->cache_hit * 100 / (lksv_lsm->cache_hit + lksv_lsm->cache_miss));
            } else if (kv_cache_available(lksv_lsm->lsm_cache, cache_level(HASH_LIST, i))) {
                int n = 0;
                for (int j = 0; j < PG_N; j++) {
                    n += entries->hash_lists[j].n;
                }
                uint32_t entry_size = (n * HASH_BYTES) + 20;
                kv_cache_insert(lksv_lsm->lsm_cache, &entries->cache[HASH_LIST], entry_size, cache_level(HASH_LIST, i), KV_CACHE_WITHOUT_FLAGS);
                if (entries->cache[HASH_LIST])
                    flash_access_for_caching = true;
                lksv_lsm->cache_miss++;
            }
        }
        if (kv_cache_available(lksv_lsm->lsm_cache, cache_level(DATA_SEGMENT_GROUP, i)) && !kv_is_cached(lksv_lsm->lsm_cache, entries->cache[DATA_SEGMENT_GROUP])) {
            uint32_t entry_size = PG_N * PAGESIZE;
            kv_cache_insert(lksv_lsm->lsm_cache, &entries->cache[DATA_SEGMENT_GROUP], entry_size, cache_level(DATA_SEGMENT_GROUP, i), KV_CACHE_WITHOUT_FLAGS);
            if (entries->cache[DATA_SEGMENT_GROUP])
                flash_access_for_caching = true;
        }
        if (flash_access_for_caching) {
            for (int k = 0; k < entries->hash_list_n; k++) {
                struct femu_ppa cppa = get_next_write_ppa(ssd, entries->ppa, k);
                struct nand_cmd srd;
                srd.type = USER_IO;
                srd.cmd = NAND_READ;
                srd.stime = 0;
                lksv3_ssd_advance_status(ssd, &cppa, &srd);
            }
        }

        if (find) {
            *found = find;
            if (level) *level = i;
            // TODO: someone need to free(find)

            if (find->ppa.ppa != UNMAPPED_PPA) {
                //struct nand_page *pg2 = lksv3_get_pg(ssd, &find->ppa);
                struct nand_cmd srd;
                srd.type = USER_IO;
                srd.cmd = NAND_READ;
                srd.stime = req->etime;
                req->flash_access_count++;
                uint64_t sublat = lksv3_ssd_advance_status(ssd, &find->ppa, &srd); 
                req->etime += sublat;

                kv_assert(check_voffset(ssd, &find->ppa, find->voff, find->hash));
            }
            return FOUND;
        }
    }
    return NOTFOUND;
}
