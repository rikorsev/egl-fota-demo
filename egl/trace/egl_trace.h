/**
 * @file The module provides trace/logging macroses
 */

#ifndef EGL_TRACE_H
#define EGL_TRACE_H

#include "egl_timer.h"
#include "egl_interface.h"

typedef enum
{
    EGL_TRACE_LEVEL_DEBUG,
    EGL_TRACE_LEVEL_INFO,
    EGL_TRACE_LEVEL_WARN,
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
#define EGL_TRACE_WARN(fmt, ...)  egl_trace(egl_trace_default_get(), EGL_TRACE_LEVEL_WARN,  EGL_MODULE_NAME, fmt, ##__VA_ARGS__)
#define EGL_TRACE_ERROR(fmt, ...) egl_trace(egl_trace_default_get(), EGL_TRACE_LEVEL_ERROR, EGL_MODULE_NAME, fmt, ##__VA_ARGS__)
#define EGL_TRACE_FAIL(fmt, ...)  egl_trace(egl_trace_default_get(), EGL_TRACE_LEVEL_FAIL,  EGL_MODULE_NAME, fmt, ##__VA_ARGS__)

/**
 * @brief init trace module
 *
 * @param trace - pointer to trace module
 * @param iface - pointer to serial interface that will be used to trace the data
 * @param timer - pointer to timer that will provide timestamps
 *
 * @param EGL_SUCCESS in case of siccessfull initialization
 */
egl_result_t egl_trace_init(egl_trace_t *trace, egl_interface_t *iface, egl_timer_t *timer);

/**
 * @brief Finction to trace specific message
 *
 * Usually the function is not used directly. It is triggered through corresponding mcaroses
 * Like: EGL_TRACE_INFO, EGL_TRACE_ERROR etc
 *
 * @param trace - pointer to the trace instance
 * @param lvl - trace/log level of the message like EGL_TRACE_LEVEL_DEBUG, EGL_TRACE_LEVEL_INFO etc
 * @param module - pointer to module name from which trace function has called
 * @param format - pointer to format stirng
 * @param args   - variable number of arguments
 *
 * @return EGL_SUCCESS in case if the message has been traced successfully
 */
egl_result_t egl_trace(egl_trace_t *trace, egl_trace_level_t lvl, char *module, char *format, ...);

/**
 * @brief Set default tracer
 *
 * Default traces is used by trace marcoses to choosw through which tracer data should be passsed
 *
 * @param trace - pointer to default tracer to set
 *
 * @return EGL_SUCCESS in case if the tracer has been set successfully
 */
egl_result_t egl_trace_default_set(egl_trace_t *trace);

/**
 * @brief Get default tracer
 *
 * @return pointer to default tracer
 */
egl_trace_t *egl_trace_default_get(void);
#else
#define EGL_TRACE_DEBUG(fmt, ...)
#define EGL_TRACE_INFO(fmt, ...)
#define EFL_TRACE_WARN(fmt, ...)
#define EGL_TRACE_ERROR(fmt, ...)
#define EGL_TRACE_FAIL(fmt, ...)

egl_result_t egl_trace_init(egl_trace_t *trace, egl_interface_t *iface) { return EGL_SUCCESS; }
egl_result_t egl_trace(egl_trace_t *trace, egl_trace_level_t lvl, char *module, char *format, ...) { return EGL_SUCCESS; }
egl_result_t egl_trace_default_set(egl_trace_t *trace) { return EGL_SUCCESS; }
egl_trace_t *egl_trace_default_get(void) { return EGL_SUCCESS; }
#endif

#endif /* EGL_TRACE_H */
