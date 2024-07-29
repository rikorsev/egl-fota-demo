#ifndef EGL_PLAT_H
#define EGL_PLAT_H

#include <stdint.h>

#include "egl_result.h"

typedef struct
{
    egl_result_t (*init)(void);
    egl_result_t (*sleep)(uint32_t delay);
    egl_result_t (*reboot)(void);
    egl_result_t (*shutdown)(void);
    char        *(*info)(void);
    uint32_t     (*clock)(void);
}egl_platform_t;

egl_result_t egl_plat_init(egl_platform_t *plat);
egl_result_t egl_plat_sleep(egl_platform_t *plat, uint32_t delay);
egl_result_t egl_plat_reboot(egl_platform_t *plat);
egl_result_t egl_plat_shutdown(egl_platform_t *plat);
char *egl_plat_info(egl_platform_t *plat);
uint32_t egl_plat_clock(egl_platform_t *plat);

#endif