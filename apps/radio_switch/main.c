#include <string.h>

#include "egl_lib.h"
#include "plat.h"

typedef struct __attribute__((packed))
{
    uint8_t cmd;
    uint8_t len;
    uint8_t payload;
}packet_t;

enum
{
    REMOTE_LED_OFF,
    REMOTE_LED_ON
};

static bool is_send = false;
static bool is_recv = false;
static bool rm_led_state = false;

static egl_result_t error_handler_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static void user_button_callback(void *data)
{
    is_send = true;
}

static void radio_sw1_callback(void *data)
{
    /* TBD */
}

static void radio_sw2_callback(void *data)
{
    /* TBD */
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

static egl_result_t radio_send(void)
{
    packet_t packet = { .cmd = rm_led_state ? REMOTE_LED_OFF : REMOTE_LED_ON };
    size_t len = sizeof(packet);

    egl_result_t result = egl_iface_write(RADIO, &packet, &len);
    EGL_RESULT_CHECK(result);

    rm_led_state = !rm_led_state;

    EGL_LOG_INFO("Send remote LED State: %s", rm_led_state ? "ON" : "OFF");

    return result;
}

static egl_result_t radio_recv(void)
{
    egl_result_t result;
    packet_t packet;
    size_t len = sizeof(packet);

    result = egl_iface_read(RADIO, &packet, &len);
    EGL_RESULT_CHECK(result);

    switch(packet.cmd)
    {
        case REMOTE_LED_OFF:
            result = egl_pio_set(SYSLED, false);
            EGL_RESULT_CHECK(result);
            break;

        case REMOTE_LED_ON:
            result = egl_pio_set(SYSLED, true);
            EGL_RESULT_CHECK(result);
            break;

        default:
            EGL_LOG_WARN("Unknown command: %u", packet.cmd);
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

    if(is_send)
    {
        is_send = false;

        result = radio_send();
        EGL_RESULT_CHECK(result);
    }

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