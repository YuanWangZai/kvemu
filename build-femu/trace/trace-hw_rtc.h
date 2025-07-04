/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_HW_RTC_GENERATED_TRACERS_H
#define TRACE_HW_RTC_GENERATED_TRACERS_H

#include "trace/control.h"

extern TraceEvent _TRACE_ALLWINNER_RTC_READ_EVENT;
extern TraceEvent _TRACE_ALLWINNER_RTC_WRITE_EVENT;
extern TraceEvent _TRACE_SUN4V_RTC_READ_EVENT;
extern TraceEvent _TRACE_SUN4V_RTC_WRITE_EVENT;
extern TraceEvent _TRACE_XLNX_ZYNQMP_RTC_GETTIME_EVENT;
extern TraceEvent _TRACE_PL031_IRQ_STATE_EVENT;
extern TraceEvent _TRACE_PL031_READ_EVENT;
extern TraceEvent _TRACE_PL031_WRITE_EVENT;
extern TraceEvent _TRACE_PL031_ALARM_RAISED_EVENT;
extern TraceEvent _TRACE_PL031_SET_ALARM_EVENT;
extern TraceEvent _TRACE_ASPEED_RTC_READ_EVENT;
extern TraceEvent _TRACE_ASPEED_RTC_WRITE_EVENT;
extern TraceEvent _TRACE_M48TXX_NVRAM_IO_READ_EVENT;
extern TraceEvent _TRACE_M48TXX_NVRAM_IO_WRITE_EVENT;
extern TraceEvent _TRACE_M48TXX_NVRAM_MEM_READ_EVENT;
extern TraceEvent _TRACE_M48TXX_NVRAM_MEM_WRITE_EVENT;
extern TraceEvent _TRACE_GOLDFISH_RTC_READ_EVENT;
extern TraceEvent _TRACE_GOLDFISH_RTC_WRITE_EVENT;
extern uint16_t _TRACE_ALLWINNER_RTC_READ_DSTATE;
extern uint16_t _TRACE_ALLWINNER_RTC_WRITE_DSTATE;
extern uint16_t _TRACE_SUN4V_RTC_READ_DSTATE;
extern uint16_t _TRACE_SUN4V_RTC_WRITE_DSTATE;
extern uint16_t _TRACE_XLNX_ZYNQMP_RTC_GETTIME_DSTATE;
extern uint16_t _TRACE_PL031_IRQ_STATE_DSTATE;
extern uint16_t _TRACE_PL031_READ_DSTATE;
extern uint16_t _TRACE_PL031_WRITE_DSTATE;
extern uint16_t _TRACE_PL031_ALARM_RAISED_DSTATE;
extern uint16_t _TRACE_PL031_SET_ALARM_DSTATE;
extern uint16_t _TRACE_ASPEED_RTC_READ_DSTATE;
extern uint16_t _TRACE_ASPEED_RTC_WRITE_DSTATE;
extern uint16_t _TRACE_M48TXX_NVRAM_IO_READ_DSTATE;
extern uint16_t _TRACE_M48TXX_NVRAM_IO_WRITE_DSTATE;
extern uint16_t _TRACE_M48TXX_NVRAM_MEM_READ_DSTATE;
extern uint16_t _TRACE_M48TXX_NVRAM_MEM_WRITE_DSTATE;
extern uint16_t _TRACE_GOLDFISH_RTC_READ_DSTATE;
extern uint16_t _TRACE_GOLDFISH_RTC_WRITE_DSTATE;
#define TRACE_ALLWINNER_RTC_READ_ENABLED 1
#define TRACE_ALLWINNER_RTC_WRITE_ENABLED 1
#define TRACE_SUN4V_RTC_READ_ENABLED 1
#define TRACE_SUN4V_RTC_WRITE_ENABLED 1
#define TRACE_XLNX_ZYNQMP_RTC_GETTIME_ENABLED 1
#define TRACE_PL031_IRQ_STATE_ENABLED 1
#define TRACE_PL031_READ_ENABLED 1
#define TRACE_PL031_WRITE_ENABLED 1
#define TRACE_PL031_ALARM_RAISED_ENABLED 1
#define TRACE_PL031_SET_ALARM_ENABLED 1
#define TRACE_ASPEED_RTC_READ_ENABLED 1
#define TRACE_ASPEED_RTC_WRITE_ENABLED 1
#define TRACE_M48TXX_NVRAM_IO_READ_ENABLED 1
#define TRACE_M48TXX_NVRAM_IO_WRITE_ENABLED 1
#define TRACE_M48TXX_NVRAM_MEM_READ_ENABLED 1
#define TRACE_M48TXX_NVRAM_MEM_WRITE_ENABLED 1
#define TRACE_GOLDFISH_RTC_READ_ENABLED 1
#define TRACE_GOLDFISH_RTC_WRITE_ENABLED 1
#include "qemu/log-for-trace.h"
#include "qemu/error-report.h"


