#include "egl_lib.h"
#include "plat.h"

static egl_result_t sleep(uint32_t delay)
{
    uint32_t target = egl_timer_get(SYSTIMER) + delay;

    while(egl_timer_get(SYSTIMER) < target)
    {
        /* Wait */
    }

    return EGL_SUCCESS;
}

static egl_result_t reset(void)
{
    __NVIC_SystemReset();
    return EGL_SUCCESS;
}

static egl_pm_t plat_syspm_inst =
{
    .sleep = sleep,
    .reset = reset
};

egl_pm_t *plat_syspm_get(void)
{
    return &plat_syspm_inst;
}
