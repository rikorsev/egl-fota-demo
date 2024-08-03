#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#include "egl_crc.h"

egl_result_t egl_crc_init(egl_crc_t *crc, uint32_t poly, uint32_t init)
{
    EGL_ASSERT_CHECK(crc);
    EGL_ASSERT_CHECK(crc->init);

    return crc->init(poly, init);
}

egl_result_t egl_crc_reset(egl_crc_t *crc)
{
    EGL_ASSERT_CHECK(crc);
    EGL_ASSERT_CHECK(crc->reset);

    return crc->reset();
}

uint8_t egl_crc8_calc(egl_crc_t *crc, void *data, size_t len)
{
    EGL_ASSERT_CHECK(crc);
    EGL_ASSERT_CHECK(crc->calc8);
    EGL_ASSERT_CHECK(data);

    return crc->calc8(data, len);
}

uint16_t egl_crc16_calc(egl_crc_t *crc, void *data, size_t len)
{
    EGL_ASSERT_CHECK(crc);
    EGL_ASSERT_CHECK(crc->calc16);
    EGL_ASSERT_CHECK(data);

    return crc->calc8(data, len);
}

uint32_t egl_crc32_calc(egl_crc_t *crc, void *data, size_t len)
{
    EGL_ASSERT_CHECK(crc);
    EGL_ASSERT_CHECK(crc->calc32);
    EGL_ASSERT_CHECK(data);

    return crc->calc32(data, len);
}

egl_result_t egl_crc_deinit(egl_crc_t *crc)
{
    EGL_ASSERT_CHECK(crc);
    EGL_ASSERT_CHECK(crc->deinit);

    return crc->deinit();
}
