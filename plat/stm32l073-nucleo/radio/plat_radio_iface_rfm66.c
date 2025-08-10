#include "egl_lib.h"
#include "plat.h"
#include "plat_radio_tx_led.h"
#include "plat_radio_rx_led.h"

extern egl_rfm66_t plat_rfm66_inst;

static egl_rfm66_iface_t rfm66_iface_inst =
{
    .rfm = &plat_rfm66_inst,
    .pm_wait = PLAT_SYSPM_RUN,
    .rx_timeout = 1000,
    .tx_timeout = 1000
};

static egl_result_t init(void)
{
    egl_result_t result;

    static const uint8_t sync[] = { 0x91, 0xD3 };
    const egl_rfm66_config_t config =
    {
        .frequency = RADIO_FREQ,
        .deviation = RADIO_DEVIATION,
        .bitrate   = RADIO_BITRATE,
        .preamble  = RADIO_PREAMBLE,
        .bandwidth = EGL_RFM66_BANDWIDTH_100000,
        .node_addr = RADIO_NODE_ADDR,
        .sync      = sync,
        .sync_size = sizeof(sync),
        .power     = RADIO_POWER_DB
    };

    result = egl_pio_init(RADIO_TX_LED);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(RADIO_RX_LED);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_iface_init(&rfm66_iface_inst, (egl_rfm66_config_t *)&config);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t write(void *data, size_t *len)
{
    egl_result_t result;

    result = egl_pio_set(RADIO_TX_LED, true);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_iface_write(&rfm66_iface_inst, data, len);
    EGL_RESULT_CHECK(result);

    result = egl_pio_set(RADIO_TX_LED, false);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t read(void *data, size_t *len)
{
    egl_result_t result;

    result = egl_pio_set(RADIO_RX_LED, false);
    EGL_RESULT_CHECK(result);

    result = egl_rfm66_iface_read(&rfm66_iface_inst, data, len);
    EGL_RESULT_CHECK(result);

    result = egl_pio_set(RADIO_RX_LED, true);
    EGL_RESULT_CHECK(result);

    return result;
}

static const egl_iface_t plat_radio_iface_inst =
{
    .init = init,
    .write = write,
    .read = read
};

egl_iface_t *plat_radio_iface_get(void)
{
    return (egl_iface_t *)&plat_radio_iface_inst;
}
