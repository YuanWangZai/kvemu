/* This file is autogenerated by tracetool, do not edit. */

#ifndef TRACE_NET_GENERATED_TRACERS_H
#define TRACE_NET_GENERATED_TRACERS_H

#include "trace/control.h"

extern TraceEvent _TRACE_QEMU_ANNOUNCE_SELF_ITER_EVENT;
extern TraceEvent _TRACE_QEMU_ANNOUNCE_TIMER_DEL_EVENT;
extern TraceEvent _TRACE_VHOST_USER_EVENT_EVENT;
extern TraceEvent _TRACE_COLO_PROXY_MAIN_EVENT;
extern TraceEvent _TRACE_COLO_COMPARE_MAIN_EVENT;
extern TraceEvent _TRACE_COLO_COMPARE_DROP_PACKET_EVENT;
extern TraceEvent _TRACE_COLO_COMPARE_UDP_MISCOMPARE_EVENT;
extern TraceEvent _TRACE_COLO_COMPARE_ICMP_MISCOMPARE_EVENT;
extern TraceEvent _TRACE_COLO_COMPARE_IP_INFO_EVENT;
extern TraceEvent _TRACE_COLO_OLD_PACKET_CHECK_FOUND_EVENT;
extern TraceEvent _TRACE_COLO_COMPARE_TCP_INFO_EVENT;
extern TraceEvent _TRACE_COLO_FILTER_REWRITER_PKT_INFO_EVENT;
extern TraceEvent _TRACE_COLO_FILTER_REWRITER_CONN_OFFSET_EVENT;
extern uint16_t _TRACE_QEMU_ANNOUNCE_SELF_ITER_DSTATE;
extern uint16_t _TRACE_QEMU_ANNOUNCE_TIMER_DEL_DSTATE;
extern uint16_t _TRACE_VHOST_USER_EVENT_DSTATE;
extern uint16_t _TRACE_COLO_PROXY_MAIN_DSTATE;
extern uint16_t _TRACE_COLO_COMPARE_MAIN_DSTATE;
extern uint16_t _TRACE_COLO_COMPARE_DROP_PACKET_DSTATE;
extern uint16_t _TRACE_COLO_COMPARE_UDP_MISCOMPARE_DSTATE;
extern uint16_t _TRACE_COLO_COMPARE_ICMP_MISCOMPARE_DSTATE;
extern uint16_t _TRACE_COLO_COMPARE_IP_INFO_DSTATE;
extern uint16_t _TRACE_COLO_OLD_PACKET_CHECK_FOUND_DSTATE;
extern uint16_t _TRACE_COLO_COMPARE_TCP_INFO_DSTATE;
extern uint16_t _TRACE_COLO_FILTER_REWRITER_PKT_INFO_DSTATE;
extern uint16_t _TRACE_COLO_FILTER_REWRITER_CONN_OFFSET_DSTATE;
#define TRACE_QEMU_ANNOUNCE_SELF_ITER_ENABLED 1
#define TRACE_QEMU_ANNOUNCE_TIMER_DEL_ENABLED 1
#define TRACE_VHOST_USER_EVENT_ENABLED 1
#define TRACE_COLO_PROXY_MAIN_ENABLED 1
#define TRACE_COLO_COMPARE_MAIN_ENABLED 1
#define TRACE_COLO_COMPARE_DROP_PACKET_ENABLED 1
#define TRACE_COLO_COMPARE_UDP_MISCOMPARE_ENABLED 1
#define TRACE_COLO_COMPARE_ICMP_MISCOMPARE_ENABLED 1
#define TRACE_COLO_COMPARE_IP_INFO_ENABLED 1
#define TRACE_COLO_OLD_PACKET_CHECK_FOUND_ENABLED 1
#define TRACE_COLO_COMPARE_TCP_INFO_ENABLED 1
#define TRACE_COLO_FILTER_REWRITER_PKT_INFO_ENABLED 1
#define TRACE_COLO_FILTER_REWRITER_CONN_OFFSET_ENABLED 1
#include "qemu/log-for-trace.h"
#include "qemu/error-report.h"


