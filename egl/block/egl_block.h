/**
 * @file The module represents block devices such as flash, HDD etc.
 */

#ifndef EGL_BLOCK_H
#define EGL_BLOCK_H

#include <stdint.h>
#include "egl_result.h"

typedef struct
{
    egl_result_t (*init)(void);
    egl_result_t (*read)(uint32_t addr, void *data);
    egl_result_t (*write)(uint32_t addr, void *data);
    egl_result_t (*deinit)(void);
}egl_block_t;

/**
 * @brief Init block device
 *
 * @param block - pointer to block device to init
 *
 * @return EGL_SUCCESS if the device is inited successfully
 */
egl_result_t egl_block_init(egl_block_t *block);

/**
 * @brief Read from block device
 *
 * @param block - pointer to block device read from
 * @param addr - address to read from
 * @param data - pointer to buffer which whill store read data
 *
 * @return EGL_SUCCESS in case of successfull read
 */
egl_result_t egl_block_read(egl_block_t *block, uint32_t addr, void *data);

/**
 * @brief Write to block device
 *
 * @param block - pointer to block device to write
 * @param addr - address to write
 * @param data - data that shall be written to block device
 *
 * @return EGL_SUCCESS in case of successful write
 */
egl_result_t egl_block_write(egl_block_t *block, uint32_t addr, void *data);

/**
 * @brief Deinit block device
 *
 * @param block - pointer to block device to deinit
 */
egl_result_t egl_block_deinit(egl_block_t *block);

#endif
