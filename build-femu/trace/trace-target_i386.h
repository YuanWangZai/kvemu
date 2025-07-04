/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_TARGET_I386_GENERATED_TRACERS_H
#define TRACE_TARGET_I386_GENERATED_TRACERS_H

#include "trace/control.h"

extern TraceEvent _TRACE_KVM_SEV_INIT_EVENT;
extern TraceEvent _TRACE_KVM_MEMCRYPT_REGISTER_REGION_EVENT;
extern TraceEvent _TRACE_KVM_MEMCRYPT_UNREGISTER_REGION_EVENT;
extern TraceEvent _TRACE_KVM_SEV_CHANGE_STATE_EVENT;
extern TraceEvent _TRACE_KVM_SEV_LAUNCH_START_EVENT;
extern TraceEvent _TRACE_KVM_SEV_LAUNCH_UPDATE_DATA_EVENT;
extern TraceEvent _TRACE_KVM_SEV_LAUNCH_MEASUREMENT_EVENT;
extern TraceEvent _TRACE_KVM_SEV_LAUNCH_FINISH_EVENT;
extern TraceEvent _TRACE_KVM_SEV_LAUNCH_SECRET_EVENT;
extern TraceEvent _TRACE_KVM_SEV_ATTESTATION_REPORT_EVENT;
extern uint16_t _TRACE_KVM_SEV_INIT_DSTATE;
extern uint16_t _TRACE_KVM_MEMCRYPT_REGISTER_REGION_DSTATE;
extern uint16_t _TRACE_KVM_MEMCRYPT_UNREGISTER_REGION_DSTATE;
extern uint16_t _TRACE_KVM_SEV_CHANGE_STATE_DSTATE;
extern uint16_t _TRACE_KVM_SEV_LAUNCH_START_DSTATE;
extern uint16_t _TRACE_KVM_SEV_LAUNCH_UPDATE_DATA_DSTATE;
extern uint16_t _TRACE_KVM_SEV_LAUNCH_MEASUREMENT_DSTATE;
extern uint16_t _TRACE_KVM_SEV_LAUNCH_FINISH_DSTATE;
extern uint16_t _TRACE_KVM_SEV_LAUNCH_SECRET_DSTATE;
extern uint16_t _TRACE_KVM_SEV_ATTESTATION_REPORT_DSTATE;
#define TRACE_KVM_SEV_INIT_ENABLED 1
#define TRACE_KVM_MEMCRYPT_REGISTER_REGION_ENABLED 1
#define TRACE_KVM_MEMCRYPT_UNREGISTER_REGION_ENABLED 1
#define TRACE_KVM_SEV_CHANGE_STATE_ENABLED 1
#define TRACE_KVM_SEV_LAUNCH_START_ENABLED 1
#define TRACE_KVM_SEV_LAUNCH_UPDATE_DATA_ENABLED 1
#define TRACE_KVM_SEV_LAUNCH_MEASUREMENT_ENABLED 1
#define TRACE_KVM_SEV_LAUNCH_FINISH_ENABLED 1
#define TRACE_KVM_SEV_LAUNCH_SECRET_ENABLED 1
#define TRACE_KVM_SEV_ATTESTATION_REPORT_ENABLED 1
#include "qemu/log-for-trace.h"
#include "qemu/error-report.h"


#define TRACE_KVM_SEV_INIT_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_INIT) || \
    false)

