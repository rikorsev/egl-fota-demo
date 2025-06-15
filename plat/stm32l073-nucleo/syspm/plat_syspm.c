#include "egl_lib.h"
#include "plat.h"

static egl_result_t reset(void)
{
    __NVIC_SystemReset();
    return EGL_SUCCESS;
}

egl_pm_t plat_syspm_inst =
{
    .reset = reset
};

egl_pm_t *plat_syspm_get(void)
{
    return &plat_syspm_inst;
}
