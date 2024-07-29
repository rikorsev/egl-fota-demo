#include "egl_lib.h"
#include "plat.h"

static uint32_t clock(void)
{
    return SystemCoreClock;
}

static egl_result_t init(void)
{
    return EGL_SUCCESS;
}

static egl_result_t sleep(uint32_t delay)
{
    uint32_t target = egl_timer_get(SYSTIMER) + delay;

    while(egl_timer_get(SYSTIMER) < target)
    {
        /* Wait */
    }

    return EGL_SUCCESS;
}

static egl_result_t reboot(void)
{
    __NVIC_SystemReset();
    return EGL_SUCCESS;
}

static egl_result_t shutdown(void)
{
    return EGL_SUCCESS;
}

static char *info(void)
{
    return "NUCLEO64 (STM32L412)";
}

static egl_platform_t platform_inst =
{
    .init     = init,
    .reboot   = reboot,
    .sleep    = sleep,
    .shutdown = shutdown,
    .info     = info,
    .clock    = clock
};

egl_platform_t *platform_get(void)
{
    return &platform_inst;
}
