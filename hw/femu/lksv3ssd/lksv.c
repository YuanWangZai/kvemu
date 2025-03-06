#include "hw/femu/nvme.h"
#include "hw/femu/kvssd/lksv/lksv3_ftl.h"

static void lksv3_init_ctrl_str(FemuCtrl *n)
{
    static int fsid_vlksv3 = 0;
    const char *vlksv3ssd_mn = "FEMU KeyValue-SSD Controller";
    const char *vlksv3ssd_sn = "vLKSV3SSD";

    nvme_set_ctrl_name(n, vlksv3ssd_mn, vlksv3ssd_sn, &fsid_vlksv3);
}

/* kv <=> key-value */
static void lksv3_init(FemuCtrl *n, Error **errp)
{
    struct ssd *ssd = n->ssd = g_malloc0(sizeof(struct ssd));
    kvssd_init_latency(&ssd->lat, TLC);

    lksv3_init_ctrl_str(n);

    ssd->dataplane_started_ptr = &n->dataplane_started;
    ssd->ssdname = (char *)n->devname;
    kv_debug("Starting FEMU in LKSV3-SSD mode ...\n");
    lksv3ssd_init(n);
    n->current_handle = 0;
    n->iterate_idx = 0;
    ssd->n = n;
}

static void lksv3_flip(FemuCtrl *n, NvmeCmd *cmd)
{
    struct ssd *ssd = n->ssd;
    int64_t cdw10 = le64_to_cpu(cmd->cdw10);

    switch (cdw10) {
    case FEMU_ENABLE_GC_DELAY:
        ssd->sp.enable_gc_delay = true;
        kv_log("%s,FEMU GC Delay Emulation [Enabled]!\n", n->devname);
        break;
    case FEMU_DISABLE_GC_DELAY:
        ssd->sp.enable_gc_delay = false;
        kv_log("%s,FEMU GC Delay Emulation [Disabled]!\n", n->devname);
        break;
    case FEMU_ENABLE_DELAY_EMU:
        kv_log("%s,FEMU Delay Emulation [Enabled]!\n", n->devname);
        break;
    case FEMU_DISABLE_DELAY_EMU:
        kv_log("%s,FEMU Delay Emulation [Disabled]!\n", n->devname);
        break;
    case FEMU_RESET_ACCT:
        n->nr_tt_ios = 0;
        n->nr_tt_late_ios = 0;
        kv_log("%s,Reset tt_late_ios/tt_ios,%lu/%lu\n", n->devname,
                n->nr_tt_late_ios, n->nr_tt_ios);
        break;
    case FEMU_ENABLE_LOG:
        n->print_log = true;
        kv_log("%s,Log print [Enabled]!\n", n->devname);
        break;
    case FEMU_DISABLE_LOG:
        n->print_log = false;
        kv_log("%s,Log print [Disabled]!\n", n->devname);
        break;
    default:
        printf("FEMU:%s,Not implemented flip cmd (%lu)\n", n->devname, cdw10);
    }
}

static uint16_t lksv3_nvme_rw(FemuCtrl *n, NvmeNamespace *ns, NvmeCmd *cmd,
                           NvmeRequest *req)
{
    return nvme_rw(n, ns, cmd, req);
}