#define TRACE_ALLWINNER_RTC_READ_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_ALLWINNER_RTC_READ) || \
    false)

static inline void _nocheck__trace_allwinner_rtc_read(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_ALLWINNER_RTC_READ) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 4 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:allwinner_rtc_read " "addr 0x%" PRIx64 " value 0x%" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 82 "trace/trace-hw_rtc.h"
        } else {
#line 4 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("allwinner_rtc_read " "addr 0x%" PRIx64 " value 0x%" PRIx64 "\n", addr, value);
#line 86 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_allwinner_rtc_read(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_allwinner_rtc_read(addr, value);
    }
}

#define TRACE_ALLWINNER_RTC_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_ALLWINNER_RTC_WRITE) || \
    false)

static inline void _nocheck__trace_allwinner_rtc_write(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_ALLWINNER_RTC_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 5 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:allwinner_rtc_write " "addr 0x%" PRIx64 " value 0x%" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 113 "trace/trace-hw_rtc.h"
        } else {
#line 5 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("allwinner_rtc_write " "addr 0x%" PRIx64 " value 0x%" PRIx64 "\n", addr, value);
#line 117 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_allwinner_rtc_write(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_allwinner_rtc_write(addr, value);
    }
}

#define TRACE_SUN4V_RTC_READ_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_SUN4V_RTC_READ) || \
    false)

static inline void _nocheck__trace_sun4v_rtc_read(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_SUN4V_RTC_READ) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 8 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:sun4v_rtc_read " "read: addr 0x%" PRIx64 " value 0x%" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 144 "trace/trace-hw_rtc.h"
        } else {
#line 8 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("sun4v_rtc_read " "read: addr 0x%" PRIx64 " value 0x%" PRIx64 "\n", addr, value);
#line 148 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_sun4v_rtc_read(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_sun4v_rtc_read(addr, value);
    }
}

#define TRACE_SUN4V_RTC_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_SUN4V_RTC_WRITE) || \
    false)

static inline void _nocheck__trace_sun4v_rtc_write(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_SUN4V_RTC_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 9 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:sun4v_rtc_write " "write: addr 0x%" PRIx64 " value 0x%" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 175 "trace/trace-hw_rtc.h"
        } else {
#line 9 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("sun4v_rtc_write " "write: addr 0x%" PRIx64 " value 0x%" PRIx64 "\n", addr, value);
#line 179 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_sun4v_rtc_write(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_sun4v_rtc_write(addr, value);
    }
}

#define TRACE_XLNX_ZYNQMP_RTC_GETTIME_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_XLNX_ZYNQMP_RTC_GETTIME) || \
    false)

static inline void _nocheck__trace_xlnx_zynqmp_rtc_gettime(int year, int month, int day, int hour, int min, int sec)
{
    if (trace_event_get_state(TRACE_XLNX_ZYNQMP_RTC_GETTIME) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 12 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:xlnx_zynqmp_rtc_gettime " "Get time from host: %d-%d-%d %2d:%02d:%02d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , year, month, day, hour, min, sec);
#line 206 "trace/trace-hw_rtc.h"
        } else {
#line 12 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("xlnx_zynqmp_rtc_gettime " "Get time from host: %d-%d-%d %2d:%02d:%02d" "\n", year, month, day, hour, min, sec);
#line 210 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_xlnx_zynqmp_rtc_gettime(int year, int month, int day, int hour, int min, int sec)
{
    if (true) {
        _nocheck__trace_xlnx_zynqmp_rtc_gettime(year, month, day, hour, min, sec);
    }
}

#define TRACE_PL031_IRQ_STATE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_PL031_IRQ_STATE) || \
    false)

