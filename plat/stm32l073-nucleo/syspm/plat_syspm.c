#include "egl_lib.h"
#include "plat.h"

static egl_result_t reset(void)
{
    __NVIC_SystemReset();
    return EGL_SUCCESS;
}

static egl_result_t mode_set(uint32_t mode)
{
    switch(mode)
    {
        case PLAT_SYSPM_WAIT:
            __WFI();
            break;
    }

    return EGL_SUCCESS;
}


egl_pm_t plat_syspm_inst =
{
    .reset = reset,
    .mode_set = mode_set
};

egl_pm_t *plat_syspm_get(void)
{
    return &plat_syspm_inst;
}