static inline void _nocheck__trace_kvm_sev_init(void)
{
    if (trace_event_get_state(TRACE_KVM_SEV_INIT) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 4 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_init " "" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     );
#line 58 "trace/trace-target_i386.h"
        } else {
#line 4 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_init " "" "\n");
#line 62 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_init(void)
{
    if (true) {
        _nocheck__trace_kvm_sev_init();
    }
}

#define TRACE_KVM_MEMCRYPT_REGISTER_REGION_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_MEMCRYPT_REGISTER_REGION) || \
    false)

static inline void _nocheck__trace_kvm_memcrypt_register_region(void * addr, size_t len)
{
    if (trace_event_get_state(TRACE_KVM_MEMCRYPT_REGISTER_REGION) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 5 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_memcrypt_register_region " "addr %p len 0x%zx" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, len);
#line 89 "trace/trace-target_i386.h"
        } else {
#line 5 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_memcrypt_register_region " "addr %p len 0x%zx" "\n", addr, len);
#line 93 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_memcrypt_register_region(void * addr, size_t len)
{
    if (true) {
        _nocheck__trace_kvm_memcrypt_register_region(addr, len);
    }
}

#define TRACE_KVM_MEMCRYPT_UNREGISTER_REGION_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_MEMCRYPT_UNREGISTER_REGION) || \
    false)

static inline void _nocheck__trace_kvm_memcrypt_unregister_region(void * addr, size_t len)
{
    if (trace_event_get_state(TRACE_KVM_MEMCRYPT_UNREGISTER_REGION) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 6 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_memcrypt_unregister_region " "addr %p len 0x%zx" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, len);
#line 120 "trace/trace-target_i386.h"
        } else {
#line 6 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_memcrypt_unregister_region " "addr %p len 0x%zx" "\n", addr, len);
#line 124 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_memcrypt_unregister_region(void * addr, size_t len)
{
    if (true) {
        _nocheck__trace_kvm_memcrypt_unregister_region(addr, len);
    }
}

#define TRACE_KVM_SEV_CHANGE_STATE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_CHANGE_STATE) || \
    false)

static inline void _nocheck__trace_kvm_sev_change_state(const char * old, const char * new)
{
    if (trace_event_get_state(TRACE_KVM_SEV_CHANGE_STATE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 7 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_change_state " "%s -> %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , old, new);
#line 151 "trace/trace-target_i386.h"
        } else {
#line 7 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_change_state " "%s -> %s" "\n", old, new);
#line 155 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_change_state(const char * old, const char * new)
{
    if (true) {
        _nocheck__trace_kvm_sev_change_state(old, new);
    }
}

#define TRACE_KVM_SEV_LAUNCH_START_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_LAUNCH_START) || \
    false)

static inline void _nocheck__trace_kvm_sev_launch_start(int policy, void * session, void * pdh)
{
    if (trace_event_get_state(TRACE_KVM_SEV_LAUNCH_START) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 8 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_launch_start " "policy 0x%x session %p pdh %p" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , policy, session, pdh);
#line 182 "trace/trace-target_i386.h"
        } else {
#line 8 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_launch_start " "policy 0x%x session %p pdh %p" "\n", policy, session, pdh);
#line 186 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_launch_start(int policy, void * session, void * pdh)
{
    if (true) {
        _nocheck__trace_kvm_sev_launch_start(policy, session, pdh);
    }
}

#define TRACE_KVM_SEV_LAUNCH_UPDATE_DATA_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_LAUNCH_UPDATE_DATA) || \
    false)

static inline void _nocheck__trace_kvm_sev_launch_update_data(void * addr, uint64_t len)
{
    if (trace_event_get_state(TRACE_KVM_SEV_LAUNCH_UPDATE_DATA) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 9 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_launch_update_data " "addr %p len 0x%" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, len);
#line 213 "trace/trace-target_i386.h"
        } else {
#line 9 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_launch_update_data " "addr %p len 0x%" PRIx64 "\n", addr, len);
#line 217 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_launch_update_data(void * addr, uint64_t len)
{
    if (true) {
        _nocheck__trace_kvm_sev_launch_update_data(addr, len);
    }
}

#define TRACE_KVM_SEV_LAUNCH_MEASUREMENT_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_LAUNCH_MEASUREMENT) || \
    false)

static inline void _nocheck__trace_kvm_sev_launch_measurement(const char * value)
{
    if (trace_event_get_state(TRACE_KVM_SEV_LAUNCH_MEASUREMENT) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 10 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_launch_measurement " "data %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , value);
#line 244 "trace/trace-target_i386.h"
        } else {
#line 10 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_launch_measurement " "data %s" "\n", value);
#line 248 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_launch_measurement(const char * value)
{
    if (true) {
        _nocheck__trace_kvm_sev_launch_measurement(value);
    }
}

#define TRACE_KVM_SEV_LAUNCH_FINISH_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_LAUNCH_FINISH) || \
    false)

static inline void _nocheck__trace_kvm_sev_launch_finish(void)
{
    if (trace_event_get_state(TRACE_KVM_SEV_LAUNCH_FINISH) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 11 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_launch_finish " "" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     );
#line 275 "trace/trace-target_i386.h"
        } else {
#line 11 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_launch_finish " "" "\n");
#line 279 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_launch_finish(void)
{
    if (true) {
        _nocheck__trace_kvm_sev_launch_finish();
    }
}

#define TRACE_KVM_SEV_LAUNCH_SECRET_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_LAUNCH_SECRET) || \
    false)

static inline void _nocheck__trace_kvm_sev_launch_secret(uint64_t hpa, uint64_t hva, uint64_t secret, int len)
{
    if (trace_event_get_state(TRACE_KVM_SEV_LAUNCH_SECRET) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 12 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_launch_secret " "hpa 0x%" PRIx64 " hva 0x%" PRIx64 " data 0x%" PRIx64 " len %d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , hpa, hva, secret, len);
#line 306 "trace/trace-target_i386.h"
        } else {
#line 12 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_launch_secret " "hpa 0x%" PRIx64 " hva 0x%" PRIx64 " data 0x%" PRIx64 " len %d" "\n", hpa, hva, secret, len);
#line 310 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_launch_secret(uint64_t hpa, uint64_t hva, uint64_t secret, int len)
{
    if (true) {
        _nocheck__trace_kvm_sev_launch_secret(hpa, hva, secret, len);
    }
}

#define TRACE_KVM_SEV_ATTESTATION_REPORT_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_KVM_SEV_ATTESTATION_REPORT) || \
    false)

static inline void _nocheck__trace_kvm_sev_attestation_report(const char * mnonce, const char * data)
{
    if (trace_event_get_state(TRACE_KVM_SEV_ATTESTATION_REPORT) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 13 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("%d@%zu.%06zu:kvm_sev_attestation_report " "mnonce %s data %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , mnonce, data);
#line 337 "trace/trace-target_i386.h"
        } else {
#line 13 "/opt/work/kvemu/target/i386/trace-events"
            qemu_log("kvm_sev_attestation_report " "mnonce %s data %s" "\n", mnonce, data);
#line 341 "trace/trace-target_i386.h"
        }
    }
}

static inline void trace_kvm_sev_attestation_report(const char * mnonce, const char * data)
{
    if (true) {
        _nocheck__trace_kvm_sev_attestation_report(mnonce, data);
    }
}
#endif /* TRACE_TARGET_I386_GENERATED_TRACERS_H */
