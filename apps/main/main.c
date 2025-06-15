#include "egl_lib.h"
#include "plat.h"
#include "rfm_test.h"

static egl_result_t error_hook_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static egl_result_t init(void)
{
    egl_result_t result;
    egl_result_error_hook_t error_hook = { error_hook_func };

    result = egl_system_init(SYSTEM);
    EGL_RESULT_CHECK(result);

    egl_result_error_hook_set(&error_hook);

    result = egl_pio_init(SYSLED);
    EGL_RESULT_CHECK(result);

    result = rfm_test_init();
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t info(void)
{
#if CONFIG_EGL_LOG_ENABLED
    slot_info_t *info = egl_plat_info(PLATFORM);
    EGL_ASSERT_CHECK(info, EGL_NULL_POINTER);

    EGL_LOG_INFO("Application %s (%u.%u.%u%s) started", info->name,
                                                          info->version.major,
                                                          info->version.minor,
                                                          info->version.revision,
                                                          info->version.sufix);
    EGL_LOG_INFO("Date: %s", info->buildtime);
    EGL_LOG_INFO("Size: %u", info->size);
    EGL_LOG_INFO("Checksum: %08x", info->checksum);
#endif

    return EGL_SUCCESS;
}

static void blink(void)
{
    egl_pio_set(SYSLED, true);
    egl_sys_delay(50);
    egl_pio_set(SYSLED, false);
    egl_sys_delay(950);

    EGL_LOG_INFO("Tick...");
}

static void loop(void)
{
    blink();
    rfm_test_run();
}

int main(void)
{
    egl_result_t result = init();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_FAIL("Fatal error");
        EGL_RESULT_FATAL();
    }

    result = info();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_FAIL("Fatal error");
        EGL_RESULT_FATAL();
    }

    while(1)
    {
        loop();
    }

    return 0;
}