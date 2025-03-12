#include "hw/femu/kvssd/pink/pink_ftl.h"
#include "hw/femu/kvssd/pink/skiplist.h"

static void
do_gc(struct ssd *ssd)
{
    while (pink_should_data_gc_high(ssd)) {
        int n = ssd->lm.data.lines / 10;
        if (n < 10)
            n = 10;

        int gc_pick_err = 0;
        int gc_pick_err_threshold = n / 3;
        for (int i = 0; i < n; i ++) {
            switch (gc_data_femu(ssd)) {
                case 0:
                    break;
                case -2:
                    gc_pick_err++;
                    break;
                default:
                    kv_log("unknown return code\n");
                    abort();
            }
        }
        if (gc_pick_err > gc_pick_err_threshold) {
            kv_log("gc_pick_err exceeds threshold: %d times\n", gc_pick_err);
            print_level_summary(pink_lsm);
            kv_log("line_partition meta: %d lines, %d frees, %d victims, %d fulls, %ld age\n", ssd->lm.meta.lines, ssd->lm.meta.free_line_cnt, ssd->lm.meta.victim_line_cnt, ssd->lm.meta.full_line_cnt, ssd->lm.meta.age);
            kv_log("line_partition data: %d lines, %d frees, %d victims, %d fulls, %ld age\n", ssd->lm.data.lines, ssd->lm.data.free_line_cnt, ssd->lm.data.victim_line_cnt, ssd->lm.data.full_line_cnt, ssd->lm.data.age);
            abort();
        }
    }
    while (pink_should_meta_gc_high(ssd)) {
        int n = ssd->lm.meta.lines / 10;
        if (n < 2)
            n = 2;
        for (int i = 0; i < n; i ++) {
            gc_meta_femu(ssd);
        }
    }
}

static void
print_stats(void)
{
    if (rand() % 1000 == 0) {
        kv_debug("write_cnt %lu\n", pink_lsm->num_data_written);
        kv_debug("[META] free line cnt: %d\n", ssd->lm.meta.free_line_cnt);
        kv_debug("[META] full line cnt: %d\n", ssd->lm.meta.full_line_cnt);
        kv_debug("[META] victim line cnt: %d\n", ssd->lm.meta.victim_line_cnt);
        kv_debug("[DATA] free line cnt: %d\n", ssd->lm.data.free_line_cnt);
        kv_debug("[DATA] full line cnt: %d\n", ssd->lm.data.full_line_cnt);
        kv_debug("[DATA] victim line cnt: %d\n", ssd->lm.data.victim_line_cnt);
        print_level_summary(pink_lsm);
    }
}

static void compaction_selector(struct ssd *ssd, pink_level *a, pink_level *b, leveling_node *lnode){
    leveling(ssd, a, b, lnode);

    if (b->idx == LSM_LEVELN - 1)
        pink_lsm_adjust_level_multiplier();
    if (b->idx > 0) // We don't want too many calling adjust_lines().
        pink_adjust_lines(ssd);
}

static int
compact_memtable(void)
{
    print_stats();

    if (pink_lsm->imm)
    {
        compaction_data_write(pink_lsm->ssd, pink_lsm->imm);

        kv_skiplist_put(pink_lsm->imm);
        pink_lsm->imm = NULL;
    }

    if (pink_lsm->key_only_imm)
    {
        leveling_node lnode;
        kv_skiplist *tmp = pink_skiplist_cutting_header(pink_lsm->key_only_imm);
        if (tmp == pink_lsm->key_only_imm)
            pink_lsm->key_only_imm = NULL;

        kv_skiplist_get_start_end_key(tmp, &lnode.start, &lnode.end);
        lnode.mem = tmp;
        compaction_selector(pink_lsm->ssd, NULL, pink_lsm->disk[0], &lnode);

        FREE(lnode.start.key);
        FREE(lnode.end.key);

        kv_skiplist_put(tmp);
    }
    else if (kv_skiplist_approximate_memory_usage(pink_lsm->key_only_mem) >= KEY_ONLY_WRITE_BUFFER_SIZE)
    {
        pink_lsm->key_only_imm = pink_lsm->key_only_mem;
        pink_lsm->key_only_mem = kv_skiplist_init();
    }

    do_gc(pink_lsm->ssd);

    return 0;
}

