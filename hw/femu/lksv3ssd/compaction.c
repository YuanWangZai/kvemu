#include "hw/femu/kvssd/lksv/lksv3_ftl.h"
#include "hw/femu/kvssd/lksv/skiplist.h"

void
lksv_comp_read_delay(struct femu_ppa *ppa)
{
    struct nand_cmd cpr;
    cpr.type = COMP_IO;
    cpr.cmd = NAND_READ;
    cpr.stime = 0;
    lksv3_ssd_advance_status(ppa, &cpr);
}

void
lksv_comp_write_delay(struct femu_ppa *ppa)
{
    struct nand_cmd cpw;
    cpw.type = COMP_IO;
    cpw.cmd = NAND_WRITE;
    cpw.stime = 0;
    lksv3_ssd_advance_status(ppa, &cpw);
}

static void
free_compaction(lksv_compaction *c)
{
    free(c->inputs[0]);
    free(c->inputs[1]);
    free(c);
}

static void
setup_other_inputs(lksv_compaction *c)
{
    c->inputs[1] = lksv_overlaps(c->level+1,
                                 c->inputs[0][0]->smallest,
                                 c->inputs[0][c->input_n[0]-1]->largest,
                                 &c->input_n[1]);
}

static lksv_compaction *
pick_compaction(void)
{
    lksv_compaction *c;

    if (lksv_lsm->versions.compaction_score <= 1)
        return NULL;

    c = (lksv_compaction *) malloc(sizeof(lksv_compaction));
    c->log_triggered = false;
    c->log_triggered_line_id = -1;
    c->level = lksv_lsm->versions.compaction_level;

    int size = lksv_lsm->versions.n_files[c->level] - lksv_lsm->versions.m_files[c->level];
    int offset = rand() % (lksv_lsm->versions.n_files[c->level] - size);

    c->inputs[0] = (lksv_level_list_entry **) malloc(size * sizeof(lksv_level_list_entry *));
    for (int i = 0; i < size; i++)
        c->inputs[0][i] = lksv_lsm->versions.files[c->level][offset+i];
    c->input_n[0] = size;

    setup_other_inputs(c);

    return c;
}

static gboolean
find_first_kv (gpointer key, gpointer value, gpointer user_data)
{
    return true;
}

static int
compact_disk_tables(lksv_compaction *c)
{
    lksv_write_level123_table(c);

    free_compaction(c);

    return 0;
}

static void
call_log_triggered_compaction(void)
{
    // TODO: pick the lowest referenced line.
    struct line *victim_line = NULL;
    int i, max;

    max = INT32_MAX;

    for (i = 0; i < lksv_ssd->lm.tt_lines; i++)
    {
        struct line *l = &lksv_ssd->lm.lines[i];

        if (l->meta)
            continue;

        if (l->vsc == 0 && l->isc == 0)
            continue;

        if (i == lksv_ssd->lm.data.wp.blk)
            continue;

        if (per_line_data(l)->referenced_by_memtable > 0)
            continue;

        if (per_line_data(l)->referenced_by_files < max)
        {
            victim_line = l;
            max = per_line_data(l)->referenced_by_files;
        }
    }

    if (!victim_line)
        return;

    kv_log("Log-triggered compaction: picked line id: %d\n", victim_line->id);

    const int max_compactions = 4;
    int compactions = 0;

    for (i = 0; i < LSM_LEVELN; i++)
    {
        while (true)
        {
            lksv_level_list_entry *e = g_hash_table_find(
                    per_line_data(victim_line)->files[i],
                    find_first_kv,
                    NULL);
            if (!e)
                break;

            lksv_compaction *c = malloc(sizeof(lksv_compaction));
            c->log_triggered = true;
            c->log_triggered_line_id = victim_line->id;
            c->level = i;
            c->inputs[0] = (lksv_level_list_entry **) malloc(1 * sizeof(lksv_level_list_entry *));
            c->inputs[0][0] = e;
            c->input_n[0] = 1;

            setup_other_inputs(c);

            compact_disk_tables(c);

            compactions++;
            if (compactions > max_compactions)
                return;
        }
    }

    lksv_ssd->lm.data.victim_line_cnt--;

    kv_assert(per_line_data(victim_line)->referenced_by_files == 0);
    lksv_gc_data_femu(victim_line->id);
}

