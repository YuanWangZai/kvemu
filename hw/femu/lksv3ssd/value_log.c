#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

static void
next_page(lksv_value_log_writer *w)
{
    struct nand_page *pg;

    w->writing_ppa = lksv3_get_new_data_page();

    pg = lksv3_get_pg(&w->writing_ppa);

    if (!pg->data)
        pg->data = calloc(1, PAGESIZE);

    w->left = PAGESIZE - LKSV3_SSTABLE_FOOTER_BLK_SIZE;
    w->wp = 0;
    w->n = 0;
}

static void
flush_page(lksv_value_log_writer *w)
{
    struct line *line = lksv3_get_line(&w->writing_ppa);

    line->vsc += w->n;
    lksv3_mark_page_valid(&w->writing_ppa);
    lksv3_ssd_advance_write_pointer(&lksv_ssd->lm.data);

    lksv_comp_write_delay(&w->writing_ppa);
}

lksv_value_log_writer *
lksv_new_value_log_writer(void)
{
    lksv_value_log_writer *w = calloc(1, sizeof(lksv_value_log_writer));

    next_page(w);

    return w;
}

void
lksv_set_value_log_writer(lksv_value_log_writer *w, lksv_kv_descriptor *d)
{
    struct nand_page *pg;
    lksv_block_footer footer;
    lksv_block_meta meta;
    int write_size = d->key.len +
                     d->value.length +
                     LKSV3_SSTABLE_META_BLK_SIZE +
                     LKSV3_SSTABLE_STR_IDX_SIZE;

    if (w->left < write_size)
    {
        flush_page(w);
        next_page(w);
    }
    w->left -= write_size;

    pg = lksv3_get_pg(&w->writing_ppa);
    d->ppa = w->writing_ppa;
    d->value_log_offset = w->n;

    /* Write footer */
    w->n += 1;
    footer.f = 0;
    footer.g.n = w->n;
    memcpy(pg->data + (PAGESIZE - LKSV3_SSTABLE_FOOTER_BLK_SIZE),
           &footer, sizeof(lksv_block_footer));

    /* Write meta */
    meta.m1 = meta.m2 = 0;
    meta.g1.off = w->wp;
    meta.g1.klen = d->key.len;
    meta.g1.flag = VALUE_LOG;
    meta.g1.hash = d->hash;
    meta.g2.snum = 1;
    meta.g2.slen = d->value.length;
    memcpy(pg->data + (
                PAGESIZE -
                LKSV3_SSTABLE_FOOTER_BLK_SIZE -
                (LKSV3_SSTABLE_META_BLK_SIZE * w->n)
           ), &meta, sizeof(lksv_block_meta));

    /* Write payload */
    memcpy(pg->data + w->wp, d->key.key, d->key.len);
    w->wp += d->key.len;

    memcpy(pg->data + w->wp, d->value.value, d->value.length);
    w->wp += d->value.length;
}

void
lksv_close_value_log_writer(lksv_value_log_writer *w)
{
    if (w->n > 0)
        flush_page(w);

    free(w);
}
