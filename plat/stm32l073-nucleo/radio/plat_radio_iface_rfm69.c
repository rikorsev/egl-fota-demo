#include "egl_lib.h"
#include "plat.h"
#include "plat_radio_tx_led.h"
#include "plat_radio_rx_led.h"

extern egl_rfm69_t plat_rfm69_inst;

static void(*rts_callback)(void *) = NULL;
static bool is_writing = false;
static egl_rfm69_iface_t rfm69_iface_inst =
{
    .rfm                   = &plat_rfm69_inst,
    .pm_wait               = PLAT_SYSPM_RUN,
    .rx_timeout            = 100,
    .tx_timeout            = 1000,
    .rx_exit_mode          = EGL_RFM69_RX_MODE,
    .tx_exit_mode          = EGL_RFM69_RX_MODE,
    .is_rx_partial         = true,
    .is_rx_inc_tout        = true,
    .tx_inter_packet_delay = RADIO_TX_INTER_PACKET_DELAY
};

static egl_result_t plat_radio_iface_ioctl_rts_callback_set(void *data, size_t *len)
{
    rts_callback = data;

    return EGL_SUCCESS;
}

static void plat_radio_iface_rts_callback(void *data)
{
    if(is_writing != true && rts_callback != NULL)
    {
        rts_callback(data);
    }
}

static egl_result_t init(void)
{
    egl_result_t result;

    const egl_rfm69_config_t config =
    {
        .frequency   = RADIO_FREQ,
        .deviation   = RADIO_DEVIATION,
        .bitrate     = RADIO_BITRATE,
        .preamble    = RADIO_PREAMBLE,
        .bandwidth   = EGL_RFM69_BANDWIDTH_FSK_200000_OOK_100000,
        .node_addr   = RADIO_NODE_ADDR,
        .sync        = RADIO_SYNC,
        .sync_size   = sizeof(RADIO_SYNC),
        .power       = RADIO_POWER_DB,
        .rssi_thresh = RADIO_RSSI_THRESH
    };

    result = egl_pio_init(RADIO_TX_LED);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(RADIO_RX_LED);
    EGL_RESULT_CHECK(result);

    result = egl_pio_callback_set(rfm69_iface_inst.rfm->dio2, plat_radio_iface_rts_callback);
    EGL_RESULT_CHECK(result);

    result = egl_rfm69_iface_init(&rfm69_iface_inst, (egl_rfm69_config_t *)&config);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t write(void *data, size_t *len)
{
    egl_result_t result;
    egl_result_t result2;

    is_writing = true;

    result = egl_pio_set(RADIO_TX_LED, true);
    EGL_RESULT_CHECK_EXIT(result);

    result = egl_rfm69_iface_write(&rfm69_iface_inst, data, len);
    EGL_RESULT_CHECK_EXIT(result);

exit:
    is_writing = false;

    result2 = egl_pio_set(RADIO_TX_LED, false);
    EGL_RESULT_CHECK(result2);

    return result;
}

static egl_result_t read(void *data, size_t *len)
{
    egl_result_t result;
    egl_result_t result2;

    result = egl_pio_set(RADIO_RX_LED, true);
    EGL_RESULT_CHECK_EXIT(result);

    result = egl_rfm69_iface_read(&rfm69_iface_inst, data, len);
    EGL_RESULT_CHECK_EXIT(result);

exit:
    result2 = egl_pio_set(RADIO_RX_LED, false);
    EGL_RESULT_CHECK(result2);

    return result;
}

static egl_result_t ioctl(uint8_t opcode, void *data, size_t *len)
{
    egl_result_t result;

    switch(opcode)
    {
        case RADIO_IOCTL_RTS_CALLBACK_SET:
            result = plat_radio_iface_ioctl_rts_callback_set(data, len);
            EGL_RESULT_CHECK(result);
            break;

        case RADIO_IOCTL_RX_MODE_SET:
            result = egl_rfm69_iface_ioctl(&rfm69_iface_inst, EGL_RFM69_IOCTL_RX_MODE_SET, data, len);
            EGL_RESULT_CHECK(result);
            break;

        case RADIO_IOCTL_RX_TIMEOUT_SET:
            result = egl_rfm69_iface_ioctl(&rfm69_iface_inst, EGL_RFM69_IOCTL_RX_TIMEOUT_SET, data, len);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }

    return result;
}

static const egl_iface_t plat_radio_iface_inst =
{
    .init = init,
    .write = write,
    .read = read,
    .ioctl = ioctl
};

egl_iface_t *plat_radio_iface_get(void)
{
    return (egl_iface_t *)&plat_radio_iface_inst;
}
