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
        kv_cache_delete_entry(pink_lsm->lsm_cache, e->cache[LEVEL_LIST_ENTRY]);
        kv_cache_delete_entry(pink_lsm->lsm_cache, e->cache[META_SEGMENT]);

        free(e->smallest.key);
        free(e->largest.key);

        mark_page_invalid(&e->ppa);

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

void
pink_update_compaction_score(void)
{
    double compaction_score;
    int i;

    pink_lsm->versions.compaction_level = 0;
    pink_lsm->versions.compaction_score = 0;

    for (i = 0; i < LSM_LEVELN-1; i++)
    {
        compaction_score = (double) pink_lsm->versions.n_files[i] / (double) pink_lsm->versions.m_files[i];

        if (compaction_score > pink_lsm->versions.compaction_score)
        {
            pink_lsm->versions.compaction_score = compaction_score;
            pink_lsm->versions.compaction_level = i;
        }
    }
}

pink_level_list_entry **
pink_overlaps(int level, kv_key smallest, kv_key largest, int *n)
{
    pink_level_list_entry **ret;
    pink_level_list_entry *e;
    int i, start, end;

    start = -1;

    for (i = 0; i < pink_lsm->versions.n_files[level]; i++)
    {
        e = pink_lsm->versions.files[level][i];
        
        if (kv_cmp_key(e->largest, smallest) < 0)
            continue;

        if (kv_cmp_key(e->smallest, largest) > 0)
            break;

        if (start == -1)
            start = i;

        end = i;
    }

    if (start == -1)
    {
        *n = 0;
        return NULL;
    }

    *n = end - start + 1;

    ret = malloc((*n) * sizeof(pink_level_list_entry *));

    for (i = start; i < end + 1; i++)
        ret[i-start] = pink_lsm->versions.files[level][i];

    return ret;
}

static bool
move_line_d2m(void)
{
    struct line_partition *m = &pink_ssd->lm.meta;
    struct line_partition *d = &pink_ssd->lm.data;
    struct line *line;

    if (d->free_line_cnt == 0) {
        return false;
    }

    line = get_next_free_line(d);
    kv_assert(!line->meta);
    d->lines--;

    QTAILQ_INSERT_TAIL(&m->free_line_list, line, entry);
    line->meta = true;
    m->free_line_cnt++;
    m->lines++;

    kv_log("d2m line(%d): data lines(%d), meta lines(%d)\n", line->id, d->lines, m->lines);
    return true;
}

static void
pink_adjust_lines(void)
{
    int compaction_margin = 10 + (pink_ssd->lm.meta.lines / 10);
    int min_meta_lines;
    int meta_pages = 0;

    for (int i = 0; i < LSM_LEVELN; i++)
        meta_pages += pink_lsm->versions.m_files[i];

    min_meta_lines = (meta_pages / pink_ssd->sp.pgs_per_line) + compaction_margin;

    pink_lsm->should_d2m = false;
    while (pink_ssd->lm.meta.lines < min_meta_lines)
    {
        if (!move_line_d2m())
        {
            pink_lsm->should_d2m = true;
            break;
        }
    }
}

void
pink_lsm_adjust_level_multiplier(void)
{
    if (pink_lsm->versions.n_files[LSM_LEVELN-1] >= pink_lsm->versions.m_files[LSM_LEVELN-1] * 9 / 10)
    {
        kv_debug("increase level multiplier %.2f to %.2f\n",
                pink_lsm->opts->level_multiplier,
                pink_lsm->opts->level_multiplier + 1);
        pink_lsm->opts->level_multiplier += 1;

        int m_files = pink_lsm->opts->level_multiplier;
        for (int i = 0; i < LSM_LEVELN; i++)
        {
            pink_lsm->versions.m_files[i] = m_files;
            m_files *= pink_lsm->opts->level_multiplier;
        }

        pink_lsm->should_d2m = true;
    }

    if (pink_lsm->should_d2m)
        pink_adjust_lines();
}

void
pink_user_read_delay(struct femu_ppa *ppa, NvmeRequest *req)
{
    struct nand_cmd srd;
    srd.type = USER_IO;
    srd.cmd = NAND_READ;

    if (req)
    {
        srd.stime = req->etime;
        req->flash_access_count++;
    }
    else
        srd.stime = 0;

    uint64_t sublat = pink_ssd_advance_status(ppa, &srd);

    if (req)
        req->etime += sublat;
}

static int
binary_search(int level, kv_key k, NvmeRequest *req)
{
    int end = pink_lsm->versions.n_files[level];
    int start = 0;
    int mid, res;

    while (start < end)
    {
        mid = (start + end) / 2;

        if (!kv_is_cached(pink_lsm->lsm_cache,
                          pink_lsm->versions.files[level][mid]->cache[LEVEL_LIST_ENTRY]))
        {
            // TODO: Change the fake PPA to real one.
            struct femu_ppa fake_ppa = { .ppa = 0 };
            fake_ppa.g.ch = mid % 8;
            fake_ppa.g.lun = (mid * 3) % 8;
            pink_user_read_delay(&fake_ppa, req);
        }

        res = kv_cmp_key(pink_lsm->versions.files[level][mid]->largest, k);

        if (res < 0)
            start = mid + 1;
        else
            end = mid;
    }

    return start;
}

kv_value *
pink_get(kv_key k, NvmeRequest *req)
{
    kv_value *v = NULL;

    for (int i = 0; i < LSM_LEVELN; i++)
    {
        if (pink_lsm->versions.n_files[i] == 0)
            continue;

        int idx = binary_search(i, k, req);

        if (idx == pink_lsm->versions.n_files[i])
            continue;

        pink_level_list_entry *e = pink_lsm->versions.files[i][idx];

        if (kv_cmp_key(e->smallest, k) > 0)
            continue;

        v = pink_internal_get(e->id, k, req);

        if (v)
            return v;
    }

    return v;
}

