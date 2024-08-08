#include <assert.h>

#include "egl_lib.h"
#include "plat.h"
#include "fota.h"

#define EGL_MODULE_NAME "boot"
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

static egl_result_t validate(egl_plat_info_t *info)
{
    egl_result_t result;
    uint32_t crc_val;
    uint32_t app_addr = (uint32_t)info;
    uint8_t buffer[PLAT_FLASH_PAGE_SIZE];
    uint32_t total_len=0;
    uint32_t pages_to_read;

    EGL_TRACE_INFO("Validatoin...");

    if(info == NULL)
    {
        EGL_TRACE_ERROR("Application information not detected");
        return EGL_FAIL;
    }

    EGL_TRACE_INFO("Binary len: %u", info->size);

    pages_to_read = (info->size + CONFIG_PLAT_INFO_SECTION_SIZE)/PLAT_FLASH_PAGE_SIZE + 1;
    for(int i = 0, offset = 0; i < pages_to_read; i++, offset += PLAT_FLASH_PAGE_SIZE)
    {
        uint8_t *data = buffer;
        size_t len = PLAT_FLASH_PAGE_SIZE;

        EGL_TRACE_INFO("Read flash page %u, Offset: %u (0x%08x)", i, offset, offset);

        /* Read flash page */
        result = egl_block_read(PLAT_FLASH, app_addr + offset, buffer);
        if(result != EGL_SUCCESS)
        {
            return result;
        }

        /* If it is first page to read, it contains info section that should't be calculated 
           So, just skip it */
        if(offset == 0)
        {
            data += CONFIG_PLAT_INFO_SECTION_SIZE;
            len -= CONFIG_PLAT_INFO_SECTION_SIZE;
        }

        /* if number of bytes in the page more that binary size we should truncate it to 
           binary size */
        if((offset + PLAT_FLASH_PAGE_SIZE) > (info->size + CONFIG_PLAT_INFO_SECTION_SIZE))
        {
            len -= (offset + PLAT_FLASH_PAGE_SIZE) - (info->size + CONFIG_PLAT_INFO_SECTION_SIZE);
        }

        crc_val = egl_crc32_calc(PLAT_CRC, data, len);

        total_len += len;
    }

    EGL_TRACE_INFO("Total len calculated: %u", total_len);
    EGL_TRACE_INFO("Calculated CRC: 0x%08x, Expected: 0x%08x", crc_val, info->checksum);

    return crc_val == info->checksum ? EGL_SUCCESS : EGL_FAIL;
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
    result = validate(slot_info);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_WARN("Slot A validation fail. Result: %s", EGL_RESULT(result));
    }

    EGL_TRACE_INFO("Slot A to boot information:");
    print_info(slot_info);

    /* Validate the slot B */
    slot_info = egl_plat_slot_info(PLATFORM, PLAT_SLOT_B);
    result = validate(slot_info);
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