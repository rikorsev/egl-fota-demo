#include "egl_lib.h"
#include "plat.h"

#if CONFIG_APP_TARGET_RFM_69

#elif CONFIG_APP_TARGET_RFM_66
static egl_result_t init(void)
{
    egl_result_t result;

    static const egl_rfm66_config_t config =
    {
        .frequency = 915000000,
        .bandwith = 83300,
        .deviation = 30000,
        .bitrate = 115200,
    };

    result = egl_rfm66_iface_init(PLAT_RFM66, (egl_rfm66_config_t *)&config);
    EGL_RESULT_CHECK(result);

    return result;
}

static const egl_interface_t plat_radio_iface_inst =
{
    .init = init,
};

egl_interface_t *plat_radio_iface_get(void)
{
    return (egl_interface_t *)&plat_radio_iface_inst;
}

#endif