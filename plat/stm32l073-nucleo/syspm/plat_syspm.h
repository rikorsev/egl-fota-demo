#ifndef PLAT_SYSPM_H
#define PLAT_SYSPM_H

enum
{
    PLAT_SYSPM_RUN,
    PLAT_SYSPM_WAIT
};

egl_pm_t *plat_syspm_get(void);

#endif