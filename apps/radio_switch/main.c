#include <string.h>

#include "egl_lib.h"
#include "plat.h"
#include "protocol.h"

enum
{
    EVENT_NONE,
    EVENT_LED_OFF,
    EVENT_LED_ON,
    EVENT_UPLOAD_SLOT_A,
    EVENT_UPLOAD_SLOT_B,
    EVENT_DOWNLOAD_SLOT_A,
    EVENT_DOWNLOAD_SLOT_B
};

static bool is_recv = false;
static bool rm_led_state = false;
static uint8_t event = EVENT_NONE;

static egl_result_t error_handler_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static void user_button_callback(void *data)
{
    event = rm_led_state ? EVENT_LED_OFF : EVENT_LED_ON;
    rm_led_state = !rm_led_state;
}

static void radio_sw1_callback(void *data)
{
    event = EVENT_UPLOAD_SLOT_A;
}

static void radio_sw2_callback(void *data)
{
    event = EVENT_UPLOAD_SLOT_B;
}

static void radio_rts_callback(void *data)
{
    is_recv = true;
}

static egl_result_t init(void)
{
    egl_result_t result;
    static egl_result_error_handler_t error_handler = { error_handler_func };

    result = egl_system_init(SYSTEM);
    EGL_RESULT_CHECK(result);

    egl_result_error_handler_set(&error_handler);

    result = egl_iface_init(RADIO);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(SYSLED);
    EGL_RESULT_CHECK(result);

    result = egl_pio_set(SYSLED, false);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(USER_BUTTON);
    EGL_RESULT_CHECK(result);

    result = egl_pio_callback_set(USER_BUTTON, user_button_callback);
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

    return result;
}

static egl_result_t info(void)
{
    egl_result_t result = EGL_SUCCESS;

#if CONFIG_EGL_LOG_ENABLED
    app_info_t *info = NULL;
    result = egl_value_ptr_get(APP_INFO, (void **)&info);
    EGL_RESULT_CHECK(result);
    EGL_ASSERT_CHECK(info, EGL_NOT_SUPPORTED);

    EGL_LOG_INFO("Application %s (%u.%u.%u%s) started", info->name,
                                                        info->version.major,
                                                        info->version.minor,
                                                        info->version.revision,
                                                        info->version.sufix);
    EGL_LOG_INFO("Date: %s", info->buildtime);
    EGL_LOG_INFO("Size: %u", info->size);
    EGL_LOG_INFO("Checksum: %08x", info->checksum);
#endif

    return result;
}

static egl_result_t event_switch_handle(bool target_state)
{
    egl_result_t result;
    uint8_t target_state_value = target_state ? 1 : 0;

    PROTOCOL_PACKET_DECLARE(packet, sizeof(target_state_value));
    size_t len = sizeof(packet);

    result = protocol_packet_build((protocol_packet_t *)packet,
                                   PROTOCOL_CMD_SWITCH,
                                   &target_state_value, sizeof(target_state_value));
    EGL_RESULT_CHECK(result);

    result = egl_iface_write(RADIO, packet, &len);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t event_handle(uint8_t *event)
{
    egl_result_t result;
    plat_boot_config_t boot_config = { .slot = PLAT_SLOT_BOOT };

    switch(*event)
    {
        case EVENT_NONE:
            result = EGL_SUCCESS;
            break;

        case EVENT_LED_OFF:
        case EVENT_LED_ON:
            result = event_switch_handle(*event == EVENT_LED_ON);
            break;

        case EVENT_UPLOAD_SLOT_A:
            boot_config.task = PLAT_FOTA_TASK_UPLOAD_SLOT_A;
            result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, &boot_config, NULL);
            break;

        case EVENT_UPLOAD_SLOT_B:
            result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, &boot_config, NULL);
            break;
    }

    /* Reset current command */
    *event = EVENT_NONE;

    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t switch_handle(protocol_packet_t *packet)
{
    return egl_pio_set(SYSLED, packet->payload[0]);
}

static egl_result_t request_to_download_handle(protocol_packet_t *packet)
{
    egl_result_t result;
    plat_boot_config_t boot_config = { .slot = PLAT_SLOT_BOOT };
    plat_boot_slot_t target_slot = packet->payload[0];

    switch(target_slot)
    {
        case PLAT_SLOT_A:
            boot_config.task = PLAT_FOTA_TASK_DOWNLOAD_SLOT_A;
            break;

        case PLAT_SLOT_B:
            boot_config.task = PLAT_FOTA_TASK_DOWNLOAD_SLOT_B;
            break;

        default:
            return EGL_NOT_SUPPORTED;
    }

    result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, &boot_config, NULL);
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t radio_recv(void)
{
    egl_result_t result;
    uint8_t packet_buff[16];
    protocol_packet_t *packet_ptr = (protocol_packet_t *)packet_buff;
    size_t len = sizeof(packet_buff);

    result = egl_iface_read(RADIO, packet_buff, &len);
    EGL_RESULT_CHECK(result);

    result = protocol_packet_validate(packet_ptr);
    EGL_RESULT_CHECK(result);

    switch(packet_ptr->cmd)
    {
        case PROTOCOL_CMD_SWITCH:
            result = switch_handle(packet_ptr);
            break;

        case PROTOCOL_CMD_REQUEST_TO_DOWNLOAD:
            result = request_to_download_handle(packet_ptr);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    return result;
}

egl_result_t loop(void)
{
    egl_result_t result = EGL_SUCCESS;

    if(is_recv)
    {
        is_recv = false;

        result = radio_recv();
        EGL_RESULT_CHECK(result);
    }

    result = event_handle(&event);
    EGL_RESULT_CHECK(result);

    return result;
}

int main(void)
{
    egl_result_t result;

    result = init();
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, 0);

    result = info();
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, 0);

    while(1)
    {
        result = loop();
        if(result != EGL_SUCCESS)
        {
            EGL_LOG_WARN("loop - fail. Result: %s", EGL_RESULT(result));
        }
    }

    return 0;
}