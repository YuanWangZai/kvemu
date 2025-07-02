#include "hw/femu/kvssd/pink/pink_ftl.h"

struct pink_lsmtree *pink_lsm;

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

void pink_open(struct kv_lsm_options *opts)
{
    pink_lsm = calloc(1, sizeof(struct pink_lsmtree));
    pink_lsm->opts = opts;
    pink_lsm_adjust_level_multiplier();

    kv_init_min_max_key();
    pink_lsm->mem = kv_skiplist_init();
    pink_lsm->key_only_mem = kv_skiplist_init();
    qemu_mutex_init(&pink_lsm->mu);

    pink_lsm->level_list_entries = g_hash_table_new(uint64_t_hash,
                                                    uint64_t_equal);
    pthread_spin_init(&pink_lsm->level_list_entries_lock,
                      PTHREAD_PROCESS_PRIVATE);

    pink_lsm->lsm_cache = kv_cache_init(pink_lsm->opts->cache_memory_size, LSM_LEVELN * CACHE_TYPES);
    pink_compaction_init();
}
