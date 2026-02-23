#include <string.h>

#include "egl_lib.h"
#include "plat.h"
#include "radio.h"
#include "fota.h"
#include "switch.h"
#include "protocol.h"

#define APP_CRC_POLY ((uint32_t)0x4C11DB7)
#define APP_CRC_INIT ((uint32_t)0xFFFFFFFF)

static egl_result_t error_handler_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static void user_button_callback(void *data)
{
    egl_result_t result;
    result = switch_flag_set(SWITCH_TOGGLE_SEND_FLAG);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, RETURN_VOID);
}

static egl_result_t recv_handler(protocol_packet_t *packet)
{
    egl_result_t result;

    switch(packet->cmd)
    {
        case PROTOCOL_CMD_SWITCH:
            result = switch_toggle_recv_cmd_handle(packet);
            break;

        case PROTOCOL_CMD_FOTA_REQUEST_CHUNK:
            result = fota_request_chunk_handle(packet);
            break;

        case PROTOCOL_CMD_FOTA_COMPLETE:
            result = fota_complete_handle(packet);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    return result;
}

static egl_result_t init(void)
{
    egl_result_t result;
    static egl_result_error_handler_t error_handler = { error_handler_func };

    result = egl_system_init(SYSTEM);
    EGL_RESULT_CHECK(result);

    egl_result_error_handler_set(&error_handler);

    result = egl_pio_init(USER_BUTTON);
    EGL_RESULT_CHECK(result);

    result = egl_pio_callback_set(USER_BUTTON, user_button_callback);
    EGL_RESULT_CHECK(result);

    result = egl_crc_init(PLAT_CRC, APP_CRC_POLY, APP_CRC_INIT);
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

static egl_result_t app_start(void *param, ...)
{
    egl_result_t result;

    result = switch_init();
    EGL_RESULT_CHECK(result);

    result = radio_init(recv_handler);
    EGL_RESULT_CHECK(result);

    result = fota_init();
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

    result = egl_os_start(SYSOS, app_start);
    EGL_ASSERT_CHECK(result == EGL_SUCCESS, 0);

    return 0;
}