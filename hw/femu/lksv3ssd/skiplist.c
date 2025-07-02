#include "hw/femu/kvssd/lksv/lksv3_ftl.h"
#include "hw/femu/kvssd/lksv/skiplist.h"

kv_snode *
lksv3_skiplist_insert(kv_skiplist *list, kv_key key, kv_value* value, bool deletef)
{
    kv_snode *t = kv_skiplist_insert(list, key, value);

    if (t->private)
    {
        lksv_ssd->lm.lines[snode_ppa(t)->g.blk].isc++;
        lksv_ssd->lm.lines[snode_ppa(t)->g.blk].vsc--;
    }

    return t;
}