#define TRACE_QEMU_ANNOUNCE_SELF_ITER_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_QEMU_ANNOUNCE_SELF_ITER) || \
    false)

static inline void _nocheck__trace_qemu_announce_self_iter(const char * id, const char * name, const char * mac, int skip)
{
    if (trace_event_get_state(TRACE_QEMU_ANNOUNCE_SELF_ITER) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 4 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:qemu_announce_self_iter " "%s:%s:%s skip: %d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , id, name, mac, skip);
#line 67 "trace/trace-net.h"
        } else {
#line 4 "/opt/work/kvemu/net/trace-events"
            qemu_log("qemu_announce_self_iter " "%s:%s:%s skip: %d" "\n", id, name, mac, skip);
#line 71 "trace/trace-net.h"
        }
    }
}

static inline void trace_qemu_announce_self_iter(const char * id, const char * name, const char * mac, int skip)
{
    if (true) {
        _nocheck__trace_qemu_announce_self_iter(id, name, mac, skip);
    }
}

#define TRACE_QEMU_ANNOUNCE_TIMER_DEL_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_QEMU_ANNOUNCE_TIMER_DEL) || \
    false)

static inline void _nocheck__trace_qemu_announce_timer_del(bool free_named, bool free_timer, char * id)
{
    if (trace_event_get_state(TRACE_QEMU_ANNOUNCE_TIMER_DEL) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 5 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:qemu_announce_timer_del " "free named: %d free timer: %d id: %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , free_named, free_timer, id);
#line 98 "trace/trace-net.h"
        } else {
#line 5 "/opt/work/kvemu/net/trace-events"
            qemu_log("qemu_announce_timer_del " "free named: %d free timer: %d id: %s" "\n", free_named, free_timer, id);
#line 102 "trace/trace-net.h"
        }
    }
}

static inline void trace_qemu_announce_timer_del(bool free_named, bool free_timer, char * id)
{
    if (true) {
        _nocheck__trace_qemu_announce_timer_del(free_named, free_timer, id);
    }
}

#define TRACE_VHOST_USER_EVENT_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_VHOST_USER_EVENT) || \
    false)

static inline void _nocheck__trace_vhost_user_event(const char * chr, int event)
{
    if (trace_event_get_state(TRACE_VHOST_USER_EVENT) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 8 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:vhost_user_event " "chr: %s got event: %d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , chr, event);
#line 129 "trace/trace-net.h"
        } else {
#line 8 "/opt/work/kvemu/net/trace-events"
            qemu_log("vhost_user_event " "chr: %s got event: %d" "\n", chr, event);
#line 133 "trace/trace-net.h"
        }
    }
}

static inline void trace_vhost_user_event(const char * chr, int event)
{
    if (true) {
        _nocheck__trace_vhost_user_event(chr, event);
    }
}

#define TRACE_COLO_PROXY_MAIN_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_PROXY_MAIN) || \
    false)

static inline void _nocheck__trace_colo_proxy_main(const char * chr)
{
    if (trace_event_get_state(TRACE_COLO_PROXY_MAIN) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 11 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_proxy_main " ": %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , chr);
#line 160 "trace/trace-net.h"
        } else {
#line 11 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_proxy_main " ": %s" "\n", chr);
#line 164 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_proxy_main(const char * chr)
{
    if (true) {
        _nocheck__trace_colo_proxy_main(chr);
    }
}

#define TRACE_COLO_COMPARE_MAIN_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_COMPARE_MAIN) || \
    false)

static inline void _nocheck__trace_colo_compare_main(const char * chr)
{
    if (trace_event_get_state(TRACE_COLO_COMPARE_MAIN) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 14 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_compare_main " ": %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , chr);
#line 191 "trace/trace-net.h"
        } else {
#line 14 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_compare_main " ": %s" "\n", chr);
#line 195 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_compare_main(const char * chr)
{
    if (true) {
        _nocheck__trace_colo_compare_main(chr);
    }
}

#define TRACE_COLO_COMPARE_DROP_PACKET_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_COMPARE_DROP_PACKET) || \
    false)

