#include <pthread.h>
#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

/*
 * lput puts an level list entry, dropping its reference count. If the entry
 * reference count hits zero, the entry is then freed.
 */
void
lksv_lput(lksv_level_list_entry *e)
{
    pthread_spin_lock(&lksv_lsm->level_list_entries_lock);

    e->ref_count--;
    if (e->ref_count == 0) {
        g_hash_table_remove(lksv_lsm->level_list_entries, &e->id);
        free(e);
    }

    pthread_spin_unlock(&lksv_lsm->level_list_entries_lock);
}

/*
 * lget obtains an level list entry, increasing its reference count.
 */
lksv_level_list_entry *
lksv_lget(uint64_t id)
{
    lksv_level_list_entry *e;

    pthread_spin_lock(&lksv_lsm->level_list_entries_lock);

    e = g_hash_table_lookup(lksv_lsm->level_list_entries, &id);
    kv_assert(e);
    e->ref_count++;

    pthread_spin_unlock(&lksv_lsm->level_list_entries_lock);

    return e;
}

/*
 * lnew creates an level list entry.
 */
lksv_level_list_entry *
lksv_lnew(void)
{
    lksv_level_list_entry *e = calloc(1, sizeof(lksv_level_list_entry));

    e->id = qatomic_fetch_inc(&lksv_lsm->next_level_list_entry_id);
    e->ref_count++;

    g_hash_table_insert(lksv_lsm->level_list_entries, &e->id, e);

    return e;
}

void
lksv_update_compaction_score(void)
{
    double compaction_score;
    double max_meta_segments;
    int i;

    lksv_lsm->compaction_level = 0;
    lksv_lsm->compaction_score = 0;

    for (i = 0; i < LSM_LEVELN-1; i++)
    {
        max_meta_segments = lksv_lsm->disk[i]->m_num * 0.9;
        compaction_score = (double) lksv_lsm->disk[i]->n_num / max_meta_segments;

        if (compaction_score > lksv_lsm->compaction_score)
        {
            lksv_lsm->compaction_score = compaction_score;
            lksv_lsm->compaction_level = i;
        }
    }
}

