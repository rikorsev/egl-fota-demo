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

egl_result_t egl_block_init(egl_block_t *block);
egl_result_t egl_block_read(egl_block_t *block, uint32_t addr, void *data);
egl_result_t egl_block_write(egl_block_t *block, uint32_t addr, void *data);
egl_result_t egl_block_deinit(egl_block_t *block);

#endif