static inline void _nocheck__trace_pl031_irq_state(int level)
{
    if (trace_event_get_state(TRACE_PL031_IRQ_STATE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 15 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:pl031_irq_state " "irq state %d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , level);
#line 237 "trace/trace-hw_rtc.h"
        } else {
#line 15 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("pl031_irq_state " "irq state %d" "\n", level);
#line 241 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_pl031_irq_state(int level)
{
    if (true) {
        _nocheck__trace_pl031_irq_state(level);
    }
}

#define TRACE_PL031_READ_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_PL031_READ) || \
    false)

static inline void _nocheck__trace_pl031_read(uint32_t addr, uint32_t value)
{
    if (trace_event_get_state(TRACE_PL031_READ) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 16 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:pl031_read " "addr 0x%08x value 0x%08x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 268 "trace/trace-hw_rtc.h"
        } else {
#line 16 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("pl031_read " "addr 0x%08x value 0x%08x" "\n", addr, value);
#line 272 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_pl031_read(uint32_t addr, uint32_t value)
{
    if (true) {
        _nocheck__trace_pl031_read(addr, value);
    }
}

#define TRACE_PL031_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_PL031_WRITE) || \
    false)

static inline void _nocheck__trace_pl031_write(uint32_t addr, uint32_t value)
{
    if (trace_event_get_state(TRACE_PL031_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 17 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:pl031_write " "addr 0x%08x value 0x%08x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 299 "trace/trace-hw_rtc.h"
        } else {
#line 17 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("pl031_write " "addr 0x%08x value 0x%08x" "\n", addr, value);
#line 303 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_pl031_write(uint32_t addr, uint32_t value)
{
    if (true) {
        _nocheck__trace_pl031_write(addr, value);
    }
}

#define TRACE_PL031_ALARM_RAISED_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_PL031_ALARM_RAISED) || \
    false)

static inline void _nocheck__trace_pl031_alarm_raised(void)
{
    if (trace_event_get_state(TRACE_PL031_ALARM_RAISED) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 18 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:pl031_alarm_raised " "alarm raised" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     );
#line 330 "trace/trace-hw_rtc.h"
        } else {
#line 18 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("pl031_alarm_raised " "alarm raised" "\n");
#line 334 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_pl031_alarm_raised(void)
{
    if (true) {
        _nocheck__trace_pl031_alarm_raised();
    }
}

#define TRACE_PL031_SET_ALARM_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_PL031_SET_ALARM) || \
    false)

static inline void _nocheck__trace_pl031_set_alarm(uint32_t ticks)
{
    if (trace_event_get_state(TRACE_PL031_SET_ALARM) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 19 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:pl031_set_alarm " "alarm set for %u ticks" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , ticks);
#line 361 "trace/trace-hw_rtc.h"
        } else {
#line 19 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("pl031_set_alarm " "alarm set for %u ticks" "\n", ticks);
#line 365 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_pl031_set_alarm(uint32_t ticks)
{
    if (true) {
        _nocheck__trace_pl031_set_alarm(ticks);
    }
}

#define TRACE_ASPEED_RTC_READ_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_ASPEED_RTC_READ) || \
    false)

static inline void _nocheck__trace_aspeed_rtc_read(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_ASPEED_RTC_READ) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 22 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:aspeed_rtc_read " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 392 "trace/trace-hw_rtc.h"
        } else {
#line 22 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("aspeed_rtc_read " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n", addr, value);
#line 396 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_aspeed_rtc_read(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_aspeed_rtc_read(addr, value);
    }
}

#define TRACE_ASPEED_RTC_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_ASPEED_RTC_WRITE) || \
    false)

static inline void _nocheck__trace_aspeed_rtc_write(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_ASPEED_RTC_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 23 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:aspeed_rtc_write " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 423 "trace/trace-hw_rtc.h"
        } else {
#line 23 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("aspeed_rtc_write " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n", addr, value);
#line 427 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_aspeed_rtc_write(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_aspeed_rtc_write(addr, value);
    }
}

#define TRACE_M48TXX_NVRAM_IO_READ_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_M48TXX_NVRAM_IO_READ) || \
    false)

static inline void _nocheck__trace_m48txx_nvram_io_read(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_M48TXX_NVRAM_IO_READ) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 26 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:m48txx_nvram_io_read " "io read addr:0x%04" PRIx64 " value:0x%02" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 454 "trace/trace-hw_rtc.h"
        } else {
#line 26 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("m48txx_nvram_io_read " "io read addr:0x%04" PRIx64 " value:0x%02" PRIx64 "\n", addr, value);
#line 458 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_m48txx_nvram_io_read(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_m48txx_nvram_io_read(addr, value);
    }
}

