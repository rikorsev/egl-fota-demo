#include <string.h>

#include "egl_lib.h"
#include "plat.h"

enum
{
    CMD_NONE,
    CMD_LED_OFF,
    CMD_LED_ON,
    CMD_UPLOAD_SLOT_A,
    CMD_UPLOAD_SLOT_B
};

static bool is_recv = false;
static bool rm_led_state = false;
static uint8_t cmd = CMD_NONE;

static egl_result_t error_handler_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static void user_button_callback(void *data)
{
    cmd = rm_led_state ? CMD_LED_OFF : CMD_LED_ON;
    rm_led_state = !rm_led_state;
}

static void radio_sw1_callback(void *data)
{
    cmd = CMD_UPLOAD_SLOT_A;
}

static void radio_sw2_callback(void *data)
{
    cmd = CMD_UPLOAD_SLOT_B;
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

    result = egl_iface_ioctl(RADIO, RADIO_IOCTL_RX_MODE_SET, NULL, &len);
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

static egl_result_t cmd_handle(uint8_t *cmd)
{
    egl_result_t result;
    size_t len = sizeof(*cmd);

    switch(*cmd)
    {
        case CMD_NONE:
            result = EGL_SUCCESS;
            break;

        case CMD_LED_OFF:
        case CMD_LED_ON:
            result = egl_iface_write(RADIO, cmd, &len);
            EGL_RESULT_CHECK_EXIT(result);
            break;

        case CMD_UPLOAD_SLOT_A:
        case CMD_UPLOAD_SLOT_B:
            result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, (void *)PLAT_SLOT_BOOT, NULL);
            EGL_RESULT_CHECK_EXIT(result);
            break;
    }

exit:
    /* Reset command */
    *cmd = CMD_NONE;

    return result;
}

static egl_result_t radio_recv(void)
{
    egl_result_t result;
    uint8_t recv_cmd;
    size_t len = sizeof(recv_cmd);

    result = egl_iface_read(RADIO, &recv_cmd, &len);
    EGL_RESULT_CHECK(result);

    switch(recv_cmd)
    {
        case CMD_LED_OFF:
            result = egl_pio_set(SYSLED, false);
            EGL_RESULT_CHECK(result);
            break;

        case CMD_LED_ON:
            result = egl_pio_set(SYSLED, true);
            EGL_RESULT_CHECK(result);
            break;

        case CMD_UPLOAD_SLOT_A:
            result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, (void *)PLAT_SLOT_BOOT, NULL);
            EGL_RESULT_CHECK(result);
            break;

        case CMD_UPLOAD_SLOT_B:
            result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, (void *)PLAT_SLOT_BOOT, NULL);
            EGL_RESULT_CHECK(result);
            break;

        default:
            EGL_LOG_WARN("Unknown command: %u", recv_cmd);
    }

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

    result = cmd_handle(&cmd);
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