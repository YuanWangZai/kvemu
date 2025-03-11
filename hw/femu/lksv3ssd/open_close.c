#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

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
    lksv_lsm = calloc(1, sizeof(struct lksv3_lsmtree));
    lksv_lsm->opts = opts;

    kv_init_min_max_key();
    lksv_lsm->mem = kv_skiplist_init();
    lksv_lsm->key_only_mem = kv_skiplist_init();

    lksv_lsm->level_list_entries = g_hash_table_new(uint64_t_hash,
                                                    uint64_t_equal);
    pthread_spin_init(&lksv_lsm->level_list_entries_lock,
                      PTHREAD_PROCESS_PRIVATE);
}
