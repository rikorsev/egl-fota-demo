#include "egl_lib.h"
#include "plat.h"

bool is_ping = false;

static egl_result_t error_hook_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static void user_button_callback(void *data)
{
    is_ping = true;
}

static egl_result_t init(void)
{
    egl_result_t result;
    static egl_result_error_hook_t error_hook = { error_hook_func };

    result = egl_system_init(SYSTEM);
    EGL_RESULT_CHECK(result);

    egl_result_error_hook_set(&error_hook);

    result = egl_iface_init(RADIO);
    EGL_RESULT_CHECK(result);

    result = egl_pio_init(USER_BUTTON);
    EGL_RESULT_CHECK(result);

    result = egl_pio_callback_set(USER_BUTTON, user_button_callback);
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

static void radio_ping(void)
{
    EGL_LOG_INFO("Ping...");

    uint8_t buff[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x11, 0x22, 0x33, 0x44 };
    size_t len = sizeof(buff);
    egl_result_t result = egl_iface_write(RADIO, buff, &len);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

static void radio_scan(void)
{
    size_t len;
    uint8_t buff[64] = { 0 };
    egl_result_t result;

    result = egl_iface_read(RADIO, buff, &len);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);

    result = egl_log_buff(SYSLOG, EGL_LOG_LEVEL_INFO, "received", buff, len, 8);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

void loop(void)
{
    EGL_LOG_INFO("Tick...");

    if(is_ping)
    {
        is_ping = false;
        radio_ping();
    }

    radio_scan();
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