static void
write_value_logs(kv_skiplist *skl)
{
    kv_snode *n, *t;
    kv_key k;
    kv_value *v;
    lksv_value_log_writer *w;
    lksv_kv_descriptor desc;

    w = lksv_new_value_log_writer();

    for_each_sk(n, skl)
    {
        desc.key = n->key;
        desc.value = *n->value;
        desc.hash = XXH32(desc.key.key, desc.key.len, 0);

        lksv_set_value_log_writer(w, &desc);

        kv_assert(!n->private);
        n->private = malloc(sizeof(lksv_per_snode_data));
        *snode_ppa(n) = desc.ppa;
        *snode_off(n) = desc.value_log_offset;
        *snode_hash(n) = desc.hash;
    }

    lksv_close_value_log_writer(w);

    for_each_sk(n, skl)
    {
        kv_copy_key(&k, &n->key);
        v = calloc(1, sizeof(kv_value));
        v->length = PPA_LENGTH;

        t = lksv3_skiplist_insert(lksv_lsm->key_only_mem, k, v, true);

        if (t->private)
        {
            per_line_data(
                    &lksv_ssd->lm.lines[snode_ppa(t)->g.blk]
            )->referenced_by_memtable--;
            free(t->private);
        }

        t->private = n->private;
        n->private = NULL;

        per_line_data(
                &lksv_ssd->lm.lines[snode_ppa(t)->g.blk]
        )->referenced_by_memtable++;
    }
}

static void
print_stats(void)
{
    kv_debug("[META] free line cnt: %d\n", lksv_ssd->lm.meta.free_line_cnt);
    kv_debug("[META] full line cnt: %d\n", lksv_ssd->lm.meta.full_line_cnt);
    kv_debug("[META] victim line cnt: %d\n", lksv_ssd->lm.meta.victim_line_cnt);
    kv_debug("[DATA] free line cnt: %d\n", lksv_ssd->lm.data.free_line_cnt);
    kv_debug("[DATA] full line cnt: %d\n", lksv_ssd->lm.data.full_line_cnt);
    kv_debug("[DATA] victim line cnt: %d\n", lksv_ssd->lm.data.victim_line_cnt);
}

static int
compact_memtable(void)
{
    if (rand() % 100 == 0)
        print_stats();

    if (lksv_lsm->imm)
    {
        write_value_logs(lksv_lsm->imm);

        kv_skiplist_put(lksv_lsm->imm);
        lksv_lsm->imm = NULL;
    }

    if (lksv_lsm->key_only_imm)
    {
        lksv_write_level0_table(lksv_lsm->key_only_imm);

        kv_skiplist_put(lksv_lsm->key_only_imm);
        lksv_lsm->key_only_imm = NULL;
    }
    else if (kv_skiplist_approximate_memory_usage(lksv_lsm->key_only_mem) >= KEY_ONLY_WRITE_BUFFER_SIZE)
    {
        lksv_lsm->key_only_imm = lksv_lsm->key_only_mem;
        lksv_lsm->key_only_mem = kv_skiplist_init();
    }

    return 0;
}

static int
compact1(void)
{
    lksv_compaction *c;

    if (lksv_lsm->imm || lksv_lsm->key_only_imm)
    {
        compact_memtable();

        // TODO: move this function into the version update function.
        lksv_update_compaction_score();
    }

    c = pick_compaction();

    if (c)
    {
        compact_disk_tables(c);

        // TODO: move this function into the version update function.
        lksv_update_compaction_score();
    }

    while (lksv3_should_meta_gc_high(0))
    {
        if (lksv_gc_meta_femu() < 0)
            break;
    }

    if (lksv3_should_data_gc_high(0))
    {
        call_log_triggered_compaction();

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

            usleep(1000);
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
        if (lksv_lsm->versions.compaction_score < 1)
            return;
    }

    lksv_lsm->compacting = true;

    qatomic_inc(&lksv_lsm->compaction_calls);
}

