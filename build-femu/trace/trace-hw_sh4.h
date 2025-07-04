/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_HW_SH4_GENERATED_TRACERS_H
#define TRACE_HW_SH4_GENERATED_TRACERS_H

#include "trace/control.h"

extern TraceEvent _TRACE_SH7750_PORTA_EVENT;
extern TraceEvent _TRACE_SH7750_PORTB_EVENT;
extern uint16_t _TRACE_SH7750_PORTA_DSTATE;
extern uint16_t _TRACE_SH7750_PORTB_DSTATE;
#define TRACE_SH7750_PORTA_ENABLED 1
#define TRACE_SH7750_PORTB_ENABLED 1
#include "qemu/log-for-trace.h"
#include "qemu/error-report.h"


#define TRACE_SH7750_PORTA_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_SH7750_PORTA) || \
    false)

static inline void _nocheck__trace_sh7750_porta(uint16_t prev, uint16_t cur, uint16_t pdtr, uint16_t pctr)
{
    if (trace_event_get_state(TRACE_SH7750_PORTA) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 2 "/opt/work/kvemu/hw/sh4/trace-events"
            qemu_log("%d@%zu.%06zu:sh7750_porta " "porta changed from 0x%04x to 0x%04x\npdtra=0x%04x, pctra=0x%08x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , prev, cur, pdtr, pctr);
#line 34 "trace/trace-hw_sh4.h"
        } else {
#line 2 "/opt/work/kvemu/hw/sh4/trace-events"
            qemu_log("sh7750_porta " "porta changed from 0x%04x to 0x%04x\npdtra=0x%04x, pctra=0x%08x" "\n", prev, cur, pdtr, pctr);
#line 38 "trace/trace-hw_sh4.h"
        }
    }
}

static inline void trace_sh7750_porta(uint16_t prev, uint16_t cur, uint16_t pdtr, uint16_t pctr)
{
    if (true) {
        _nocheck__trace_sh7750_porta(prev, cur, pdtr, pctr);
    }
}

#define TRACE_SH7750_PORTB_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_SH7750_PORTB) || \
    false)

static inline void _nocheck__trace_sh7750_portb(uint16_t prev, uint16_t cur, uint16_t pdtr, uint16_t pctr)
{
    if (trace_event_get_state(TRACE_SH7750_PORTB) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 3 "/opt/work/kvemu/hw/sh4/trace-events"
            qemu_log("%d@%zu.%06zu:sh7750_portb " "portb changed from 0x%04x to 0x%04x\npdtrb=0x%04x, pctrb=0x%08x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , prev, cur, pdtr, pctr);
#line 65 "trace/trace-hw_sh4.h"
        } else {
#line 3 "/opt/work/kvemu/hw/sh4/trace-events"
            qemu_log("sh7750_portb " "portb changed from 0x%04x to 0x%04x\npdtrb=0x%04x, pctrb=0x%08x" "\n", prev, cur, pdtr, pctr);
#line 69 "trace/trace-hw_sh4.h"
        }
    }
}

static inline void trace_sh7750_portb(uint16_t prev, uint16_t cur, uint16_t pdtr, uint16_t pctr)
{
    if (true) {
        _nocheck__trace_sh7750_portb(prev, cur, pdtr, pctr);
    }
}
#endif /* TRACE_HW_SH4_GENERATED_TRACERS_H */
