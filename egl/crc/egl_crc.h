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

egl_result_t egl_crc_init  (egl_crc_t *crc, uint32_t poly, uint32_t start_val);
egl_result_t egl_crc_reset (egl_crc_t *crc);
uint8_t      egl_crc8_calc (egl_crc_t *crc, void *data, size_t len);
uint16_t     egl_crc16_calc(egl_crc_t *crc, void *data, size_t len);
uint32_t     egl_crc32_calc(egl_crc_t *crc, void *data, size_t len);
egl_result_t egl_crc_deinit(egl_crc_t *crc);

#endif
