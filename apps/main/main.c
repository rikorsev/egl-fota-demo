#include <assert.h>

#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "main"

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

    result = egl_pio_init(LED_GREEN);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init led. Result %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static void blink(void)
{
    egl_pio_set(LED_GREEN, true);
    egl_plat_sleep(PLATFORM, 50);
    egl_pio_set(LED_GREEN, false);
    egl_plat_sleep(PLATFORM, 950);
}

static void loop(void)
{
    blink();
}

int main(void)
{
    egl_result_t result = init();
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_FAIL("Fatal error");
        EGL_RESULT_FATAL();
    }

    egl_plat_info_t *info = egl_plat_info(PLATFORM);
    EGL_TRACE_INFO("Application %s (%u.%u.%u%s) started", info->name,
                                                            info->version.major,
                                                            info->version.minor,
                                                            info->version.revision,
                                                            info->version.sufix);

    while(1)
    {
        loop();
    }

    return 0;
}