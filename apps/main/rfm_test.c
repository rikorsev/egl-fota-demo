#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "rfm_test"

static egl_result_t rfm_version_test_run(void)
{
    egl_result_t result;
    uint8_t version;

    /* Check version */
    result = egl_rfm69_read_byte(PLAT_RFM69, EGL_RFM69_REG_VERSION, &version);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to read rfm69 version. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RFM version: 0x%02x", version);

    return result;
}

static egl_result_t rfm_bitrate_test_run(void)
{
    egl_result_t result;
    uint32_t bitrate;

    result = egl_rfm69_bitrate_get(PLAT_RFM69, &bitrate);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get rfm69 bitrate. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RFM bitrate: %u", bitrate);

    result = egl_rfm69_bitrate_set(PLAT_RFM69, 115200);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set rfm69 bitrate. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static egl_result_t rfm_mode_test_run(void)
{
    egl_result_t result;
    egl_rfm_mode_t mode;

    result = egl_rfm69_mode_get(PLAT_RFM69, &mode);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to get rfm69 mode. Result: %s", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RFM mode: %u", mode);

    result = egl_rfm69_mode_set(PLAT_RFM69, EGL_RFM69_RX_MODE);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to set rfm69 mode. Result: %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

void rfm_test_run(void)
{
    egl_result_t result;

    result = rfm_version_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Version test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_bitrate_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Bitrate test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_mode_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Mode test fail. Result: %s", EGL_RESULT(result));
    }
}

egl_result_t rfm_test_init(void)
{
    egl_result_t result;

    result = egl_rfm69_init(PLAT_RFM69);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to init rfm69 module. Result: %d", EGL_RESULT(result));
        return result;
    }

    return result;
}