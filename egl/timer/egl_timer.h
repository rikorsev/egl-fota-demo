#ifndef EGL_TIMER_H
#define EGL_TIMER_H

#include <stdint.h>

#include "egl_result.h"

typedef struct
{
    egl_result_t    (*init)   (void);
    uint32_t        (*get) (void);
    egl_result_t    (*update) (void);
    egl_result_t    (*set)    (uint32_t val);
    egl_result_t    (*deinit) (void);
}egl_timer_t;

egl_result_t egl_timer_init   (egl_timer_t *timer);
uint32_t     egl_timer_get    (egl_timer_t *timer);
egl_result_t egl_timer_update (egl_timer_t *timer);
egl_result_t egl_timer_set    (egl_timer_t *timer, uint32_t val);
egl_result_t egl_timer_deinit (egl_timer_t *timer);

#endif /* EGL_TIMER_H */
