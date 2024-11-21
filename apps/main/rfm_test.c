#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "rfm_test"

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

egl_result_t rfm_test_run(void)
{
    egl_result_t result;
    uint8_t version;

    result = egl_rfm69_read_byte(PLAT_RFM69, EGL_RFM69_REG_VERSION, &version);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to read rfm69 version. Result: %d", EGL_RESULT(result));
        return result;
    }

    EGL_LOG_INFO("RFM version: 0x%02x", version);

    return result;
}