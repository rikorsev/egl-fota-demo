#include <string.h>
#include "egl_lib.h"
#include "plat.h"

static egl_result_t init(void)
{
    return EGL_SUCCESS;
}

static egl_result_t read(uint32_t addr, void *data)
{
    memcpy(data, (void *)addr, PLAT_FLASH_PAGE_SIZE);

    return EGL_SUCCESS;
}

static egl_result_t write(uint32_t addr, void *data)
{
    EGL_LOG_DEBUG("flash write page. Addr: %u", addr);

    return EGL_SUCCESS;
}

static const egl_block_t egl_block_flash_inst =
{
    .init = init,
    .read = read,
    .write = write
};

egl_block_t *egl_block_flash_get(void)
{
    return (egl_block_t *)&egl_block_flash_inst;
}