#define TRACE_M48TXX_NVRAM_IO_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_M48TXX_NVRAM_IO_WRITE) || \
    false)

static inline void _nocheck__trace_m48txx_nvram_io_write(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_M48TXX_NVRAM_IO_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 27 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:m48txx_nvram_io_write " "io write addr:0x%04" PRIx64 " value:0x%02" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 485 "trace/trace-hw_rtc.h"
        } else {
#line 27 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("m48txx_nvram_io_write " "io write addr:0x%04" PRIx64 " value:0x%02" PRIx64 "\n", addr, value);
#line 489 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_m48txx_nvram_io_write(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_m48txx_nvram_io_write(addr, value);
    }
}

#define TRACE_M48TXX_NVRAM_MEM_READ_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_M48TXX_NVRAM_MEM_READ) || \
    false)

static inline void _nocheck__trace_m48txx_nvram_mem_read(uint32_t addr, uint32_t value)
{
    if (trace_event_get_state(TRACE_M48TXX_NVRAM_MEM_READ) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 28 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:m48txx_nvram_mem_read " "mem read addr:0x%04x value:0x%02x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 516 "trace/trace-hw_rtc.h"
        } else {
#line 28 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("m48txx_nvram_mem_read " "mem read addr:0x%04x value:0x%02x" "\n", addr, value);
#line 520 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_m48txx_nvram_mem_read(uint32_t addr, uint32_t value)
{
    if (true) {
        _nocheck__trace_m48txx_nvram_mem_read(addr, value);
    }
}

#define TRACE_M48TXX_NVRAM_MEM_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_M48TXX_NVRAM_MEM_WRITE) || \
    false)

static inline void _nocheck__trace_m48txx_nvram_mem_write(uint32_t addr, uint32_t value)
{
    if (trace_event_get_state(TRACE_M48TXX_NVRAM_MEM_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 29 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:m48txx_nvram_mem_write " "mem write addr:0x%04x value:0x%02x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 547 "trace/trace-hw_rtc.h"
        } else {
#line 29 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("m48txx_nvram_mem_write " "mem write addr:0x%04x value:0x%02x" "\n", addr, value);
#line 551 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_m48txx_nvram_mem_write(uint32_t addr, uint32_t value)
{
    if (true) {
        _nocheck__trace_m48txx_nvram_mem_write(addr, value);
    }
}

#define TRACE_GOLDFISH_RTC_READ_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_GOLDFISH_RTC_READ) || \
    false)

static inline void _nocheck__trace_goldfish_rtc_read(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_GOLDFISH_RTC_READ) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 32 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:goldfish_rtc_read " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 578 "trace/trace-hw_rtc.h"
        } else {
#line 32 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("goldfish_rtc_read " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n", addr, value);
#line 582 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_goldfish_rtc_read(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_goldfish_rtc_read(addr, value);
    }
}

#define TRACE_GOLDFISH_RTC_WRITE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_GOLDFISH_RTC_WRITE) || \
    false)

static inline void _nocheck__trace_goldfish_rtc_write(uint64_t addr, uint64_t value)
{
    if (trace_event_get_state(TRACE_GOLDFISH_RTC_WRITE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 33 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("%d@%zu.%06zu:goldfish_rtc_write " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , addr, value);
#line 609 "trace/trace-hw_rtc.h"
        } else {
#line 33 "/opt/work/kvemu/hw/rtc/trace-events"
            qemu_log("goldfish_rtc_write " "addr 0x%02" PRIx64 " value 0x%08" PRIx64 "\n", addr, value);
#line 613 "trace/trace-hw_rtc.h"
        }
    }
}

static inline void trace_goldfish_rtc_write(uint64_t addr, uint64_t value)
{
    if (true) {
        _nocheck__trace_goldfish_rtc_write(addr, value);
    }
}
#endif /* TRACE_HW_RTC_GENERATED_TRACERS_H */
