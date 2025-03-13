#include "hw/femu/kvssd/lksv/lksv3_ftl.h"
#include "hw/femu/kvssd/lksv/skiplist.h"

static void compaction_selector(struct ssd *ssd, lksv3_level *a, lksv3_level *b, leveling_node *lnode){
    lksv3_leveling(ssd, a, b, lnode);
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

static void
print_stats(void)
{
    if (rand() % 100 == 0) {
        kv_debug("write_cnt %lu\n", lksv_lsm->num_data_written);
        kv_debug("[META] free line cnt: %d\n", lksv_lsm->ssd->lm.meta.free_line_cnt);
        kv_debug("[META] full line cnt: %d\n", lksv_lsm->ssd->lm.meta.full_line_cnt);
        kv_debug("[META] victim line cnt: %d\n", lksv_lsm->ssd->lm.meta.victim_line_cnt);
        kv_debug("[DATA] free line cnt: %d\n", lksv_lsm->ssd->lm.data.free_line_cnt);
        kv_debug("[DATA] full line cnt: %d\n", lksv_lsm->ssd->lm.data.full_line_cnt);
        kv_debug("[DATA] victim line cnt: %d\n", lksv_lsm->ssd->lm.data.victim_line_cnt);
        lksv3_print_level_summary(lksv_lsm);
    }
}

static int
compact_memtable(void)
{
    print_stats();

    if (lksv_lsm->imm)
    {
        log_write(lksv_lsm->ssd, lksv_lsm->imm);
        check_473(lksv_lsm->ssd);

        kv_skiplist_put(lksv_lsm->imm);
        lksv_lsm->imm = NULL;
    }

    if (lksv_lsm->key_only_imm)
    {
        leveling_node lnode;
        kv_skiplist *tmp = lksv_skiplist_cutting_header(lksv_lsm->key_only_imm, false, false, true);
        if (tmp == lksv_lsm->key_only_imm)
            lksv_lsm->key_only_imm = NULL;

        kv_snode *t;
        for_each_sk (t, tmp)
        {
            if (!lksv_lsm->flush_reference_lines[snode_ppa(t)->g.blk])
            {
                lksv_lsm->flush_reference_lines[snode_ppa(t)->g.blk] = true;
                per_line_data(&lksv_lsm->ssd->lm.lines[snode_ppa(t)->g.blk])->referenced_flush = true;
            }

            if (lksv_lsm->flush_buffer_reference_lines[snode_ppa(t)->g.blk])
                per_line_data(&lksv_lsm->ssd->lm.lines[snode_ppa(t)->g.blk])->referenced_flush_buffer = false;
        }

        lnode.mem = tmp;
        kv_skiplist_get_start_end_key(lnode.mem, &lnode.start, &lnode.end);
        compaction_selector(lksv_lsm->ssd, NULL, lksv_lsm->disk[0], &lnode);

        FREE(lnode.start.key);
        FREE(lnode.end.key);

        kv_skiplist_put(tmp);

        while (lksv3_should_meta_gc_high(lksv_lsm->ssd)) {
            if (lksv3_gc_meta_femu(lksv_lsm->ssd))
                break;
        }
    }
    else if (kv_skiplist_approximate_memory_usage(lksv_lsm->key_only_mem) >= KEY_ONLY_WRITE_BUFFER_SIZE)
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
                per_line_data(&lksv_lsm->ssd->lm.lines[snode_ppa(t)->g.blk])->referenced_flush_buffer = true;
            }
        }
    }

    return 0;
}

static int
compact_disk_tables(void)
{
    compaction_selector(lksv_lsm->ssd,
                        lksv_lsm->disk[lksv_lsm->compaction_level],
                        lksv_lsm->disk[lksv_lsm->compaction_level+1],
                        NULL);
    update_lines(lksv_lsm->ssd);

    if (lksv3_should_data_gc_high(lksv_lsm->ssd, 0))
    {
        call_log_triggered_compaction(lksv_lsm->ssd);
        update_lines(lksv_lsm->ssd);
    }

    if (lksv_lsm->ssd->lm.data.lines > lksv_lsm->ssd->sp.tt_lines - lksv_lsm->t_meta)
        move_line_d2m(lksv_lsm->ssd, false);
    else if (lksv_lsm->ssd->lm.data.lines < lksv_lsm->ssd->sp.tt_lines - lksv_lsm->t_meta)
        move_line_m2d(lksv_lsm->ssd, false);

    return 0;
}

static int
compact1(void)
{
    if (lksv_lsm->imm || lksv_lsm->key_only_imm)
    {
        compact_memtable();

        // TODO: move this function into the version update function.
        lksv_update_compaction_score();
    }

    if (lksv_lsm->compaction_score >= 1)
    {
        compact_disk_tables();

        // TODO: move this function into the version update function.
        lksv_update_compaction_score();
    }

    return 0;
}

static void *
compact(void *arg)
{
    while (true)
    {
        if (qatomic_read(&lksv_lsm->compaction_calls) > 0)
        {
            qatomic_dec(&lksv_lsm->compaction_calls);

            qemu_mutex_lock(&lksv_lsm->mu);

            if (compact1() != 0)
            {
                // TODO: handling error.
            }

            lksv_lsm->compacting = false;

            lksv_maybe_schedule_compaction();

            qemu_mutex_unlock(&lksv_lsm->mu);
        }
    }

    return NULL;
}

void
lksv_compaction_init(void)
{
    qemu_thread_create(&lksv_lsm->comp_thread, "FEMU-COMP-Thread",
                       compact, NULL, QEMU_THREAD_JOINABLE);
}

void
lksv_maybe_schedule_compaction(void)
{
    if (lksv_lsm->compacting)
        return;

    if (!lksv_lsm->imm && !lksv_lsm->key_only_imm)
    {
        if (lksv_lsm->compaction_score < 1)
            return;
    }

    lksv_lsm->compacting = true;

    qatomic_inc(&lksv_lsm->compaction_calls);
}

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

