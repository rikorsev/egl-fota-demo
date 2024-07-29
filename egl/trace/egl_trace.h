#ifndef EGL_TRACE_H
#define EGL_TRACE_H

#include "egl_timer.h"
#include "egl_interface.h"

typedef enum
{
    EGL_TRACE_LEVEL_DEBUG,
    EGL_TRACE_LEVEL_INFO,
    EGL_TRACE_LEVEL_ERROR,
    EGL_TRACE_LEVEL_FAIL,
    EGL_TRACE_LEVEL_LAST
}egl_trace_level_t;

typedef struct
{
    egl_interface_t   *iface;
    egl_timer_t       *timer;
    char              buff[CONFIG_EGL_TRACE_BUFF_SIZE];
}egl_trace_t;

#if CONFIG_EGL_TRACE_ENABLED
#define EGL_TRACE_DEBUG(fmt, ...) egl_trace(egl_trace_default_get(), EGL_TRACE_LEVEL_DEBUG, EGL_MODULE_NAME, fmt, ##__VA_ARGS__)
#define EGL_TRACE_INFO(fmt, ...)  egl_trace(egl_trace_default_get(), EGL_TRACE_LEVEL_INFO,  EGL_MODULE_NAME, fmt, ##__VA_ARGS__)
#define EGL_TRACE_ERROR(fmt, ...) egl_trace(egl_trace_default_get(), EGL_TRACE_LEVEL_ERROR, EGL_MODULE_NAME, fmt, ##__VA_ARGS__)
#define EGL_TRACE_FAIL(fmt, ...)  egl_trace(egl_trace_default_get(), EGL_TRACE_LEVEL_FAIL,  EGL_MODULE_NAME, fmt, ##__VA_ARGS__)

egl_result_t egl_trace_init(egl_trace_t *trace, egl_interface_t *iface, egl_timer_t *timer);
egl_result_t egl_trace(egl_trace_t *trace, egl_trace_level_t lvl, char *module, char *format, ...);
egl_result_t egl_trace_default_set(egl_trace_t *trace);
egl_trace_t *egl_trace_default_get(void);
#else
#define EGL_TRACE_DEBUG(fmt, ...)
#define EGL_TRACE_INFO(fmt, ...)
#define EGL_TRACE_ERROR(fmt, ...)
#define EGL_TRACE_FAIL(fmt, ...)

egl_result_t egl_trace_init(egl_trace_t *trace, egl_interface_t *iface) { return EGL_SUCCESS; }
egl_result_t egl_trace(egl_trace_level_t lvl, char* module, char* format, ...) { return EGL_SUCCESS; }
egl_result_t egl_trace_default_set(egl_trace_t *trace) { return EGL_SUCCESS; }
egl_trace_t *egl_trace_default_get(void) { return EGL_SUCCESS; }
#endif

#endif /* EGL_TRACE_H */
