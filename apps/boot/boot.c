#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "boot"

egl_result_t boot_slot_validate(egl_plat_info_t *info)
{
    egl_result_t result;
    uint32_t crc_val;
    uint32_t app_addr = (uint32_t)info;
    uint8_t buffer[PLAT_FLASH_PAGE_SIZE];
    uint32_t total_len=0;
    uint32_t pages_to_read;

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