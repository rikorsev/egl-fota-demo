#include <assert.h>

#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "main"
#define MAIN_BLINCK_PERIOD_MS (1000U)

#if 0
static egl_result_t init_trace(void)
{
    egl_result_t result;
    static egl_trace_t trace = {0};

    /* Init stdio interface */
    result = egl_itf_init(STDIO);
    EGL_RESULT_CHECK(result);

    /* Init tracer */
    result = egl_trace_init(&trace, STDIO);
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

    /* Init platform */
    result = egl_plat_init(PLATFORM);
    EGL_RESULT_CHECK(result);

#if 0
    result = init_trace();
    EGL_RESULT_CHECK(result);
#endif

    /* Init led */
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
    egl_result_t result;

    result = egl_pio_toggle(LED_GREEN);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to toggle led. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    egl_plat_sleep(PLATFORM, MAIN_BLINCK_PERIOD_MS);
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

    EGL_TRACE_INFO("Welcome from %s", egl_plat_info(PLATFORM));

    while(1)
    {
        loop();
    }

    return 0;
}