static int
compact_disk_tables(void)
{
    compaction_selector(pink_lsm->ssd,
                        pink_lsm->disk[pink_lsm->compaction_level],
                        pink_lsm->disk[pink_lsm->compaction_level+1],
                        NULL);

    return 0;
}

static int
compact1(void)
{
    if (pink_lsm->imm || pink_lsm->key_only_imm)
    {
        compact_memtable();

        // TODO: move this function into the version update function.
        update_compaction_score();
    }

    if (pink_lsm->compaction_score >= 1)
    {
        compact_disk_tables();

        // TODO: move this function into the version update function.
        update_compaction_score();
    }

    return 0;
}

static void *
compact(void *arg)
{
    while (true)
    {
        if (qatomic_read(&pink_lsm->compaction_calls) > 0)
        {
            qatomic_dec(&pink_lsm->compaction_calls);

            qemu_mutex_lock(&pink_lsm->mu);

            if (compact1() != 0)
            {
                // TODO: handling error.
            }

            pink_lsm->compacting = false;

            maybe_schedule_compaction();

            qemu_mutex_unlock(&pink_lsm->mu);
        }
    }

    return NULL;
}

void
compaction_init(void)
{
    qemu_thread_create(&pink_lsm->comp_thread, "FEMU-COMP-Thread",
                       compact, NULL, QEMU_THREAD_JOINABLE);
}

void
maybe_schedule_compaction(void)
{
    if (pink_lsm->compacting)
        return;

    if (!pink_lsm->imm && !pink_lsm->key_only_imm)
    {
        if (pink_lsm->compaction_score < 1)
            return;
    }

    pink_lsm->compacting = true;

    qatomic_inc(&pink_lsm->compaction_calls);
}

uint32_t level_change(struct pink_lsmtree *LSM, pink_level *from, pink_level *to, pink_level *target) {
    pink_level **src_ptr=NULL, **des_ptr=NULL;
    des_ptr=&LSM->disk[to->idx];

    if(from!=NULL){
        src_ptr=&LSM->disk[from->idx];
        *(src_ptr)=level_init(from->idx);
        free_level(LSM, from);
    }

    (*des_ptr)=target;
    free_level(LSM, to);

    return 1;
}

uint32_t leveling(struct ssd *ssd, pink_level *from, pink_level *to, leveling_node *l_node){
    pink_level *target_origin = to;
    pink_level *target = level_init(to->idx);
    pink_level_list_entry *entry = NULL;

    // TODO: LEVEL_COMP_READ_DELAY
    read_run_delay_comp(ssd, to);
    partial_leveling(ssd, target,target_origin,l_node,from);

    if (entry) FREE(entry);
    uint32_t res = level_change(pink_lsm, from, to, target);
    pink_lsm->c_level = NULL;

    if(target->idx == LSM_LEVELN-1){
        kv_debug("last level %d/%d (n:f)\n",target->n_num,target->m_num);
    }
    return res;
}

uint32_t partial_leveling(struct ssd *ssd, pink_level* t, pink_level *origin, leveling_node *lnode, pink_level* upper){
    kv_key start=kv_key_min;
    kv_key end=kv_key_max;
    pink_level_list_entry **target_s=NULL;
    pink_level_list_entry **data=NULL;
    kv_skiplist *skip=lnode?lnode->mem:kv_skiplist_init();

    if(!upper){
        range_find_compaction(origin,start,end,&target_s);

        for(int j=0; target_s[j]!=NULL; j++){
            compaction_meta_segment_read_femu(ssd, target_s[j]);
        }

        compaction_subprocessing(ssd, skip,NULL,target_s,t);
    } else {
        int src_num, des_num; //for stream compaction
        des_num=range_find_compaction(origin,start,end,&target_s);//for stream compaction
        src_num=range_find_compaction(upper,start,end,&data);

        for(int i=0; i < des_num; i++){
            kv_assert(target_s[i]);
            pink_level_list_entry *temp=target_s[i];
            compaction_meta_segment_read_femu(ssd, temp);
        }

        for(int i=0; i < src_num; i++){
            kv_assert(data[i]);
            pink_level_list_entry *temp=data[i];
            compaction_meta_segment_read_femu(ssd, temp);
        }
        //wait_delay(ssd, true);
        compaction_subprocessing(ssd, NULL,data,target_s,t);
    }
    if(!lnode) kv_skiplist_put(skip);
    return 1;
}

