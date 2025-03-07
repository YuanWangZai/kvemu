#include "hw/femu/kvssd/pink/pink_ftl.h"
#include "hw/femu/kvssd/pink/skiplist.h"

kv_skiplist *pink_skiplist_cutting_header(kv_skiplist *skl) {
    const uint32_t ms_pg_n_limit = KEYBITMAP / sizeof(uint16_t) - 2;
    const uint32_t ms_pg_size_limit = PAGESIZE - KEYBITMAP - VERSIONBITMAP;

    int ms_pg_used = 0;
    int ms_key_num = 0;
    int ms_meta_size = 0;

    int cutting_num = 0;
    int cutting_key_size = 0;
    int cutting_val_size = 0;

    kv_snode *node;

    // Fastpath: If skl is fit to a single page, then return immediately.
    if (skl->key_size + (skl->n * PPA_LENGTH) < ms_pg_size_limit) {
        return skl;
    }

    for_each_sk(node, skl) {
        ms_meta_size = node->key.len + PPA_LENGTH;
        ms_pg_used += ms_meta_size;
        ms_key_num++;

        if (ms_pg_used > ms_pg_size_limit || ms_key_num > ms_pg_n_limit) {
            node = node->back;
            break;
        }

        cutting_num++;
        cutting_key_size += node->key.len;
        cutting_val_size += node->value->length;
    }
    return (node == skl->header) ? skl : kv_skiplist_divide(skl, node, cutting_num, cutting_key_size, cutting_val_size);
}

pink_l_bucket *pink_skiplist_make_length_bucket(kv_skiplist *sl)
{
    pink_l_bucket *b = (pink_l_bucket *) calloc(1, sizeof(pink_l_bucket));
    kv_snode *target;

    /*
     * Packing values into the bucket by their data length (unit of PIECE)
     */
    for_each_sk (target, sl) {
        if (target->value == NULL || target->value->length == 0)
            abort();

        int vsize = target->value->length;
        if (b->bucket[vsize] == NULL)
            b->bucket[vsize] = (kv_snode**)malloc(sizeof(kv_snode*) * (sl->n + 1));
        b->bucket[vsize][b->indices[vsize]++] = target;
    }

    return b;
}

