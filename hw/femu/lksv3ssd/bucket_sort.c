#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

static int
compare_hash(const void *a, const void *b)
{
    if ((*(lksv_kv_descriptor **) a)->hash > (*(lksv_kv_descriptor **) b)->hash)
        return 1;
    else if ((*(lksv_kv_descriptor **) a)->hash < (*(lksv_kv_descriptor **) b)->hash)
        return -1;
    else
        return 0;
}


static int
round_up_pow2(int n)
{
    n -= 1;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

void
lksv_bucket_sort(lksv_kv_descriptor **buffer, int size)
{
    int bucket_size = round_up_pow2(size);
    lksv_kv_descriptor **bucket = calloc(bucket_size, sizeof(lksv_kv_descriptor *));
    lksv_kv_descriptor **collision_bucket = calloc(size, sizeof(lksv_kv_descriptor *));
    int collision_size = 0;
    int pow_n = 0;
    int n = 1;

    if (size <= 1)
        return;

    while (n < bucket_size)
    {
        n = n << 1;
        pow_n++;
    }

    uint32_t mask = 32 - pow_n;

    for (int i = 0; i < size; i++)
    {
        int idx = buffer[i]->hash >> mask;
        
        if (bucket[idx] == NULL)
            bucket[idx] = buffer[i];
        else
        {
            collision_bucket[collision_size] = buffer[i];
            collision_size++;
        }
    }

    qsort(collision_bucket, collision_size, sizeof(lksv_kv_descriptor *), compare_hash);

    int i, j, k;
    i = j = k = 0;
    while (i < size)
    {
        lksv_kv_descriptor *a = NULL;
        lksv_kv_descriptor *b = NULL;

        while (j < bucket_size)
        {
            if (bucket[j])
            {
                a = bucket[j];
                break;
            }
            j++;
        }

        if (k < collision_size)
            b = collision_bucket[k];

        int res;
        if (a && b)
            res = a->hash < b->hash ? -1 : +1;
        else if (a)
            res = -1;
        else
            res = +1;

        if (res < 0)
        {
            buffer[i] = a;
            j++;
        }
        else
        {
            buffer[i] = b;
            k++;
        }

        i++;
    }

    free(bucket);
    free(collision_bucket);
}