static uint16_t lksv3_io_cmd(FemuCtrl *n, NvmeNamespace *ns, NvmeCmd *cmd,
                          NvmeRequest *req)
{
    /*kv_debug("cmd: 0x%x, mptr: 0x%lx, prp1: 0x%lx, prp2: 0x%lx\n", cmd->opcode,
      le64_to_cpu(cmd->mptr), le64_to_cpu(cmd->dptr.prp1),
      le64_to_cpu(cmd->dptr.prp2));
      kv_debug(
      "cdw10: 0x%x, cdw11: 0x%x, cdw12: 0x%x, cdw13: 0x%x, cdw14: 0x%x, cdw15: "
      "0x%x\n",
      le32_to_cpu(cmd->cdw10), le32_to_cpu(cmd->cdw11), le32_to_cpu(cmd->cdw12),
      le32_to_cpu(cmd->cdw13), le32_to_cpu(cmd->cdw14),
      le32_to_cpu(cmd->cdw15));*/
    uint32_t value_length;
    uint32_t key_length;
    uint8_t *value;
    uint64_t prp1, prp2;
    uint64_t key_prp1, key_prp2;
    uint16_t status;
    switch (cmd->opcode) {
        case NVME_CMD_READ:
        case NVME_CMD_WRITE:
            return lksv3_nvme_rw(n, ns, cmd, req);
        case NVME_CMD_KV_STORE:
            value_length = le32_to_cpu(cmd->cdw10) * 4;
            key_length = (le32_to_cpu(cmd->cdw11) & 0xFF) + 1;
            value = g_malloc0(value_length);
            prp1 = le64_to_cpu(cmd->dptr.prp1);
            prp2 = le64_to_cpu(cmd->dptr.prp2);
            key_prp1 =
                le32_to_cpu(cmd->cdw12) + ((uint64_t)le32_to_cpu(cmd->cdw13) << 32);
            key_prp2 =
                le32_to_cpu(cmd->cdw14) + ((uint64_t)le32_to_cpu(cmd->cdw15) << 32);
            status = NVME_SUCCESS;

            if (key_length <= 16) {
                *((uint32_t *)req->key_buf) = le32_to_cpu(cmd->cdw12);
                *((uint32_t *)(req->key_buf + 4)) = le32_to_cpu(cmd->cdw13);
                *((uint32_t *)(req->key_buf + 8)) = le32_to_cpu(cmd->cdw14);
                *((uint32_t *)(req->key_buf + 12)) = le32_to_cpu(cmd->cdw15);
            } else {
                /*
                kv_debug("key_length: 0x%x, key_prp1: 0x%lx, key_prp2: 0x%lx\n",
                        key_length, key_prp1, key_prp2);
                        */
                status = dma_write_prp(n, req->key_buf, key_length, key_prp1, key_prp2);
                if (status != NVME_SUCCESS) {
                    return status;  // XXX free the resources
                }
            }
#ifdef HASH_COLLISION_MODELING
            memcpy(req->key_buf+4, req->key_buf, 4);
            memcpy(req->key_buf+8, req->key_buf, 4);
            memcpy(req->key_buf+2, req->key_buf+7, 4);
#endif

            /*
            kv_debug("keylen: %d: ", key_length);
            for (uint32_t i = 0; i < key_length; ++i) {
                kv_debug("%x", key[i]);
            }
            kv_debug("\n");
            */

            status = dma_write_prp(n, value, value_length, prp1, prp2);
            if (status != NVME_SUCCESS) {
                return status;  // XXX free the resources
            }
            /*for (uint32_t i = 0; i < key_length; ++i) {
              kv_debug("%x", key[i]);
              }
              kv_debug("\n");*/
            req->key_length = key_length;
            req->key_buf[key_length+1] = 0;
            req->value_length = value_length;
            req->value = value;
            return NVME_SUCCESS;
        case NVME_CMD_KV_RETRIEVE:
            value_length = le32_to_cpu(cmd->cdw10) * 4;
            key_length = (le32_to_cpu(cmd->cdw11) & 0xFF) + 1;
            //value = g_malloc0(value_length);
            prp1 = le64_to_cpu(cmd->dptr.prp1);
            prp2 = le64_to_cpu(cmd->dptr.prp2);
            key_prp1 =
                le32_to_cpu(cmd->cdw12) + ((uint64_t)le32_to_cpu(cmd->cdw13) << 32);
            key_prp2 =
                le32_to_cpu(cmd->cdw14) + ((uint64_t)le32_to_cpu(cmd->cdw15) << 32);
            status = NVME_SUCCESS;

            if (key_length <= 16) {
                *((uint32_t *)req->key_buf) = le32_to_cpu(cmd->cdw12);
                *((uint32_t *)(req->key_buf + 4)) = le32_to_cpu(cmd->cdw13);
                *((uint32_t *)(req->key_buf + 8)) = le32_to_cpu(cmd->cdw14);
                *((uint32_t *)(req->key_buf + 12)) = le32_to_cpu(cmd->cdw15);
            } else {
                /*
                kv_debug("key_length: 0x%x, key_prp1: 0x%lx, key_prp2: 0x%lx\n",
                        key_length, key_prp1, key_prp2);
                        */
                status = dma_write_prp(n, req->key_buf, key_length, key_prp1, key_prp2);
                if (status != NVME_SUCCESS) {
                    return status;  // XXX free the resources
                }
            }

#ifdef HASH_COLLISION_MODELING
            memcpy(req->key_buf+4, req->key_buf, 4);
            memcpy(req->key_buf+8, req->key_buf, 4);
            memcpy(req->key_buf+2, req->key_buf+7, 4);
#endif
            /*
            kv_debug("keylen: %d: ", key_length);
            for (uint32_t i = 0; i < key_length; ++i) {
                kv_debug("%x", key[i]);
            }
            kv_debug("\n");
            */
            req->key_length = key_length;
            req->key_buf[key_length+1] = 0;
            //FREE(value);
            return NVME_SUCCESS;
        case NVME_CMD_KV_DELETE:
            return NVME_INVALID_OPCODE | NVME_DNR;
        case NVME_CMD_KV_ITERATE_REQUEST:
            return NVME_INVALID_OPCODE | NVME_DNR;
        case NVME_CMD_KV_ITERATE_READ:
            return NVME_INVALID_OPCODE | NVME_DNR;
        case NVME_CMD_KV_DUMP:
            return NVME_SUCCESS;
        default:
            return NVME_INVALID_OPCODE | NVME_DNR;
    }
}

static uint16_t lksv3_admin_cmd(FemuCtrl *n, NvmeCmd *cmd)
{
    switch (cmd->opcode) {
    case NVME_ADM_CMD_FEMU_FLIP:
        lksv3_flip(n, cmd);
        return NVME_SUCCESS;
    default:
        return NVME_INVALID_OPCODE | NVME_DNR;
    }
}

int nvme_register_lksv3ssd_exp(FemuCtrl *n)
{
    n->ext_ops = (FemuExtCtrlOps) {
        .state            = NULL,
        .init             = lksv3_init,
        .exit             = NULL,
        .rw_check_req     = NULL,
        .admin_cmd        = lksv3_admin_cmd,
        .io_cmd           = lksv3_io_cmd,
        .get_log          = NULL,
    };

    return 0;
}

