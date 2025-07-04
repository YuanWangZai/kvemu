/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_TARGET_MIPS_TCG_GENERATED_TRACERS_H
#define TRACE_TARGET_MIPS_TCG_GENERATED_TRACERS_H

#include "trace/control.h"

extern TraceEvent _TRACE_MIPS_TRANSLATE_C0_EVENT;
extern TraceEvent _TRACE_MIPS_TRANSLATE_TR_EVENT;
extern uint16_t _TRACE_MIPS_TRANSLATE_C0_DSTATE;
extern uint16_t _TRACE_MIPS_TRANSLATE_TR_DSTATE;
#define TRACE_MIPS_TRANSLATE_C0_ENABLED 1
#define TRACE_MIPS_TRANSLATE_TR_ENABLED 1
#include "qemu/log-for-trace.h"
#include "qemu/error-report.h"


#define TRACE_MIPS_TRANSLATE_C0_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_MIPS_TRANSLATE_C0) || \
    false)

static inline void _nocheck__trace_mips_translate_c0(const char * instr, const char * rn, int reg, int sel)
{
    if (trace_event_get_state(TRACE_MIPS_TRANSLATE_C0) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 4 "/opt/work/kvemu/target/mips/tcg/trace-events"
            qemu_log("%d@%zu.%06zu:mips_translate_c0 " "%s %s (reg %d sel %d)" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , instr, rn, reg, sel);
#line 34 "trace/trace-target_mips_tcg.h"
        } else {
#line 4 "/opt/work/kvemu/target/mips/tcg/trace-events"
            qemu_log("mips_translate_c0 " "%s %s (reg %d sel %d)" "\n", instr, rn, reg, sel);
#line 38 "trace/trace-target_mips_tcg.h"
        }
    }
}

static inline void trace_mips_translate_c0(const char * instr, const char * rn, int reg, int sel)
{
    if (true) {
        _nocheck__trace_mips_translate_c0(instr, rn, reg, sel);
    }
}

#define TRACE_MIPS_TRANSLATE_TR_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_MIPS_TRANSLATE_TR) || \
    false)

static inline void _nocheck__trace_mips_translate_tr(const char * instr, int rt, int u, int sel, int h)
{
    if (trace_event_get_state(TRACE_MIPS_TRANSLATE_TR) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 5 "/opt/work/kvemu/target/mips/tcg/trace-events"
            qemu_log("%d@%zu.%06zu:mips_translate_tr " "%s (reg %d u %d sel %d h %d)" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , instr, rt, u, sel, h);
#line 65 "trace/trace-target_mips_tcg.h"
        } else {
#line 5 "/opt/work/kvemu/target/mips/tcg/trace-events"
            qemu_log("mips_translate_tr " "%s (reg %d u %d sel %d h %d)" "\n", instr, rt, u, sel, h);
#line 69 "trace/trace-target_mips_tcg.h"
        }
    }
}

static inline void trace_mips_translate_tr(const char * instr, int rt, int u, int sel, int h)
{
    if (true) {
        _nocheck__trace_mips_translate_tr(instr, rt, u, sel, h);
    }
}
#endif /* TRACE_TARGET_MIPS_TCG_GENERATED_TRACERS_H */
