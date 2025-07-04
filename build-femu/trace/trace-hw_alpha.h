/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_HW_ALPHA_GENERATED_TRACERS_H
#define TRACE_HW_ALPHA_GENERATED_TRACERS_H

#include "trace/control.h"

extern TraceEvent _TRACE_ALPHA_PCI_IACK_WRITE_EVENT;
extern uint16_t _TRACE_ALPHA_PCI_IACK_WRITE_DSTATE;
#define TRACE_ALPHA_PCI_IACK_WRITE_ENABLED 1
#include "qemu/log-for-trace.h"
#include "qemu/error-report.h"


#define TRACE_ALPHA_PCI_IACK_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_ALPHA_PCI_IACK_WRITE) || \
    false)

static inline void _nocheck__trace_alpha_pci_iack_write(void)
{
    if (trace_event_get_state(TRACE_ALPHA_PCI_IACK_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 4 "/opt/work/kvemu/hw/alpha/trace-events"
            qemu_log("%d@%zu.%06zu:alpha_pci_iack_write " "" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     );
#line 31 "trace/trace-hw_alpha.h"
        } else {
#line 4 "/opt/work/kvemu/hw/alpha/trace-events"
            qemu_log("alpha_pci_iack_write " "" "\n");
#line 35 "trace/trace-hw_alpha.h"
        }
    }
}

static inline void trace_alpha_pci_iack_write(void)
{
    if (true) {
        _nocheck__trace_alpha_pci_iack_write();
    }
}
#endif /* TRACE_HW_ALPHA_GENERATED_TRACERS_H */
