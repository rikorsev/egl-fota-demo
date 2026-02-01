#include "egl_lib.h"
#include "protocol.h"
#include "plat.h"
#include "switch.h"
#include "radio.h"

static void *thread_handle = NULL;
static void *flags_handle = NULL;

static void radio_sw1_callback(void *data)
{

}

static void radio_sw2_callback(void *data)
{
    egl_result_t result;
    result = radio_flag_set(RADIO_TEST_CMD_SEND_FLAG);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

static void radio_rts_callback(void *data)
{
    egl_result_t result;
    result = radio_flag_set(RADIO_RECV_FLAG);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

static egl_result_t radio_switch_cmd_handle(protocol_packet_t *packet)
{
    egl_result_t result;

    result = protocol_packet_validate(packet, PROTOCOL_CMD_SWITCH, 0);
    EGL_RESULT_CHECK(result);

    result = switch_flag_set(SWITCH_TOGGLE_FLAG);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t radio_recv_handle(void)
{
    egl_result_t result;
    static PROTOCOL_PACKET_DECLARE(packet, 4096);
    size_t len = sizeof(packet_buff);

    result = egl_iface_read(RADIO, packet_buff, &len);
    EGL_RESULT_CHECK(result);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "recv", packet_buff, len, 8);
    EGL_RESULT_CHECK(result);

    switch(packet->cmd)
    {
        case PROTOCOL_CMD_SWITCH:
            result = radio_switch_cmd_handle(packet);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t radio_led_toggle_send_handle(void)
{
    egl_result_t result;

    PROTOCOL_PACKET_DECLARE(packet, 0);
    size_t len = sizeof(packet_buff);

    result = protocol_packet_build((protocol_packet_t *)packet, PROTOCOL_CMD_SWITCH, NULL, 0);
    EGL_RESULT_CHECK(result);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "send", packet_buff, sizeof(packet_buff), 8);
    EGL_RESULT_CHECK(result);

    result = egl_iface_write(RADIO, packet, &len);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t radio_test_cmd_send_handle(void)
{
    #define TEST_COMMAND_PAYLOAD_LEN (128U)

    egl_result_t result;

    PROTOCOL_PACKET_DECLARE(packet, 128);
    size_t len = sizeof(packet_buff);

    for(unsigned int i = 0; i < 128; i++)
    {
        packet->payload[i] = i;
    }

    result = protocol_packet_build((protocol_packet_t *)packet, PROTOCOL_CMD_SWITCH, NULL, 128);
    EGL_RESULT_CHECK(result);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "send", packet_buff, sizeof(packet_buff), 8);
    EGL_RESULT_CHECK(result);

    result = egl_iface_write(RADIO, packet, &len);
    EGL_RESULT_CHECK(result);

    return result;
}

static void radio_thread_entry(void *param)
{
    egl_result_t result;
    unsigned int flags = 0;

    while(1)
    {
        result = egl_os_flags_wait(SYSOS, flags_handle, RADIO_ALL_FLAGS, &flags,
                                   EGL_OS_FLAGS_OPT_WAIT_ANY, EGL_OS_WAIT_FOREWER);
        if(result != EGL_SUCCESS)
        {
            EGL_LOG_WARN("Wait for events - fail. Result: %s", EGL_RESULT(result));

            continue;
        }

        if(flags & RADIO_RECV_FLAG)
        {
            result = radio_recv_handle();
            if(result != EGL_SUCCESS)
            {
                EGL_LOG_WARN("Receive command - fail. Result: %s", EGL_RESULT(result));
            }
        }

        if(flags & RADIO_LED_TOGGLE_SEND_FLAG)
        {
            result = radio_led_toggle_send_handle();
            if(result != EGL_SUCCESS)
            {
                EGL_LOG_WARN("Send LED toggle - fail. Result: %s", EGL_RESULT(result));
            }
        }

        if(flags & RADIO_TEST_CMD_SEND_FLAG)
        {
            result = radio_test_cmd_send_handle();
            if(result != EGL_SUCCESS)
            {
                EGL_LOG_WARN("Send test command - fail. Result: %s", EGL_RESULT(result));
            }
        }
    }
}

egl_result_t radio_flag_set(unsigned int flag)
{
    egl_result_t result;

    result = egl_os_flags_set(SYSOS, flags_handle, flag);
    EGL_RESULT_CHECK(result);

    return EGL_SUCCESS;
}

egl_result_t radio_init(void)
{
    egl_result_t result;

    static egl_os_flags_ctx flags_ctx;
    static egl_os_thread_ctx thread_ctx;
    static uint8_t stack[1024];

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

    result = egl_iface_ioctl(RADIO, RADIO_IOCTL_RX_TIMEOUT_SET, (void *)100, NULL);
    EGL_RESULT_CHECK(result);

    result = egl_os_flags_create(SYSOS, &flags_handle, "RadioFlags", &flags_ctx);
    EGL_RESULT_CHECK(result);

    result = egl_os_thread_create(SYSOS, &thread_handle, "Radio",
                                  radio_thread_entry, NULL,
                                  stack, sizeof(stack),
                                  1, &thread_ctx);

    return result;
}