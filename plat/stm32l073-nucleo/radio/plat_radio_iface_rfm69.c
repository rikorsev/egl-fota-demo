#include "egl_lib.h"
#include "plat.h"
#include "plat_radio_tx_led.h"
#include "plat_radio_rx_led.h"

static egl_result_t init(void)
{
    egl_result_t result;

    const egl_rfm69_config_t config =
    {
        .frequency = RADIO_FREQ,
        .deviation = RADIO_DEVIATION,
        .bitrate   = RADIO_BITRATE,
        .preamble  = RADIO_PREAMBLE,
        .bandwidth = EGL_RFM69_BANDWIDTH_FSK_100000_OOK_50000,
        .node_addr = RADIO_NODE_ADDR,
        .sync      = RADIO_SYNC,
        .sync_size = RADIO_SYNC_SIZE,
        .power     = RADIO_POWER_DB
    };

    result = egl_pio_init(RADIO_TX_LED);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(RADIO_RX_LED);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_iface_init(PLAT_RFM69, (egl_rfm69_config_t *)&config);
    EGL_RESULT_CHECK(result);

    return result;
}

static size_t write(void *data, size_t len)
{
    egl_result_t result;

    result = egl_pio_set(RADIO_TX_LED, true);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, 0);

    len = egl_rfm69_iface_write(PLAT_RFM69, data, len);

    result = egl_pio_set(RADIO_TX_LED, false);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, 0);

    return len;
}

static size_t read(void *data, size_t len)
{
    egl_result_t result;

    result = egl_pio_set(RADIO_RX_LED, true);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, 0);

    len = egl_rfm69_iface_read(PLAT_RFM69, data, &len);

    result = egl_pio_set(RADIO_RX_LED, false);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, 0);

    return len;
}

static const egl_interface_t plat_radio_iface_inst =
{
    .init = init,
    .write = write,
    .read = read
};

egl_interface_t *plat_radio_iface_get(void)
{
    return (egl_interface_t *)&plat_radio_iface_inst;
}
