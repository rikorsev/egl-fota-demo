#include <string.h>

#include "egl_lib.h"
#include "plat.h"

static egl_result_t error_hook_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
    egl_log(egl_log_default_get(), EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));

    return result;
}

static egl_result_t rfm_version_test_run(void)
{
    egl_result_t result;
    uint8_t version;

    /* Check version */
    result = egl_rfm66_version_get(PLAT_RFM66, &version);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RFM version: 0x%02x", version);

    return result;
}

static egl_result_t rfm_mode_test_run(void)
{
    egl_result_t result;
    egl_rfm66_mode_t op_mode;
    egl_rfm66_modulation_shaping_t modshap;
    egl_rfm66_modulation_type_t    modtype;

    result = egl_rfm66_mode_get(PLAT_RFM66, &op_mode);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_shaping_get(PLAT_RFM66, &modshap);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_type_get(PLAT_RFM66, &modtype);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("OP Mode: %u", op_mode);
    EGL_LOG_INFO("Modulation shaping: %u", modshap);
    EGL_LOG_INFO("Modulation type: %u", modtype);

    result = egl_rfm66_mode_set(PLAT_RFM66, EGL_RFM66_RX_MODE);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_shaping_set(PLAT_RFM66, EGL_RFM66_MODULATION_SHAPING_1);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_modulation_type_set(PLAT_RFM66, EGL_RFM66_MODULATION_TYPE_OOK);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t rfm_bitrate_test_run(void)
{
    egl_result_t result;
    uint32_t bitrate;

    result = egl_rfm66_bitrate_get(PLAT_RFM66, &bitrate);
    EGL_RESULT_CHECK(result);

    EGL_LOG_INFO("RFM bitrate: %u", bitrate);

    result = egl_rfm66_bitrate_set(PLAT_RFM66, 115200);
    EGL_RESULT_CHECK(result);

    return result;
}

void rfm_test_run(void)
{
    egl_result_t result;
    egl_result_error_hook_t error_hook = { error_hook_func };

    egl_result_error_hook_set(&error_hook);

    result = rfm_version_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_INFO("Version test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_mode_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_INFO("Mode test fail. Result: %s", EGL_RESULT(result));
    }

    result = rfm_bitrate_test_run();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_INFO("Bitrate test fail. Result: %s", EGL_RESULT(result));
    }
}

egl_result_t rfm_test_init(void)
{
    egl_result_t result;

    result = egl_rfm66_init(PLAT_RFM66);
    EGL_RESULT_CHECK(result);

    return result;
}