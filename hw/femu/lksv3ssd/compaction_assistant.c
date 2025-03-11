#include "hw/femu/kvssd/lksv/lksv3_ftl.h"
#include "hw/femu/kvssd/lksv/skiplist.h"

static void compaction_selector(struct ssd *ssd, lksv3_level *a, lksv3_level *b, leveling_node *lnode){
    lksv3_leveling(ssd, a, b, lnode);
}

bool lksv3_compaction_init(struct ssd *ssd) {
    QTAILQ_INIT(&lksv_lsm->compaction_queue);
    return true;
}

static void compaction_assign(struct lksv3_lsmtree *LSM, compR* req){
    QTAILQ_INSERT_TAIL(&LSM->compaction_queue, req, entry);
}

void lksv3_compaction_free(struct lksv3_lsmtree *LSM){
    while (!QTAILQ_EMPTY(&LSM->compaction_queue)) {
        compR *req = QTAILQ_FIRST(&LSM->compaction_queue);
        QTAILQ_REMOVE(&LSM->compaction_queue, req, entry);
    }
}

static void
call_log_triggered_compaction(struct ssd *ssd)
{
    lksv_lsm->force = true;

    /*
     * During the code cleanup, we temporarily apply a simple and stupid policy.
     * Policy: Do compaction sequentially from the lowest to the highest level.
     */
    for (int i = 0; i < LSM_LEVELN - 1; i++)
    {
        if (!lksv3_should_data_gc_high(ssd, 5))
            break;

        if (lksv_lsm->disk[i]->n_num == 0)
            continue;

        kv_log("Log-triggered compaction: %d->%d\n", i, i+1);
        compaction_selector(ssd, lksv_lsm->disk[i], lksv_lsm->disk[i+1], NULL);
    }

    lksv_lsm->force = false;
}

static void compaction_cascading(struct ssd *ssd) {
    if (lksv3_should_compact(lksv_lsm->disk[LSM_LEVELN - 4])) {
        compaction_selector(ssd, lksv_lsm->disk[LSM_LEVELN - 4], lksv_lsm->disk[LSM_LEVELN - 3], NULL);
        update_lines(ssd);
    }
    if (lksv3_should_compact(lksv_lsm->disk[LSM_LEVELN - 3])) {
        if (lksv3_should_compact(lksv_lsm->disk[LSM_LEVELN - 2])) {
            compaction_selector(ssd, lksv_lsm->disk[LSM_LEVELN - 2], lksv_lsm->disk[LSM_LEVELN - 1], NULL);
            update_lines(ssd);
        }
        compaction_selector(ssd, lksv_lsm->disk[LSM_LEVELN - 3], lksv_lsm->disk[LSM_LEVELN - 2], NULL);
    } else if (lksv3_should_compact(lksv_lsm->disk[LSM_LEVELN - 2])) {
        compaction_selector(ssd, lksv_lsm->disk[LSM_LEVELN - 2], lksv_lsm->disk[LSM_LEVELN - 1], NULL);
        update_lines(ssd);
    }
    update_lines(ssd);

    if (lksv3_should_data_gc_high(ssd, 0)) {
        call_log_triggered_compaction(ssd);
        update_lines(ssd);
    }

    if (ssd->lm.data.lines > ssd->sp.tt_lines - lksv_lsm->t_meta)
        move_line_d2m(ssd, false);
    else if (ssd->lm.data.lines < ssd->sp.tt_lines - lksv_lsm->t_meta)
        move_line_m2d(ssd, false);
}

