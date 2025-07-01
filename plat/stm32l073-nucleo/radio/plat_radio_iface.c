#include "egl_lib.h"
#include "plat.h"

#if CONFIG_APP_TARGET_RFM_69
egl_interface_t *plat_radio_iface_get(void)
{
    return NULL;
}

#elif CONFIG_APP_TARGET_RFM_66
static egl_result_t init(void)
{
    egl_result_t result;

    static const egl_rfm66_config_t config =
    {
        .frequency = 915000000,
        .deviation = 30000,
        .bitrate = 115200,
        .preamble = 16,
        .bandwidth = EGL_RFM66_BANDWIDTH_100000,
    };

    result = egl_rfm66_iface_init(PLAT_RFM66, (egl_rfm66_config_t *)&config);
    EGL_RESULT_CHECK(result);

    return result;
}

static size_t write(void *data, size_t len)
{
    return egl_rfm66_iface_write(PLAT_RFM66, data, len);
}

static const egl_interface_t plat_radio_iface_inst =
{
    .init = init,
    .write = write,
};

egl_interface_t *plat_radio_iface_get(void)
{
    return (egl_interface_t *)&plat_radio_iface_inst;
}

#endif