#include <string.h>

#include "egl_lib.h"
#include "plat.h"

static bool is_send = false;
static bool is_recv = false;
static unsigned int size_index = 0;
static const size_t size_table[] = { 16, 32, 64, 254, 4096 };
static uint8_t send_buff[4096] = {0};

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
    size_index++;
    size_index %= EGL_ARRAY_SIZE(size_table);

    EGL_LOG_INFO("TX packet size: %u", size_table[size_index]);
}

static void radio_sw2_callback(void *data)
{
    size_index--;
    size_index %= EGL_ARRAY_SIZE(size_table);

    EGL_LOG_INFO("TX packet size: %u", size_table[size_index]);
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

    /* init send buffer */
    for(unsigned int i= 0; i < sizeof(send_buff); i++)
    {
        send_buff[i] = i & 0xFF;
    }

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

static void radio_send(void)
{
    EGL_LOG_INFO("Sending...");

    size_t len = size_table[size_index];

    egl_result_t result = egl_iface_write(RADIO, (void *)send_buff, &len);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_INFO, "send",  (void *)send_buff, len, 8);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

static void radio_recv(void)
{
    static uint8_t buff[4096] = {0};
    egl_result_t result;
    size_t len = sizeof(buff);

    result = egl_iface_read(RADIO, buff, &len);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_INFO, "recv", buff, len, 8);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);

    /* Buffer content not needed any more, so clear it */
    memset(buff, 0, sizeof(buff));
}

void loop(void)
{
    if(is_recv)
    {
        radio_recv();
        is_recv = false;
    }

    if(is_send)
    {
        radio_send();
        is_send = false;
    }
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
        loop();
    }

    return 0;
}