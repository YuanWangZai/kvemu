#include "hw/femu/kvssd/pink/pink_ftl.h"

const struct kv_lsm_operations pink_lsm_operations = {
    .open = pink_open,
};
