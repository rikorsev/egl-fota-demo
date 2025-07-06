#include "egl_lib.h"
#include "plat.h"

static egl_result_t error_hook_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static egl_result_t init(void)
{
    egl_result_t result;
    static egl_result_error_hook_t error_hook = { error_hook_func };

    result = egl_system_init(SYSTEM);
    EGL_RESULT_CHECK(result);

    egl_result_error_hook_set(&error_hook);

    result = egl_itf_init(RADIO);
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
    uint8_t buff[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    size_t len = sizeof(buff);
    egl_result_t result = egl_itf_write(RADIO, buff, &len);
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_FAIL("Ping fail. Result: %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }
}

static void radio_scan(void)
{
    size_t len;
    uint8_t buff[64] = { 0 };
    egl_result_t result = egl_itf_read(RADIO, buff, &len);
    if(result == EGL_SUCCESS)
    {
        EGL_LOG_INFO("Received(%u):", len);
        for(unsigned int i = 0; i < len; i += 8)
        {
            EGL_LOG_INFO("%02x %02x %02x %02x %02x %02x %02x %02x", buff[i],
                                                                    buff[i + 1],
                                                                    buff[i + 2],
                                                                    buff[i + 3],
                                                                    buff[i + 4],
                                                                    buff[i + 5],
                                                                    buff[i + 6],
                                                                    buff[i + 7]);
        }
    }
    else
    {
        EGL_LOG_WARN("Scan fail. Result: %s", EGL_RESULT(result));
    }
}

int main(void)
{
    egl_result_t result = init();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_FAIL("Fatal error");
        EGL_RESULT_FATAL();
    }

    result = info();
    if(result != EGL_SUCCESS)
    {
        EGL_LOG_FAIL("Fatal error");
        EGL_RESULT_FATAL();
    }

    while(1)
    {
        EGL_LOG_INFO("Tick...");
        // radio_ping();
        radio_scan();
        // egl_sys_delay(1000);
    }

    return 0;
}