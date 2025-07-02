#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

const struct kv_lsm_operations lksv_lsm_operations = {
    .open = lksv_open,
};
