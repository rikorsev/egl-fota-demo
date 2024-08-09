#include <assert.h>

#include "egl_lib.h"
#include "plat.h"
#include "fota.h"
#include "boot.h"

#define EGL_MODULE_NAME "loader"
#define BOOT_CRC_POLY ((uint32_t)0x4C11DB7)
#define BOOT_CRC_INIT ((uint32_t)0xFFFFFFFF)

#if CONFIG_EGL_TRACE_ENABLED
static egl_result_t init_trace(void)
{
    egl_result_t result;
    static egl_trace_t trace = {0};

    /* Init trace iface */
    result = egl_itf_init(TRACE_IFACE);
    EGL_RESULT_CHECK(result);

    /* Init tracer */
    result = egl_trace_init(&trace, TRACE_IFACE, SYSTIMER);
    EGL_RESULT_CHECK(result);

    /* Set default tracer */
    result = egl_trace_default_set(&trace);
    EGL_RESULT_CHECK(result);

    return result;
}
#endif

static egl_result_t init(void)
{
    egl_result_t result;

    result = egl_plat_init(PLATFORM);
    EGL_RESULT_CHECK(result);

    result = egl_timer_init(SYSTIMER);
    EGL_RESULT_CHECK(result);

#if CONFIG_EGL_TRACE_ENABLED
    result = init_trace();
    EGL_RESULT_CHECK(result);
#endif

    result = egl_crc_init(PLAT_CRC, BOOT_CRC_POLY, BOOT_CRC_INIT);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init CRC. Result %s", EGL_RESULT(result));
        return result;
    }

    result = egl_block_init(PLAT_FLASH);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init flash. Result %s", EGL_RESULT(result));
        return result;
    }

    result = fota_init();
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init fota module. Result %s", EGL_RESULT(result));
        return result;
    }

    return result;
}

static void loop(void)
{

}

static void print_info(egl_plat_info_t *info)
{
    if(info == NULL)
    {
        EGL_TRACE_WARN(" - No platform info found");

        return;
    }

    EGL_TRACE_INFO(" - Application: %s (v%d.%d.%d%s)", info->name,
                                                    info->version.major,
                                                    info->version.minor,
                                                    info->version.revision,
                                                    info->version.sufix);
    EGL_TRACE_INFO(" - Buildtime:   %s", info->buildtime);
    EGL_TRACE_INFO(" - Branch:      %s", info->git.branch);
    EGL_TRACE_INFO(" - Commit:      %s", info->git.commit);
    EGL_TRACE_INFO(" - Size:        %u", info->size);
    EGL_TRACE_INFO(" - Checksum:    %u (0x%08x)", info->checksum, info->checksum);
}

int main(void)
{
    egl_plat_info_t *slot_info;
    egl_result_t result = init();
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_FAIL("Fatal error while platform initilaization. Result: %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    EGL_TRACE_INFO("Bootloader information:");
    print_info(egl_plat_info(PLATFORM));

    fota_manager();

    /* Validate the slot A */
    slot_info = egl_plat_slot_info(PLATFORM, PLAT_SLOT_A);
    result = boot_slot_validate(slot_info);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_WARN("Slot A validation fail. Result: %s", EGL_RESULT(result));
    }

    EGL_TRACE_INFO("Slot A to boot information:");
    print_info(slot_info);

    /* Validate the slot B */
    slot_info = egl_plat_slot_info(PLATFORM, PLAT_SLOT_B);
    result = boot_slot_validate(slot_info);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_WARN("Slot B validation fail. Result: %s", EGL_RESULT(result));
    }

    EGL_TRACE_INFO("Slot B to boot information:");
    print_info(slot_info);

    // result = egl_plat_boot(PLATFORM, PLAT_SLOT_A);
    // EGL_TRACE_INFO("Exit from application. Result %d", EGL_RESULT(result));

    while(1)
    {
        loop();
    }

    return 0;
}