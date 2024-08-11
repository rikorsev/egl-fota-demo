#include <assert.h>

#include "egl_lib.h"
#include "plat.h"
#include "fota_mgr.h"
#include "boot_mgr.h"

#define EGL_MODULE_NAME "boot_main"
#define BOOT_CRC_POLY ((uint32_t)0x4C11DB7)
#define BOOT_CRC_INIT ((uint32_t)0xFFFFFFFF)

#if CONFIG_EGL_TRACE_ENABLED
static egl_result_t init_trace(void)
{
    egl_result_t result;
    static egl_trace_t trace = {0};

    /* Init trace iface */
    result = egl_itf_init(TRACE_IFACE);
    EGL_RESULT_CHECK(result);

    /* Init tracer */
    result = egl_trace_init(&trace, TRACE_IFACE, SYSTIMER);
    EGL_RESULT_CHECK(result);

    /* Set default tracer */
    result = egl_trace_default_set(&trace);
    EGL_RESULT_CHECK(result);

    return result;
}
#endif

static egl_result_t init(void)
{
    egl_result_t result;

    result = egl_plat_init(PLATFORM);
    EGL_RESULT_CHECK(result);

    result = egl_timer_init(SYSTIMER);
    EGL_RESULT_CHECK(result);

#if CONFIG_EGL_TRACE_ENABLED
    result = init_trace();
    EGL_RESULT_CHECK(result);
#endif

    result = egl_crc_init(PLAT_CRC, BOOT_CRC_POLY, BOOT_CRC_INIT);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init CRC. Result %s", EGL_RESULT(result));
        return result;
    }

    result = egl_block_init(PLAT_FLASH);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init flash. Result %s", EGL_RESULT(result));
        return result;
    }

    result = fota_mgr_init();
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init fota module. Result %s", EGL_RESULT(result));
        return result;
    }

    result = boot_mgr_init();
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init boot module. Result %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

int main(void)
{
    egl_result_t result = init();
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_FAIL("Fatal error while platform initilaization. Result: %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    fota_mgr_process();
    boot_mgr_process();

    EGL_TRACE_INFO("No bootable image detected. Reboot...");
    result = egl_plat_reboot(PLATFORM);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_FAIL("Fail to reboot platform. Result: %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    return 0;
}