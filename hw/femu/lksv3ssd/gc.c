#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

static void
gc_erase_delay(struct femu_ppa *ppa)
{
    struct nand_cmd gce;
    gce.type = GC_IO;
    gce.cmd = NAND_ERASE;
    gce.stime = 0;
    lksv3_ssd_advance_status(ppa, &gce);
}

static void
gc_read_delay(struct femu_ppa *ppa)
{
    struct nand_cmd gcr;
    gcr.type = GC_IO;
    gcr.cmd = NAND_READ;
    gcr.stime = 0;
    lksv3_ssd_advance_status(ppa, &gcr);
}

static void
gc_write_delay(struct femu_ppa *ppa)
{
    struct nand_cmd gcw;
    gcw.type = GC_IO;
    gcw.cmd = NAND_WRITE;
    gcw.stime = 0;
    lksv3_ssd_advance_status(ppa, &gcw);
}

void
lksv_gc_data_femu(int lineid)
{
    struct line *line = &lksv_ssd->lm.lines[lineid];
    struct femu_ppa ppa;

    kv_log("%d gc_data!!! (line: %d)\n", ++lksv_lsm->data_gc_cnt, line->id);
    kv_log("vpc: %d, valid bytes: %d, invalid_bytes: %d, secs_per_line: %d\n", line->vpc, line->vsc, line->isc, lksv_ssd->sp.secs_per_line);
    kv_log("free line left: %d\n", lksv_ssd->lm.data.free_line_cnt);

    ppa.ppa = 0;
    ppa.g.blk = lineid;
    for (int ch = 0; ch < lksv_ssd->sp.nchs; ch++) {
        for (int lun = 0; lun < lksv_ssd->sp.luns_per_ch; lun++) {
            ppa.g.ch = ch;
            ppa.g.lun = lun;
            ppa.g.pl = 0;
            ppa.g.sec = 0;
            ppa.g.rsv = 0;

            gc_erase_delay(&ppa);
            lksv3_mark_block_free(&ppa);
        }
    }

    if (per_line_data(line)->referenced_by_files > 0 &&
        per_line_data(line)->referenced_by_memtable > 0)
        abort();

    lksv3_mark_line_free(&ppa);
}

int
lksv_gc_meta_femu(void)
{
    struct line *victim_line = NULL;
    struct ssdparams *spp = &lksv_ssd->sp;
    struct femu_ppa ppa;
    int ch, lun, pg_n;

    victim_line = lksv3_select_victim_meta_line(false);
    if (victim_line == NULL)
        return -1;

    kv_log("%d gc_meta! (line: %d) invalid_pgs / pgs_per_line: %d / %d, vpc: %d \n", ++lksv_lsm->header_gc_cnt, victim_line->id, victim_line->ipc, lksv_ssd->sp.pgs_per_line, victim_line->vpc);

    ppa.ppa = 0;
    ppa.g.blk = victim_line->id;

    struct nand_page *pg;
    for (pg_n = 0; pg_n < spp->pgs_per_blk; pg_n++) {
        ppa.g.pg = pg_n;
        for (lun = 0; lun < spp->luns_per_ch; lun++) {
            ppa.g.lun = lun;
            for (ch = 0; ch < spp->nchs; ch++) {
                ppa.g.ch = ch;

                pg = lksv3_get_pg(&ppa);
                if (pg->status == PG_INVALID) {
                } else if (pg->status == PG_VALID) {
                    lksv_block_footer f;
                    f = *(lksv_block_footer *) (pg->data + (PAGESIZE - LKSV3_SSTABLE_FOOTER_BLK_SIZE));
                    lksv_level_list_entry *e = lksv_lget(f.level_list_entry_id);
                    struct femu_ppa new_ppa = lksv3_get_new_meta_page();

                    int i;
                    for (i = 0; i < e->hash_list_n; i++)
                    {
                        struct femu_ppa src_ppa = get_next_write_ppa(e->ppa, i);
                        struct nand_page *src_pg = lksv3_get_pg(&src_ppa);
                        gc_read_delay(&src_ppa);

                        struct femu_ppa dst_ppa = lksv3_get_new_meta_page();
                        struct nand_page *dst_pg = lksv3_get_pg(&dst_ppa);
                        lksv3_ssd_advance_write_pointer(&lksv_ssd->lm.meta);
                        gc_write_delay(&dst_ppa);

                        memcpy(dst_pg->data, src_pg->data, PAGESIZE);
                        lksv3_mark_page_invalid(&src_ppa);
                        lksv3_mark_page_valid(&dst_ppa);
                    }
                    for (; i < PG_N; i++)
                    {
                        struct femu_ppa dst_ppa = lksv3_get_new_meta_page();
                        lksv3_ssd_advance_write_pointer(&lksv_ssd->lm.meta);
                        lksv3_mark_page_valid(&dst_ppa);
                        lksv3_mark_page_invalid(&dst_ppa);
                    }

                    e->ppa = new_ppa;

                    lksv_lput(e);
                } else {
                    // TODO: handling PG_VALID case.
                    abort();
                }
            }
        }
    }

    for (ch = 0; ch < spp->nchs; ch++) {
        for (lun = 0; lun < spp->luns_per_ch; lun++) {
            ppa.g.ch = ch;
            ppa.g.lun = lun;

            gc_erase_delay(&ppa);
            lksv3_mark_block_free(&ppa);
        }
    }

    lksv3_mark_line_free(&ppa);

    return -1;
}

void
lksv_gc_meta_erase_only(void)
{
    struct line *victim_line = NULL;
    struct line *popped_line[2];
    int popped_n = 0;

retry:
    victim_line = pqueue_peek(lksv_ssd->lm.meta.victim_line_pq);

    if (victim_line == NULL)
        goto out;

    if (lksv_lsm->compacting_meta_lines[0][victim_line->id] ||
        lksv_lsm->compacting_meta_lines[1][victim_line->id])
    {
        kv_assert(popped_n < 2);
        popped_line[popped_n++] = victim_line;
        pqueue_pop(lksv_ssd->lm.meta.victim_line_pq);
        lksv_ssd->lm.meta.victim_line_cnt--;
        goto retry;
    }

    lksv_gc_meta_femu();

out:
    for (int i = 0; i < popped_n; i++)
    {
        pqueue_insert(lksv_ssd->lm.meta.victim_line_pq, popped_line[i]);
        lksv_ssd->lm.meta.victim_line_cnt++;
    }
}

