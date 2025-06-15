#include "egl_lib.h"

extern egl_platform_t platform_inst;
extern egl_log_t plat_syslog_inst;
extern egl_clock_t plat_sysclock_inst;
extern egl_pm_t plat_syspm_inst;
extern egl_timer_t plat_systimer_inst;

static egl_system_t plat_system_inst = 
{
    .plat = &platform_inst,
    .syslog = &plat_syslog_inst,
    .sysclock = &plat_sysclock_inst,
    .syspm = &plat_syspm_inst,
    .systimer = &plat_systimer_inst,
};

egl_system_t *plat_system_get(void)
{
    return &plat_system_inst;
}