/**
 * @file The module represents singe pin input output device
 */

#ifndef EGL_PIO_H
#define EGL_PIO_H

#include <stdbool.h>

typedef void (*egl_pio_callback_t)(void *data);

typedef struct
{
    egl_result_t (*init)   (void);
    egl_result_t (*set)    (bool state);
    egl_result_t (*get)    (void);
    egl_result_t (*deinit) (void);
    egl_pio_callback_t callback;
}egl_pio_t;

/**
 * @brief Init PIO
 *
 * @param pio - pointer to pio to init
 *
 * @return EGL_SUCCESS in case of successful initialization
 */
egl_result_t   egl_pio_init   (egl_pio_t *pio);

/**
 * @brief Set state of PIO
 *
 * @param pio - pointer to pio to set state
 * @param state - state valie to set
 *
 * @return EGL_SUCCES in case of successful setting
 */
egl_result_t   egl_pio_set    (egl_pio_t *pio, bool state);

/**
 * @brief Get PIO state
 *
 * @param pio - pionter to pio to get statte
 *
 * @return EGL_RESET - if pio is reset, EGL_SET - if pio is set.
 */
egl_result_t   egl_pio_get    (egl_pio_t *pio);

/**
 * @brief Set callback for PIO
 *
 * The call back will be triggered as soon as pio will change it state according to it settings
 * Usefull to track button state
 *
 * @param pio - pionter to pio to set callback
 * @param callback - pointer to callback function
 *
 * @return EGL_SUCCESS on case if callback has been set successfully
 */
egl_result_t   egl_pio_callback_set(egl_pio_t *pio, egl_pio_callback_t callback);

/**
 * @brief Toggle PIO state
 *
 * @param pio - pointer to pio to toggle state
 *
 * @return EGL_SUCCESS in case of successfull toggling
 */
egl_result_t   egl_pio_toggle (egl_pio_t *pio);

/**
 * @brief Deinit PIO
 *
 * @param pio - poiniter to pio to deinit
 *
 * @return EGL_SUCCESS in case of successful deinitialization
 */
egl_result_t   egl_pio_deinit (egl_pio_t *pio);

#endif