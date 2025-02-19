#include <pthread.h>
#include "hw/femu/kvssd/pink/pink_ftl.h"

/*
 * lput puts an level list entry, dropping its reference count. If the entry
 * reference count hits zero, the entry is then freed.
 */
void
pink_lput(pink_level_list_entry *e)
{
    pthread_spin_lock(&pink_lsm->level_list_entries_lock);

    e->ref_count--;
    if (e->ref_count == 0) {
        g_hash_table_remove(pink_lsm->level_list_entries, &e->id);
        free(e);
    }

    pthread_spin_unlock(&pink_lsm->level_list_entries_lock);
}

/*
 * lget obtains an level list entry, increasing its reference count.
 */
pink_level_list_entry *
pink_lget(uint64_t id)
{
    pink_level_list_entry *e;

    pthread_spin_lock(&pink_lsm->level_list_entries_lock);

    e = g_hash_table_lookup(pink_lsm->level_list_entries, &id);
    kv_assert(e);
    e->ref_count++;

    pthread_spin_unlock(&pink_lsm->level_list_entries_lock);

    return e;
}

/*
 * lnew creates an level list entry.
 */
pink_level_list_entry *
pink_lnew(void)
{
    pink_level_list_entry *e = calloc(1, sizeof(pink_level_list_entry));

    e->id = qatomic_fetch_inc(&pink_lsm->next_level_list_entry_id);
    e->ref_count++;

    g_hash_table_insert(pink_lsm->level_list_entries, &e->id, e);

    return e;
}

