#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

static void comp_write_delay(struct femu_ppa *ppa)
{
    if (lksv_ssd->sp.enable_comp_delay) {
        struct nand_cmd cpw;
        cpw.type = COMP_IO;
        cpw.cmd = NAND_WRITE;
        cpw.stime = 0;
        lksv3_ssd_advance_status(ppa, &cpw);
    }
}

struct femu_ppa lksv3_compaction_meta_segment_write_femu(char *data, int level) {
    struct femu_ppa fppa;
    struct nand_page *pg;

    fppa = lksv3_get_new_meta_page();
    pg = lksv3_get_pg(&fppa);
    comp_write_delay(&fppa);

    lksv3_mark_page_valid(&fppa);
    lksv3_ssd_advance_write_pointer(&lksv_ssd->lm.meta);

    FREE(pg->data);
    pg->data = data;

    return fppa;
}

struct femu_ppa lksv3_compaction_meta_segment_write_insert_femu(lksv3_level *target, lksv_level_list_entry *entry)
{
    struct femu_ppa fppa;

    kv_assert(entry->hash_list_n > 0);
    kv_assert(entry->hash_list_n <= PG_N);
    for (int i = 0; i < entry->hash_list_n; i++) {
        fppa = lksv3_compaction_meta_segment_write_femu((char *) entry->buffer[i], target->idx);
        entry->buffer[i] = NULL;

        if (i == 0) {
            entry->ppa = fppa;
            kv_assert(fppa.g.ch == 0);
        }
    }

    for (int i = entry->hash_list_n; i < PG_N; i++) {
        kv_assert(i > 0);
        fppa = lksv3_get_new_meta_page();
        lksv3_mark_page_valid(&fppa);
        lksv3_ssd_advance_write_pointer(&lksv_ssd->lm.meta);
        lksv3_mark_page_invalid(&fppa);
    }

    lksv3_insert_run(target, entry);
    lksv3_free_run(lksv_lsm, entry);

    return fppa;
}

