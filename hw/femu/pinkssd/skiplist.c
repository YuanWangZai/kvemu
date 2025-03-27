#include "hw/femu/kvssd/pink/pink_ftl.h"
#include "hw/femu/kvssd/pink/skiplist.h"

kv_snode *
pink_skiplist_insert(kv_skiplist *list, kv_key key, kv_value* value)
{
    kv_snode *t = kv_skiplist_insert(list, key, value);

    if (t->private)
        mark_sector_invalid(snode_ppa(t));

    return t;
}