static inline void _nocheck__trace_colo_compare_drop_packet(const char * queue, const char * chr)
{
    if (trace_event_get_state(TRACE_COLO_COMPARE_DROP_PACKET) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 15 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_compare_drop_packet " ": %s: %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , queue, chr);
#line 222 "trace/trace-net.h"
        } else {
#line 15 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_compare_drop_packet " ": %s: %s" "\n", queue, chr);
#line 226 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_compare_drop_packet(const char * queue, const char * chr)
{
    if (true) {
        _nocheck__trace_colo_compare_drop_packet(queue, chr);
    }
}

#define TRACE_COLO_COMPARE_UDP_MISCOMPARE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_COMPARE_UDP_MISCOMPARE) || \
    false)

static inline void _nocheck__trace_colo_compare_udp_miscompare(const char * sta, int size)
{
    if (trace_event_get_state(TRACE_COLO_COMPARE_UDP_MISCOMPARE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 16 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_compare_udp_miscompare " ": %s = %d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , sta, size);
#line 253 "trace/trace-net.h"
        } else {
#line 16 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_compare_udp_miscompare " ": %s = %d" "\n", sta, size);
#line 257 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_compare_udp_miscompare(const char * sta, int size)
{
    if (true) {
        _nocheck__trace_colo_compare_udp_miscompare(sta, size);
    }
}

#define TRACE_COLO_COMPARE_ICMP_MISCOMPARE_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_COMPARE_ICMP_MISCOMPARE) || \
    false)

static inline void _nocheck__trace_colo_compare_icmp_miscompare(const char * sta, int size)
{
    if (trace_event_get_state(TRACE_COLO_COMPARE_ICMP_MISCOMPARE) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 17 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_compare_icmp_miscompare " ": %s = %d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , sta, size);
#line 284 "trace/trace-net.h"
        } else {
#line 17 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_compare_icmp_miscompare " ": %s = %d" "\n", sta, size);
#line 288 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_compare_icmp_miscompare(const char * sta, int size)
{
    if (true) {
        _nocheck__trace_colo_compare_icmp_miscompare(sta, size);
    }
}

#define TRACE_COLO_COMPARE_IP_INFO_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_COMPARE_IP_INFO) || \
    false)

static inline void _nocheck__trace_colo_compare_ip_info(int psize, const char * sta, const char * stb, int ssize, const char * stc, const char * std)
{
    if (trace_event_get_state(TRACE_COLO_COMPARE_IP_INFO) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 18 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_compare_ip_info " "ppkt size = %d, ip_src = %s, ip_dst = %s, spkt size = %d, ip_src = %s, ip_dst = %s" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , psize, sta, stb, ssize, stc, std);
#line 315 "trace/trace-net.h"
        } else {
#line 18 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_compare_ip_info " "ppkt size = %d, ip_src = %s, ip_dst = %s, spkt size = %d, ip_src = %s, ip_dst = %s" "\n", psize, sta, stb, ssize, stc, std);
#line 319 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_compare_ip_info(int psize, const char * sta, const char * stb, int ssize, const char * stc, const char * std)
{
    if (true) {
        _nocheck__trace_colo_compare_ip_info(psize, sta, stb, ssize, stc, std);
    }
}

#define TRACE_COLO_OLD_PACKET_CHECK_FOUND_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_OLD_PACKET_CHECK_FOUND) || \
    false)

static inline void _nocheck__trace_colo_old_packet_check_found(int64_t old_time)
{
    if (trace_event_get_state(TRACE_COLO_OLD_PACKET_CHECK_FOUND) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 19 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_old_packet_check_found " "%" PRId64 "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , old_time);
#line 346 "trace/trace-net.h"
        } else {
#line 19 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_old_packet_check_found " "%" PRId64 "\n", old_time);
#line 350 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_old_packet_check_found(int64_t old_time)
{
    if (true) {
        _nocheck__trace_colo_old_packet_check_found(old_time);
    }
}

#define TRACE_COLO_COMPARE_TCP_INFO_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_COMPARE_TCP_INFO) || \
    false)

static inline void _nocheck__trace_colo_compare_tcp_info(const char * pkt, uint32_t seq, uint32_t ack, int hdlen, int pdlen, int offset, int flags)
{
    if (trace_event_get_state(TRACE_COLO_COMPARE_TCP_INFO) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 20 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_compare_tcp_info " "%s: seq/ack= %u/%u hdlen= %d pdlen= %d offset= %d flags=%d" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , pkt, seq, ack, hdlen, pdlen, offset, flags);
#line 377 "trace/trace-net.h"
        } else {
#line 20 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_compare_tcp_info " "%s: seq/ack= %u/%u hdlen= %d pdlen= %d offset= %d flags=%d" "\n", pkt, seq, ack, hdlen, pdlen, offset, flags);
#line 381 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_compare_tcp_info(const char * pkt, uint32_t seq, uint32_t ack, int hdlen, int pdlen, int offset, int flags)
{
    if (true) {
        _nocheck__trace_colo_compare_tcp_info(pkt, seq, ack, hdlen, pdlen, offset, flags);
    }
}

#define TRACE_COLO_FILTER_REWRITER_PKT_INFO_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_FILTER_REWRITER_PKT_INFO) || \
    false)

static inline void _nocheck__trace_colo_filter_rewriter_pkt_info(const char * func, const char * src, const char * dst, uint32_t seq, uint32_t ack, uint32_t flag)
{
    if (trace_event_get_state(TRACE_COLO_FILTER_REWRITER_PKT_INFO) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 23 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_filter_rewriter_pkt_info " "%s: src/dst: %s/%s p: seq/ack=%u/%u  flags=0x%x" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , func, src, dst, seq, ack, flag);
#line 408 "trace/trace-net.h"
        } else {
#line 23 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_filter_rewriter_pkt_info " "%s: src/dst: %s/%s p: seq/ack=%u/%u  flags=0x%x" "\n", func, src, dst, seq, ack, flag);
#line 412 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_filter_rewriter_pkt_info(const char * func, const char * src, const char * dst, uint32_t seq, uint32_t ack, uint32_t flag)
{
    if (true) {
        _nocheck__trace_colo_filter_rewriter_pkt_info(func, src, dst, seq, ack, flag);
    }
}

#define TRACE_COLO_FILTER_REWRITER_CONN_OFFSET_BACKEND_DSTATE() ( \
    trace_event_get_state_dynamic_by_id(TRACE_COLO_FILTER_REWRITER_CONN_OFFSET) || \
    false)

static inline void _nocheck__trace_colo_filter_rewriter_conn_offset(uint32_t offset)
{
    if (trace_event_get_state(TRACE_COLO_FILTER_REWRITER_CONN_OFFSET) && qemu_loglevel_mask(LOG_TRACE)) {
        if (message_with_timestamp) {
            struct timeval _now;
            gettimeofday(&_now, NULL);
#line 24 "/opt/work/kvemu/net/trace-events"
            qemu_log("%d@%zu.%06zu:colo_filter_rewriter_conn_offset " ": offset=%u" "\n",
                     qemu_get_thread_id(),
                     (size_t)_now.tv_sec, (size_t)_now.tv_usec
                     , offset);
#line 439 "trace/trace-net.h"
        } else {
#line 24 "/opt/work/kvemu/net/trace-events"
            qemu_log("colo_filter_rewriter_conn_offset " ": offset=%u" "\n", offset);
#line 443 "trace/trace-net.h"
        }
    }
}

static inline void trace_colo_filter_rewriter_conn_offset(uint32_t offset)
{
    if (true) {
        _nocheck__trace_colo_filter_rewriter_conn_offset(offset);
    }
}
#endif /* TRACE_NET_GENERATED_TRACERS_H */
