/**
 * @file The module modeule represents different kind of timers
 */

#ifndef EGL_TIMER_H
#define EGL_TIMER_H

#include <stdint.h>

#include "egl_result.h"

typedef struct
{
    egl_result_t    (*init)   (void);
    uint32_t        (*get) (void);
    egl_result_t    (*set)    (uint32_t val);
    egl_result_t    (*deinit) (void);
}egl_timer_t;

/**
 * @brief Init timer
 *
 * @param timer - pointer to timer to init
 *
 * @return EGL_SUCCESS in case of successful initialization
 */
egl_result_t egl_timer_init   (egl_timer_t *timer);

/**
 * @brief Get timer value
 *
 * @param timer - pointer to th timer
 *
 * @return current value of the timer
 */
uint32_t     egl_timer_get    (egl_timer_t *timer);

/**
 * @brief Set timer value
 *
 * @param timer - pointer to the timer
 * @param val - value to set
 *
 * @return EGL_SUCCES in case of successfull setting
 */
egl_result_t egl_timer_set    (egl_timer_t *timer, uint32_t val);

/**
 * @brief Deinit the timer
 *
 * @param timer - pointer to the timer
 *
 * @return EGL_SUCCESS in case of successful deinitialization
 */
egl_result_t egl_timer_deinit (egl_timer_t *timer);

#endif /* EGL_TIMER_H */
