#include "egl_lib.h"
#include "protocol.h"
#include "plat.h"
#include "switch.h"
#include "radio.h"

#define TASK_CLEAR(__task) (g_task &= ~__task)

static radio_task_t g_task;

static egl_result_t radio_task_switch_cmd_handle(protocol_packet_t *packet)
{
    egl_result_t result;

    result = protocol_packet_validate(packet, PROTOCOL_CMD_SWITCH, 0);
    EGL_RESULT_CHECK(result);

    result = switch_task_set(SWITCH_TASK_LOCAL_STATE_TOGGLE);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t radio_task_recv_handle(void)
{
    egl_result_t result;
    uint8_t packet_buff[16];
    protocol_packet_t *packet_ptr = (protocol_packet_t *)packet_buff;
    size_t len = sizeof(packet_buff);

    result = egl_iface_read(RADIO, packet_buff, &len);
    EGL_RESULT_CHECK(result);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_DEBUG, "recv", packet_buff, len, 8);
    EGL_RESULT_CHECK(result);

    switch(packet_ptr->cmd)
    {
        case PROTOCOL_CMD_SWITCH:
            result = radio_task_switch_cmd_handle(packet_ptr);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t radio_task_led_toggle_send_handle(void)
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

egl_result_t radio_task_set(radio_task_t task)
{
    g_task |= task;

    return EGL_SUCCESS;
}

egl_result_t radio_task(void)
{
    egl_result_t result = EGL_SUCCESS;

    while(g_task)
    {
        if(g_task & RADIO_TASK_RECV)
        {
            EGL_LOG_DEBUG("handle: RADIO_TASK_RECV");
            TASK_CLEAR(RADIO_TASK_RECV);
            result = radio_task_recv_handle();
            EGL_RESULT_CHECK(result);
        }

        if(g_task & RADIO_TASK_LED_TOGGLE_SEND)
        {
            EGL_LOG_DEBUG("handle: RADIO_TASK_LED_TOGGLE_SEND");
            TASK_CLEAR(RADIO_TASK_LED_TOGGLE_SEND);
            result = radio_task_led_toggle_send_handle();
            EGL_RESULT_CHECK(result);
        }
    }

    return result;
}