static void log_write(struct ssd *ssd, kv_skiplist *mem) {
    int wp = 0;
    lksv3_sst_t sst;

    memset(&sst, 0, sizeof(lksv3_sst_t));
    void *meta = calloc(2048, sizeof(lksv_block_meta));

    struct femu_ppa fppa = lksv3_get_new_data_page(ssd);
    struct nand_page *pg = lksv3_get_pg(ssd, &fppa);
    if (pg->data == NULL) {
        pg->data = calloc(1, PAGESIZE);
    }
    sst.raw = pg->data;
    sst.meta = meta;
    kv_snode *t, *t2;

    kv_key tmp_key[65536];
    kv_value *tmp_val[65536];
    struct femu_ppa tmp_ppa[65536];
    uint32_t tmp_hash[65536];
    int tmp_voff[65536];
    int tmp_i = 0;

    kv_key dummy_key;
    dummy_key.key = NULL;
    dummy_key.len = 0;

    for_each_sk (t, mem) {
        kv_assert(t->value);
        kv_assert(t->value->length > 0);

        kv_key key;
        kv_copy_key(&key, &t->key);

        kv_value *v;
        v = calloc(1, sizeof(kv_value));
        v->length = PPA_LENGTH;
        v->value = NULL;

        tmp_key[tmp_i] = key;
        tmp_val[tmp_i] = v;
        tmp_hash[tmp_i] = XXH32(key.key, key.len, 0);
        tmp_ppa[tmp_i] = fppa;
        tmp_voff[tmp_i] = sst.footer.g.n;

        lksv3_kv_pair_t kv;
        kv.k = dummy_key;
        kv.v.len = t->value->length;
        kv_assert(t->value->value);
        kv.v.val = t->value->value;
        kv.ppa.ppa = UNMAPPED_PPA;

        int ret;
retry:
        ret = lksv3_sst_encode2(&sst, &kv, tmp_hash[tmp_i], &wp, false);
        if (ret == LKSV3_TABLE_FULL) {
            struct line *line = lksv3_get_line(ssd, &fppa);
            line->vsc += sst.footer.g.n;
            per_line_data(line)->referenced_flush = true;
            lksv_lsm->flush_reference_lines[fppa.g.blk] = true;
            lksv3_mark_page_valid2(ssd, &fppa);
            lksv3_ssd_advance_write_pointer(ssd, &ssd->lm.data);

            int prev_idx = tmp_i - 1;
            if (prev_idx < 0)
                prev_idx = 0;

            if (ssd->sp.enable_comp_delay) {
                struct nand_cmd cpw;
                cpw.type = COMP_IO;
                cpw.cmd = NAND_WRITE;
                cpw.stime = 0;
                lksv3_ssd_advance_status(ssd, &fppa, &cpw);
            }
            fppa.ppa = UNMAPPED_PPA;

            memset(&sst, 0, sizeof(lksv3_sst_t));
            sst.meta = meta;
            wp = 0;

            fppa = lksv3_get_new_data_page(ssd);
            pg = lksv3_get_pg(ssd, &fppa);
            if (pg->data == NULL) {
                pg->data = calloc(1, PAGESIZE);
            }
            sst.raw = pg->data;

            tmp_ppa[tmp_i] = fppa;
            tmp_voff[tmp_i] = sst.footer.g.n;
            goto retry;
        } else {
            // kv_skiplist_free will free them.
            //FREE(t->value->value);
            //FREE(t->value);
            kv_assert(ret == LKSV3_TABLE_OK);
        }
        tmp_i++;

        if (tmp_i > 1) {
            kv_assert(kv_cmp_key(tmp_key[tmp_i-2], tmp_key[tmp_i-1]) < 0);
        }
    }

    for (int i = 0; i < tmp_i; i++) {
        t2 = lksv3_skiplist_insert(lksv_lsm->key_only_mem, tmp_key[i], tmp_val[i], true, ssd);
        if (t2->private == NULL)
            t2->private = malloc(sizeof(lksv_per_snode_data));
        *snode_ppa(t2) = tmp_ppa[i];
        *snode_off(t2) = tmp_voff[i];
        *snode_hash(t2) = tmp_hash[i];
        t2->value->length = PPA_LENGTH;
    }

    lksv3_mark_page_valid2(ssd, &fppa);
    if (sst.footer.g.n) {
        struct line *line = lksv3_get_line(ssd, &fppa);
        line->vsc += sst.footer.g.n;

        per_line_data(line)->referenced_flush = true;
        lksv_lsm->flush_reference_lines[fppa.g.blk] = true;
    }
    lksv3_ssd_advance_write_pointer(ssd, &ssd->lm.data);
    if (ssd->sp.enable_comp_delay) {
        struct nand_cmd cpw;
        cpw.type = COMP_IO;
        cpw.cmd = NAND_WRITE;
        cpw.stime = 0;
        lksv3_ssd_advance_status(ssd, &fppa, &cpw);
    }

    FREE(sst.meta);
}

