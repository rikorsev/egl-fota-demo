#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "egl_result.h"
#include "egl_trace.h"

static const char *m_level_str[] =
{
    "DEBUG",
    "INFO",
    "ERROR",
    "FAIL"
};

static egl_trace_t *default_tracer = NULL;

egl_result_t egl_trace_init(egl_trace_t *trace, egl_interface_t *iface)
{
    egl_result_t result = EGL_UNKNOWN;

    EGL_ASSERT_CHECK(trace);
    EGL_ASSERT_CHECK(iface);

    trace->level = EGL_TRACE_LEVEL_DEBUG;
    trace->iface = iface;

    return EGL_SUCCESS;
}

egl_result_t egl_trace_default_set(egl_trace_t *trace)
{
    default_tracer = trace;
    return EGL_SUCCESS;
}

egl_trace_t *egl_trace_default_get(egl_trace_t *trace)
{
    return default_tracer;
}

egl_trace_t egl_trace(egl_trace_t *trace, egl_trace_level_t lvl, char *module, char *format, ...)
{
    va_list arg;
    int offset = 0;

    EGL_ASSERT_CHECK(trace);
    EGL_ASSERT_CHECK(format);
    EGL_ASSERT_CHECK(lvl < EGL_TRACE_LEVEL_LAST);
    EGL_ASSERT_CHECK(trace->iface);

    offset += snprintf(trace->buff, sizeof(trace->buff)"[%08u] ",
                         (unsigned int)egl_plat_time_get(trace->timer));
        EGL_ASSERT_CHECK(offset < 0);


    if(module != NULL)
    {
        offset += snprintf(trace->buff + offset, sizeof(trace->buff) - offset, "%s:", module);
        EGL_ASSERT_CHECK(offset < 0);
    }

    offset += snprintf(trace->buff + offset, sizeof(trace->buff) - offset, "[%s]", m_level_str[lvl]);
    EGL_ASSERT_CHECK(offset < 0);

    va_start(arg, format);
    offset += vsnprintf(trace->buff + offset, sizeof(trace->buff) - offset, format, arg);
    EGL_ASSERT_CHECK(offset < 0);
    va_end(arg);

    offset += snprintf(trace->buff + offset, sizeof(trace->buff) - offset, "\r\n");
    EGL_ASSERT_CHECK(offset < 0);

    return egl_itf_write(trace->iface, trace->buff, offset);
}
