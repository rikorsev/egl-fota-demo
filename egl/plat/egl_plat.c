#include <stdlib.h>

#include "egl_result.h"
#include "egl_plat.h"

egl_result_t egl_plat_init(egl_platform_t *plat)
{
    EGL_ASSERT_CHECK(plat);
    EGL_ASSERT_CHECK(plat->init);

    return plat->init();
}

egl_result_t egl_plat_sleep(egl_platform_t *plat, uint32_t delay)
{
    EGL_ASSERT_CHECK(plat);
    EGL_ASSERT_CHECK(plat->sleep);

    return plat->sleep(delay);
}

egl_result_t egl_plat_boot(egl_platform_t *plat, unsigned int slot_idx)
{
    EGL_ASSERT_CHECK(plat);
    EGL_ASSERT_CHECK(plat->boot);

    return plat->boot(slot_idx);
}

egl_result_t egl_plat_reboot(egl_platform_t *plat)
{
    EGL_ASSERT_CHECK(plat);
    EGL_ASSERT_CHECK(plat->reboot);

    return plat->reboot();
}

egl_result_t egl_plat_shutdown(egl_platform_t *plat)
{
    EGL_ASSERT_CHECK(plat);
    EGL_ASSERT_CHECK(plat->shutdown);

    return plat->shutdown();
}

char *egl_plat_info(egl_platform_t *plat)
{
    if(plat == NULL || plat->info == NULL)
    {
        return NULL;
    }

    return plat->info();
}

uint32_t egl_plat_clock(egl_platform_t *plat)
{
    if(plat == NULL || plat->clock == NULL)
    {
        return 0;
    }

    return plat->clock();
}