void lksv3_do_compaction(struct ssd *ssd)
{
    if (!QTAILQ_EMPTY(&lksv_lsm->compaction_queue)) {
        compR *req = QTAILQ_FIRST(&lksv_lsm->compaction_queue);
        QTAILQ_REMOVE(&lksv_lsm->compaction_queue, req, entry);
        leveling_node lnode;

        kv_assert(ssd->lm.data.free_line_cnt > 0);

        if (kv_skiplist_approximate_memory_usage(lksv_lsm->mem) >= WRITE_BUFFER_SIZE)
        {
            log_write(ssd, lksv_lsm->mem);
            check_473(ssd);

            kv_skiplist_put(lksv_lsm->mem);
            lksv_lsm->mem = kv_skiplist_init();
        }

        // TODO: make temp table immutable.
        if (kv_skiplist_approximate_memory_usage(lksv_lsm->key_only_mem) >= KEY_ONLY_WRITE_BUFFER_SIZE)
        {
            lksv_lsm->key_only_imm = lksv_lsm->key_only_mem;
            lksv_lsm->key_only_mem = kv_skiplist_init();

            memset(lksv_lsm->flush_buffer_reference_lines, 0, 512 * sizeof(bool));
            kv_snode *t;
            for_each_sk (t, lksv_lsm->key_only_imm)
            {
                if (!lksv_lsm->flush_buffer_reference_lines[snode_ppa(t)->g.blk])
                {
                    lksv_lsm->flush_buffer_reference_lines[snode_ppa(t)->g.blk] = true;
                    per_line_data(&ssd->lm.lines[snode_ppa(t)->g.blk])->referenced_flush_buffer = true;
                }
            }

            bool done = false;
            while (!done)
            {
                kv_skiplist *tmp = lksv_skiplist_cutting_header(lksv_lsm->key_only_imm, false, false, true);
                done = (tmp == lksv_lsm->key_only_imm);

                for_each_sk (t, tmp)
                {
                    if (!lksv_lsm->flush_reference_lines[snode_ppa(t)->g.blk])
                    {
                        lksv_lsm->flush_reference_lines[snode_ppa(t)->g.blk] = true;
                        per_line_data(&ssd->lm.lines[snode_ppa(t)->g.blk])->referenced_flush = true;
                    }

                    if (lksv_lsm->flush_buffer_reference_lines[snode_ppa(t)->g.blk])
                        per_line_data(&ssd->lm.lines[snode_ppa(t)->g.blk])->referenced_flush_buffer = false;
                }

                lnode.mem = tmp;
                lksv3_compaction_data_write(ssd, &lnode);
                compaction_selector(ssd, NULL, lksv_lsm->disk[0], &lnode);
                compaction_cascading(ssd);

                FREE(lnode.start.key);
                FREE(lnode.end.key);
                kv_skiplist_put(tmp);

                while (lksv3_should_meta_gc_high(ssd)) {
                    if (lksv3_gc_meta_femu(ssd))
                        break;
                }
            }

            lksv_lsm->key_only_imm = NULL;
        }

        FREE(req);

        if (rand() % 100 == 0) {
            kv_debug("write_cnt %lu\n", lksv_lsm->num_data_written);
            kv_debug("[META] free line cnt: %d\n", ssd->lm.meta.free_line_cnt);
            kv_debug("[META] full line cnt: %d\n", ssd->lm.meta.full_line_cnt);
            kv_debug("[META] victim line cnt: %d\n", ssd->lm.meta.victim_line_cnt);
            kv_debug("[DATA] free line cnt: %d\n", ssd->lm.data.free_line_cnt);
            kv_debug("[DATA] full line cnt: %d\n", ssd->lm.data.full_line_cnt);
            kv_debug("[DATA] victim line cnt: %d\n", ssd->lm.data.victim_line_cnt);
            lksv3_print_level_summary(lksv_lsm);
        }
    }
}

void lksv3_compaction_check(struct ssd *ssd) {
    if (kv_skiplist_approximate_memory_usage(lksv_lsm->mem) < WRITE_BUFFER_SIZE)
        return;

    compR *req = calloc(1, sizeof(compR));
    compaction_assign(lksv_lsm, req);
}

