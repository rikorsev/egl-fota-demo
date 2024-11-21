#include "egl_lib.h"
#include "plat.h"
#include "rfm_test.h"

#define EGL_MODULE_NAME "main"

#if CONFIG_EGL_LOG_ENABLED
static egl_result_t init_log(void)
{
    egl_result_t result;
    static egl_log_t log = {0};

    /* Init log iface */
    result = egl_itf_init(SYSLOG);
    EGL_RESULT_CHECK(result);

    /* Init log */
    result = egl_log_init(&log, SYSLOG, SYSTIMER);
    EGL_RESULT_CHECK(result);

    /* Set default log */
    result = egl_log_default_set(&log);
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

#if CONFIG_EGL_LOG_ENABLED
    result = init_log();
    EGL_RESULT_CHECK(result);
#endif

    result = egl_pio_init(SYSLED);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to init led. Result %s", EGL_RESULT(result));
        return result;
    }

    result = rfm_test_init();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_ERROR("Fail to init RFM. Result %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static void blink(void)
{
    egl_pio_set(SYSLED, true);
    egl_pm_sleep(SYSPM, 50);
    egl_pio_set(SYSLED, false);
    egl_pm_sleep(SYSPM, 950);

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

#if CONFIG_EGL_LOG_ENABLED
    slot_info_t *info = egl_plat_info(PLATFORM);
    EGL_LOG_INFO("Application %s (%u.%u.%u%s) started", info->name,
                                                          info->version.major,
                                                          info->version.minor,
                                                          info->version.revision,
                                                          info->version.sufix);
    EGL_LOG_INFO("Date: %s", info->buildtime);
    EGL_LOG_INFO("Size: %u", info->size);
    EGL_LOG_INFO("Checksum: %08x", info->checksum);
#endif

    while(1)
    {
        loop();
    }

    return 0;
}