#include <math.h>

#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

void
lksv_lsm_setup_params(struct ssd *ssd)
{
    int showing_pages = ssd->sp.tt_pgs * 90 / 100;
    int data_segment_groups = showing_pages / PG_N;

    lksv_lsm->opts->level_multiplier = kv_calc_level_multiplier(data_segment_groups);
}

