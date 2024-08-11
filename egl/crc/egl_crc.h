/**
 * @file the module represents CRC calculation device of software implementation
 */

#ifndef EGL_CRC_H
#define EGL_CRC_H

#include "egl_result.h"

typedef struct
{
    egl_result_t (*init)(uint32_t poly, uint32_t init_val);
    uint8_t      (*calc8)(void *data, size_t len);
    uint16_t     (*calc16)(void *data, size_t len);
    uint32_t     (*calc32)(void *data, size_t len);
    egl_result_t (*reset)(void);
    egl_result_t (*deinit)(void);
}egl_crc_t;

/**
 * @brief Init CRC
 *
 * @param crc - pointer to CRC instatnce to init
 * @param poly - value of CRC polynominal
 * @param start_val - initial value for crc calculation
 *
 * @return EGL_SUCCESS in case of successfull initialization
 */
egl_result_t egl_crc_init  (egl_crc_t *crc, uint32_t poly, uint32_t start_val);

/**
 * @brief Reset CRC calculation to it init/start value
 *
 * @param crc - pointer to CRC instance to reset
 *
 * @return EGL_SUCCESS in case of successfull reset
 */
egl_result_t egl_crc_reset (egl_crc_t *crc);

/**
 * @brief Calculate CRC8
 *
 * @param crc - pointer to CRC instance
 * @param data - pointer to data to calculate
 * @param len - size of data in bytes
 *
 * @return Calculated CRC8 value
 */
uint8_t      egl_crc8_calc (egl_crc_t *crc, void *data, size_t len);

/**
 * @brief Calculate CRC16
 *
 * @param crc - pointer to CRC instance
 * @param data - pointer to data to calculate
 * @param len - size of data in bytes
 *
 * @return Calculated CRC16 value
 */
uint16_t     egl_crc16_calc(egl_crc_t *crc, void *data, size_t len);

/**
 * @brief Calculate CRC32
 *
 * @param crc - pointer to CRC instance
 * @param data - pointer to data to calculate
 * @param len - size of data in bytes
 *
 * @return Calculated CRC32 value
 */
uint32_t     egl_crc32_calc(egl_crc_t *crc, void *data, size_t len);

/**
 * @brief Deinit CRC instance
 *
 * @param crc - pointer to CRC instance to deinit
 *
 * @return EGL_SUCCESS in case of successful deinitialization
 */
egl_result_t egl_crc_deinit(egl_crc_t *crc);

#endif
