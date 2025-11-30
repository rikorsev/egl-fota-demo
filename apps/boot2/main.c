#include "egl_lib.h"
#include "plat.h"

#define BOOT_CRC_POLY ((uint32_t)0x4C11DB7)
#define BOOT_CRC_INIT ((uint32_t)0xFFFFFFFF)

static egl_result_t error_handler_func(egl_result_t result, char *file, unsigned int line, void *ctx)
{
#if CONFIG_EGL_LOG_ENABLED
    egl_log(SYSLOG, EGL_LOG_LEVEL_ERROR, file, "line: %u: Result: %s", line, EGL_RESULT(result));
#endif

    return result;
}

static egl_result_t init(void)
{
    egl_result_t result;
    static egl_result_error_handler_t error_handler = { error_handler_func };

    result = egl_system_init(SYSTEM);
    EGL_RESULT_CHECK(result);

    egl_result_error_handler_set(&error_handler);

    result = egl_crc_init(PLAT_CRC, BOOT_CRC_POLY, BOOT_CRC_INIT);
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

static egl_result_t boot(void)
{
    egl_result_t result;
    plat_boot_info_t boot_info;

    EGL_LOG_INFO("Check boot info CRC");

    result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_GET_BOOT_INFO, &boot_info, NULL);
    EGL_RESULT_CHECK(result);

    result = egl_crc_reset(PLAT_CRC);
    EGL_RESULT_CHECK(result);

    uint32_t calculated = egl_crc32_calc(PLAT_CRC, &boot_info, sizeof(boot_info) - sizeof(boot_info.checksum));
    if(calculated != boot_info.checksum)
    {
        EGL_LOG_WARN("Boot info is not valid. Perform regular boot");
    
        boot_info.task = PLAT_BOOT_TASK_NONE;
        boot_info.slot = PLAT_SLOT_A;
    }

    switch(boot_info.task)
    {
        case PLAT_BOOT_TASK_NONE:
            EGL_LOG_INFO("No boot task assigned");
            result = EGL_SUCCESS;
            break;

        case PLAT_BOOT_TASK_UPLOAD_SLOT_A:
            boot_info.slot = PLAT_SLOT_A;
            result = EGL_SUCCESS;
            EGL_LOG_DEBUG("PLAT_BOOT_TASK_UPLOAD_SLOT_A");
            break;

        case PLAT_BOOT_TASK_UPLOAD_SLOT_B:
            boot_info.slot = PLAT_SLOT_A;
            result = EGL_SUCCESS;
            EGL_LOG_DEBUG("PLAT_BOOT_TASK_UPLOAD_SLOT_B");
            break;

        case PLAT_BOOT_TASK_DOWNLOAD_SLOT_A:
            boot_info.slot = PLAT_SLOT_A;
            result = EGL_SUCCESS;
            EGL_LOG_DEBUG("PLAT_BOOT_TASK_DOWNLOAD_SLOT_A");
            break;

        case PLAT_BOOT_TASK_DOWNLOAD_SLOT_B:
            boot_info.slot = PLAT_SLOT_A;
            result = EGL_SUCCESS;
            EGL_LOG_DEBUG("PLAT_BOOT_TASK_DOWNLOAD_SLOT_B");
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    result = egl_plat_cmd_exec(PLATFORM, PLAT_CMD_BOOT, &boot_info, NULL);
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

    result = boot();
    EGL_LOG_INFO("Boot complete. Result: %s", EGL_RESULT(result));

    while(1)
    {
        /* TBD: go in DeepSleep power mode*/
        /* Wait forewer */
    }

    return 0;
}