#include "egl_lib.h"
#include "protocol.h"
#include "plat.h"
#include "switch.h"
#include "radio.h"
#include "fota.h"

#define RADIO_PAYLOAD_MAX (PLAT_FLASH_PAGE_SIZE + 8U) /* +8 bytes for meta data */

static void *thread_handle = NULL;
static void *mux_handle = NULL;
static void *sem_handle = NULL;
static radio_packet_recv_handler_func_t recv_handler = NULL;

static void radio_sw1_callback(void *data)
{
    egl_result_t result;
    result = fota_start();
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

static void radio_sw2_callback(void *data)
{

}

static void radio_rts_callback(void *data)
{
    egl_result_t result;
    result = egl_os_sem_post(SYSOS, sem_handle);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

static egl_result_t radio_recv_handle(void)
{
    egl_result_t result;
    static PROTOCOL_PACKET_DECLARE(packet, RADIO_PAYLOAD_MAX);
    size_t len = sizeof(packet_buff);
    int8_t rssi;
    int16_t freq_error;

    result = egl_os_mux_lock(SYSOS, mux_handle, EGL_OS_WAIT_FOREWER);
    EGL_RESULT_CHECK(result);

    result = egl_iface_read(RADIO, packet_buff, &len);
    EGL_RESULT_CHECK_EXIT(result);

    result = egl_iface_ioctl(RADIO, RADIO_IOCTL_RSSI_GET, &rssi, NULL);
    EGL_RESULT_CHECK_EXIT(result);

    result = egl_iface_ioctl(RADIO, RADIO_IOCTL_FREQ_ERROR_GET, &freq_error, NULL);
    EGL_RESULT_CHECK_EXIT(result);

    EGL_LOG_DEBUG("RSSI: %d, freq_error: %d", rssi, freq_error);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "recv", packet_buff, len, 8);
    EGL_RESULT_CHECK_EXIT(result);

    result = recv_handler(packet);
    EGL_RESULT_CHECK_EXIT(result);

exit:
    egl_os_mux_unlock(SYSOS, mux_handle);

    return result;
}

static void radio_thread_entry(void *param)
{
    egl_result_t result;

    while(1)
    {
        result = egl_os_sem_wait(SYSOS, sem_handle, EGL_OS_WAIT_FOREWER);
        EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);

        result = radio_recv_handle();
        if(result != EGL_SUCCESS)
        {
            EGL_LOG_WARN("Fail to handle receive. Result: %s", EGL_RESULT(result));
        }
    }
}

egl_result_t radio_packet_send(protocol_packet_t *packet)
{
    egl_result_t result;
    size_t packet_len = PROTOCOL_PACKET_SIZE(packet);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "send", (uint8_t *)packet, packet_len, 8);
    EGL_RESULT_CHECK(result);

    result = egl_os_mux_lock(SYSOS, mux_handle, EGL_OS_WAIT_FOREWER);
    EGL_RESULT_CHECK(result);

    result = egl_iface_write(RADIO, packet, &packet_len);
    EGL_RESULT_CHECK_EXIT(result);

exit:
    egl_os_mux_unlock(SYSOS, mux_handle);

    return result;
}

egl_result_t radio_packet_recv_notify(void)
{
    return egl_os_sem_post(SYSOS, sem_handle);
}

egl_result_t radio_init(radio_packet_recv_handler_func_t handler)
{
    EGL_ASSERT_CHECK(handler, EGL_NULL_POINTER);

    egl_result_t result;

    static egl_os_thread_ctx thread_ctx;
    static egl_os_mux_ctx mux_ctx;
    static egl_os_sem_ctx sem_ctx;
    static uint8_t stack[2048];

    recv_handler = handler;

    result = egl_iface_init(RADIO);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(RADIO_SW1);
    EGL_RESULT_CHECK(result);

    result = egl_pio_callback_set(RADIO_SW1, radio_sw1_callback);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(RADIO_SW2);
    EGL_RESULT_CHECK(result);

    result = egl_pio_callback_set(RADIO_SW2, radio_sw2_callback);
    EGL_RESULT_CHECK(result);

    size_t len = sizeof(radio_rts_callback);
    result = egl_iface_ioctl(RADIO, RADIO_IOCTL_RTS_CALLBACK_SET, radio_rts_callback, &len);
    EGL_RESULT_CHECK(result);

    result = egl_iface_ioctl(RADIO, RADIO_IOCTL_RX_MODE_SET, NULL, NULL);
    EGL_RESULT_CHECK(result);

    result = egl_os_mux_create(SYSOS, &mux_handle, "radio_mux", EGL_OS_MUX_TYPE_REGULAR, &mux_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_sem_create(SYSOS, &sem_handle, "radio_sem", 1, 0, &sem_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_thread_create(SYSOS, &thread_handle, "radio",
                                  radio_thread_entry, NULL,
                                  stack, sizeof(stack),
                                  1, &thread_ctx);
    EGL_RESULT_CHECK(result);

    return result;
}