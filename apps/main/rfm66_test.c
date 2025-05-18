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

}

egl_result_t rfm_test_init(void)
{
    egl_result_t result;

    result = egl_rfm66_init(PLAT_RFM66);
    EGL_RESULT_CHECK(result);

    return result;
}