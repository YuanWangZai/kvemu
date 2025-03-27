#include "hw/femu/kvssd/pink/pink_ftl.h"

static void
next_page(pink_data_seg_writer *w)
{
    w->writing_ppa = get_new_data_page();
    w->left = PAGESIZE - (4 * sizeof(uint16_t));
    w->n = 0;
}

static void
flush_page(pink_data_seg_writer *w)
{
    struct nand_page *pg = get_pg(&w->writing_ppa);
    char *ptr = pg->data;
    uint16_t *offset_map = (uint16_t *)ptr;
    uint16_t *keylen_map = (uint16_t *)(ptr + ((w->n+2) * sizeof(uint16_t)));
    uint16_t data_start = 2 * (w->n+2) * sizeof(uint16_t);

    offset_map[0] = w->n;
    keylen_map[0] = w->n;

    for (int i = 0; i < w->n; i++)
    {
        memcpy(&ptr[data_start], w->buffer[i]->key.key, w->buffer[i]->key.len);
        memcpy(&ptr[data_start+w->buffer[i]->key.len], w->buffer[i]->value.value, w->buffer[i]->value.length);
        offset_map[i+1] = data_start;
        keylen_map[i+1] = w->buffer[i]->key.len;
        data_start += keylen_map[i+1] + w->buffer[i]->value.length;
    }

    offset_map[w->n+1] = data_start;
    keylen_map[w->n+1] = -1;

    struct line *line = get_line(&w->writing_ppa);

    line->vsc += w->n;
    mark_page_valid(&w->writing_ppa);
    ssd_advance_write_pointer(&pink_ssd->lm.data);
    pink_comp_write_delay(&w->writing_ppa);
}

pink_data_seg_writer *
pink_new_data_seg_writer(void)
{
    pink_data_seg_writer *w = calloc(1, sizeof(pink_data_seg_writer));

    next_page(w);

    return w;
}

void
pink_set_data_seg_writer(pink_data_seg_writer *w, pink_kv_descriptor *d)
{
    int write_size = d->key.len +
                     d->value.length +
                     (2 * sizeof(uint16_t));

    if (w->left < write_size || w->n == 8192)
    {
        flush_page(w);
        next_page(w);
    }
    w->left -= write_size;

    d->ppa = w->writing_ppa;
    d->data_seg_offset.g.in_page_idx = w->n;

    w->buffer[w->n] = d;
    w->n++;
}

void
pink_close_data_seg_writer(pink_data_seg_writer *w)
{
    if (w->n > 0)
        flush_page(w);

    free(w);
}

