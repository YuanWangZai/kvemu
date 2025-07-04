/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_ACCEL_TCG_GENERATED_TRACERS_H
#define TRACE_ACCEL_TCG_GENERATED_TRACERS_H

#include "trace/control.h"

extern TraceEvent _TRACE_EXEC_TB_EVENT;
extern TraceEvent _TRACE_EXEC_TB_NOCACHE_EVENT;
extern TraceEvent _TRACE_EXEC_TB_EXIT_EVENT;
extern TraceEvent _TRACE_TRANSLATE_BLOCK_EVENT;
extern uint16_t _TRACE_EXEC_TB_DSTATE;
extern uint16_t _TRACE_EXEC_TB_NOCACHE_DSTATE;
extern uint16_t _TRACE_EXEC_TB_EXIT_DSTATE;
extern uint16_t _TRACE_TRANSLATE_BLOCK_DSTATE;
#define TRACE_EXEC_TB_ENABLED 1
#define TRACE_EXEC_TB_NOCACHE_ENABLED 1
#define TRACE_EXEC_TB_EXIT_ENABLED 1
#define TRACE_TRANSLATE_BLOCK_ENABLED 1
#include "qemu/log-for-trace.h"
#include "qemu/error-report.h"


#define TRACE_EXEC_TB_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_EXEC_TB) || \
    false)

static inline void _nocheck__trace_exec_tb(void * tb, uintptr_t pc)
{
    if (trace_event_get_state(TRACE_EXEC_TB) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 5 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("%d@%zu.%06zu:exec_tb " "tb:%p pc=0x%"PRIxPTR "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , tb, pc);
#line 40 "trace/trace-accel_tcg.h"
        } else {
#line 5 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("exec_tb " "tb:%p pc=0x%"PRIxPTR "\n", tb, pc);
#line 44 "trace/trace-accel_tcg.h"
        }
    }
}

static inline void trace_exec_tb(void * tb, uintptr_t pc)
{
    if (true) {
        _nocheck__trace_exec_tb(tb, pc);
    }
}

#define TRACE_EXEC_TB_NOCACHE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_EXEC_TB_NOCACHE) || \
    false)

static inline void _nocheck__trace_exec_tb_nocache(void * tb, uintptr_t pc)
{
    if (trace_event_get_state(TRACE_EXEC_TB_NOCACHE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 6 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("%d@%zu.%06zu:exec_tb_nocache " "tb:%p pc=0x%"PRIxPTR "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , tb, pc);
#line 71 "trace/trace-accel_tcg.h"
        } else {
#line 6 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("exec_tb_nocache " "tb:%p pc=0x%"PRIxPTR "\n", tb, pc);
#line 75 "trace/trace-accel_tcg.h"
        }
    }
}

static inline void trace_exec_tb_nocache(void * tb, uintptr_t pc)
{
    if (true) {
        _nocheck__trace_exec_tb_nocache(tb, pc);
    }
}

#define TRACE_EXEC_TB_EXIT_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_EXEC_TB_EXIT) || \
    false)

static inline void _nocheck__trace_exec_tb_exit(void * last_tb, unsigned int flags)
{
    if (trace_event_get_state(TRACE_EXEC_TB_EXIT) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 7 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("%d@%zu.%06zu:exec_tb_exit " "tb:%p flags=0x%x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , last_tb, flags);
#line 102 "trace/trace-accel_tcg.h"
        } else {
#line 7 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("exec_tb_exit " "tb:%p flags=0x%x" "\n", last_tb, flags);
#line 106 "trace/trace-accel_tcg.h"
        }
    }
}

static inline void trace_exec_tb_exit(void * last_tb, unsigned int flags)
{
    if (true) {
        _nocheck__trace_exec_tb_exit(last_tb, flags);
    }
}

#define TRACE_TRANSLATE_BLOCK_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_TRANSLATE_BLOCK) || \
    false)

static inline void _nocheck__trace_translate_block(void * tb, uintptr_t pc, const void * tb_code)
{
    if (trace_event_get_state(TRACE_TRANSLATE_BLOCK) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 10 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("%d@%zu.%06zu:translate_block " "tb:%p, pc:0x%"PRIxPTR", tb_code:%p" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , tb, pc, tb_code);
#line 133 "trace/trace-accel_tcg.h"
        } else {
#line 10 "/opt/work/kvemu/accel/tcg/trace-events"
            qemu_log("translate_block " "tb:%p, pc:0x%"PRIxPTR", tb_code:%p" "\n", tb, pc, tb_code);
#line 137 "trace/trace-accel_tcg.h"
        }
    }
}

static inline void trace_translate_block(void * tb, uintptr_t pc, const void * tb_code)
{
    if (true) {
        _nocheck__trace_translate_block(tb, pc, tb_code);
    }
}
#endif /* TRACE_ACCEL_TCG_GENERATED_TRACERS_H */
