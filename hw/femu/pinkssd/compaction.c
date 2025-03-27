#include "hw/femu/kvssd/pink/pink_ftl.h"
#include "hw/femu/kvssd/pink/skiplist.h"

void
pink_comp_read_delay(struct femu_ppa *ppa)
{
    struct nand_cmd cpr;
    cpr.type = COMP_IO;
    cpr.cmd = NAND_READ;
    cpr.stime = 0;
    pink_ssd_advance_status(ppa, &cpr);
}

void
pink_comp_write_delay(struct femu_ppa *ppa)
{
    struct nand_cmd cpw;
    cpw.type = COMP_IO;
    cpw.cmd = NAND_WRITE;
    cpw.stime = 0;
    pink_ssd_advance_status(ppa, &cpw);
}

static void
write_data_segments(kv_skiplist *skl)
{
    kv_snode *n, *t;
    kv_key k;
    kv_value *v;
    pink_data_seg_writer *w;
    // TODO: fix this.
    pink_kv_descriptor *desc = malloc(skl->n * sizeof(pink_kv_descriptor));
    int i = 0;

    w = pink_new_data_seg_writer();

    for_each_sk(n, skl)
    {
        desc[i].key = n->key;
        desc[i].value = *n->value;

        pink_set_data_seg_writer(w, &desc[i]);

        kv_assert(!n->private);
        n->private = malloc(sizeof(pink_per_snode_data));
        *snode_ppa(n) = desc[i].ppa;
        *snode_off(n) = desc[i].data_seg_offset.g.in_page_idx;

        i++;
    }

    pink_close_data_seg_writer(w);

    for_each_sk(n, skl)
    {
        kv_copy_key(&k, &n->key);
        v = calloc(1, sizeof(kv_value));
        v->length = PPA_LENGTH;

        t = pink_skiplist_insert(pink_lsm->key_only_mem, k, v);

        if (t->private)
            free(t->private);

        t->private = n->private;
        n->private = NULL;
    }

    free(desc);
}

static void
print_stats(void)
{
    kv_debug("write_cnt %lu\n", pink_lsm->num_data_written);
    kv_debug("[META] free line cnt: %d\n", pink_ssd->lm.meta.free_line_cnt);
    kv_debug("[META] full line cnt: %d\n", pink_ssd->lm.meta.full_line_cnt);
    kv_debug("[META] victim line cnt: %d\n", pink_ssd->lm.meta.victim_line_cnt);
    kv_debug("[DATA] free line cnt: %d\n", pink_ssd->lm.data.free_line_cnt);
    kv_debug("[DATA] full line cnt: %d\n", pink_ssd->lm.data.full_line_cnt);
    kv_debug("[DATA] victim line cnt: %d\n", pink_ssd->lm.data.victim_line_cnt);
}

static int
compact_memtable(void)
{
    if (rand() % 100 == 0)
        print_stats();

    if (pink_lsm->imm)
    {
        write_data_segments(pink_lsm->imm);

        kv_skiplist_put(pink_lsm->imm);
        pink_lsm->imm = NULL;
    }

    if (pink_lsm->key_only_imm)
    {
        pink_write_level0_table(pink_lsm->key_only_imm);

        kv_skiplist_put(pink_lsm->key_only_imm);
        pink_lsm->key_only_imm = NULL;
    }
    else if (kv_skiplist_approximate_memory_usage(pink_lsm->key_only_mem) >= KEY_ONLY_WRITE_BUFFER_SIZE)
    {
        pink_lsm->key_only_imm = pink_lsm->key_only_mem;
        pink_lsm->key_only_mem = kv_skiplist_init();
    }

    return 0;
}

static void
free_compaction(pink_compaction *c)
{
    free(c->inputs[0]);
    free(c->inputs[1]);
    free(c);
}

static void
setup_other_inputs(pink_compaction *c)
{
    c->inputs[1] = pink_overlaps(c->level+1,
                                 c->inputs[0][0]->smallest,
                                 c->inputs[0][c->input_n[0]-1]->largest,
                                 &c->input_n[1]);
}

static pink_compaction *
pick_compaction(void)
{
    pink_compaction *c;

    if (pink_lsm->versions.compaction_score <= 1)
        return NULL;

    c = (pink_compaction *) malloc(sizeof(pink_compaction));
    c->level = pink_lsm->versions.compaction_level;

    int size = pink_lsm->versions.n_files[c->level] - pink_lsm->versions.m_files[c->level];
    int offset = rand() % (pink_lsm->versions.n_files[c->level] - size);

    c->inputs[0] = (pink_level_list_entry **) malloc(size * sizeof(pink_level_list_entry *));
    for (int i = 0; i < size; i++)
        c->inputs[0][i] = pink_lsm->versions.files[c->level][offset+i];
    c->input_n[0] = size;

    setup_other_inputs(c);

    return c;
}

static int
compact_disk_tables(pink_compaction *c)
{
    pink_write_level123_table(c);

    if (c->level == LSM_LEVELN-2)
        pink_lsm_adjust_level_multiplier();

    free_compaction(c);

    return 0;
}

static int
compact1(void)
{
    pink_compaction *c;

    if (pink_lsm->imm || pink_lsm->key_only_imm)
    {
        compact_memtable();

        // TODO: move this function into the version update function.
        pink_update_compaction_score();
    }

    c = pick_compaction();

    if (c)
    {
        compact_disk_tables(c);

        pink_update_compaction_score();
    }

    while (pink_should_meta_gc_high(0))
    {
        if (pink_gc_meta_femu() < 0)
            break;
    }

    while (pink_should_data_gc_high(0))
    {
        if (pink_gc_data_femu() < 0)
            break;
    }

    return 0;
}

static void *
compact(void *arg)
{
    while (true)
    {
        if (qatomic_read(&pink_lsm->compaction_calls) > 0)
        {
            qatomic_dec(&pink_lsm->compaction_calls);

            qemu_mutex_lock(&pink_lsm->mu);

            if (compact1() != 0)
            {
                // TODO: handling error.
            }

            pink_lsm->compacting = false;

            pink_maybe_schedule_compaction();

            qemu_mutex_unlock(&pink_lsm->mu);
        }
    }

    return NULL;
}

void
pink_compaction_init(void)
{
    qemu_thread_create(&pink_lsm->comp_thread, "FEMU-COMP-Thread",
                       compact, NULL, QEMU_THREAD_JOINABLE);
}

void
pink_maybe_schedule_compaction(void)
{
    if (pink_lsm->compacting)
        return;

    if (!pink_lsm->imm && !pink_lsm->key_only_imm)
    {
        if (pink_lsm->versions.compaction_score < 1)
            return;
    }

    pink_lsm->compacting = true;

    qatomic_inc(&pink_lsm->compaction_calls);
}

