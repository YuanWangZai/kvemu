#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

struct lksv3_lsmtree *lksv_lsm;

static void
lsm_setup_params(void)
{
    int showing_pages = lksv_ssd->sp.tt_pgs * 90 / 100;
    int data_segment_groups = showing_pages / PG_N;

    lksv_lsm->opts->level_multiplier = kv_calc_level_multiplier(data_segment_groups);
}

static void
lsm_setup_levels(void)
{
    int i, max_files;

    max_files = lksv_lsm->opts->level_multiplier;

    for (i = 0; i < LSM_LEVELN; i++)
    {
        lksv_lsm->versions.m_files[i] = max_files;
        max_files *= lksv_lsm->opts->level_multiplier;
    }
}

static gboolean
uint64_t_equal (gconstpointer v1, gconstpointer v2)
{
      return *((const uint64_t*) v1) == *((const uint64_t*) v2);
}

static guint
uint64_t_hash (gconstpointer v)
{
      return (guint) *(const uint64_t*) v;
}

void lksv_open(struct kv_lsm_options *opts)
{
    for (int i = 0; i < lksv_ssd->lm.tt_lines; i++)
    {
        lksv_ssd->lm.lines[i].private = calloc(1, sizeof(struct per_line_data));
        for (int j = 0; j < LSM_LEVELN; j++)
        {
            per_line_data(&lksv_ssd->lm.lines[i])->files[j] =
                g_hash_table_new(uint64_t_hash, uint64_t_equal);
        }
    }

    lksv_lsm = calloc(1, sizeof(struct lksv3_lsmtree));
    lksv_lsm->opts = opts;

    kv_init_min_max_key();
    lksv_lsm->mem = kv_skiplist_init();
    lksv_lsm->key_only_mem = kv_skiplist_init();
    qemu_mutex_init(&lksv_lsm->mu);

    lksv_lsm->level_list_entries = g_hash_table_new(uint64_t_hash,
                                                    uint64_t_equal);
    pthread_spin_init(&lksv_lsm->level_list_entries_lock,
                      PTHREAD_PROCESS_PRIVATE);

    lsm_setup_params();
    lsm_setup_levels();
    lksv_lsm->lsm_cache = kv_cache_init(lksv_lsm->opts->cache_memory_size, LSM_LEVELN * CACHE_TYPES);

    lksv_compaction_init();
}
