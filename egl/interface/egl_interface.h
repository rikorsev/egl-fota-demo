/**
 * @file The module represents different kind of serial interfaces like UART, SPI, I2C etc.
 */

#ifndef EGL_INTERFACE_H
#define EGL_INTERFACE_H

#include "stdint.h"
#include "stdlib.h"

typedef struct
{
  egl_result_t     (*init)          (void);
  egl_result_t     (*open)          (void);
  size_t           (*write)         (void* data, size_t len);
  size_t           (*read)          (void* data, size_t len);
  egl_result_t     (*ioctl)         (uint8_t opcode, void* data, size_t len);
  egl_result_t     (*close)         (void);
  egl_result_t     (*deinit)        (void);
}egl_interface_t;

/**
 * @brief Init serial interface
 *
 * @param itf - pointer to serial interface to init
 *
 * @return EGL_SUCCESS in case of successful initialization
 */
egl_result_t egl_itf_init  (egl_interface_t *itf);

/**
 * @brief Open serial interface
 *
 * @param itf - pointer to serial interface to open
 *
 * @return EGL_SUCCESS in case of successful opening
 */
egl_result_t egl_itf_open  (egl_interface_t *itf);

/**
 * @brief Write to serial interface
 *
 * @param itf - pionter to serial interface to write
 * @param buff - pointer to data to write
 * @param len - size of data to write in bytes
 *
 * @return EGL_SUCCESS in case of successfull write
 */
egl_result_t egl_itf_write (egl_interface_t *itf, void *buff, size_t *len);

/**
 * @brief Perform IOCTL operation
 *
 * @param itf - pointer to serial interface
 * @param opcode - Operational code to perform
 * @param data - additional data required to perform IOCTL
 * @param len - size of data
 *
 * @return EGL_SUCCESS in case of success
 */
egl_result_t egl_itf_ioctl (egl_interface_t *itf, uint8_t opcode, void *data, size_t len);

/**
 * @brief Read from serial interface
 *
 * @param itf - pointer to serial interface to read from
 * @param buff - pointer to buffer to store read data
 * @param len - len of buffer/len of read data
 *
 * @return EGL_SUCCESS in case of successful read
 */
egl_result_t egl_itf_read  (egl_interface_t *itf, void *buff, size_t *len);

/**
 * @brief Close serial interface
 *
 * @param itf - pointer to serial interface to close
 *
 * @return EGL_SUCCESS in case if successful closeing
 */
egl_result_t egl_itf_close (egl_interface_t *itf);

/**
 * @brief Deinit serial interface
 *
 * @param itf - pointer to serial interface to deinit
 *
 * @return EGL_SUCCESS in case of successful deinitialization
 */
egl_result_t egl_itg_deinit(egl_interface_t *itf);

#endif /* EGL_INTERFACE_H */
