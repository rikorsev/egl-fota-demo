#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "boot_mgr"

static egl_plat_info_t *slot_table[PLAT_NUM_SLOTS];

egl_result_t boot_mgr_slot_validate(egl_plat_info_t *info)
{
    egl_result_t result;
    uint32_t crc_val;
    uint32_t app_addr = (uint32_t)info;
    uint8_t buffer[PLAT_FLASH_PAGE_SIZE];
    uint32_t total_len=0;
    uint32_t pages_to_read;

    if(info == NULL)
    {
        EGL_TRACE_ERROR("Slot information not detected");
        return EGL_FAIL;
    }

    result = egl_crc_reset(PLAT_CRC);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to reset CRC. Result: %s", EGL_RESULT(result));
        return result;
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

static void boot_mgr_slot_info_print(egl_plat_info_t *info)
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
    EGL_TRACE_INFO(" - Boot number: %u", info->boot_number);
}

uint32_t boot_mgr_highest_boot_number_get(void)
{
    return slot_table[1] == NULL ? 0 : slot_table[1]->boot_number;
}

static void boot_mgr_slot_table_sort(egl_plat_info_t **slot_table, size_t size)
{
    if(size < 2)
    {
        return;
    }

    for(uint32_t step = 0; step < size - 1; step++)
    {
        for(uint32_t i = 0; i < size - step - 1; i++)
        {
            egl_plat_info_t *tmp;
            uint32_t boot_number_a = slot_table[i] == NULL ? 0 : slot_table[i]->boot_number;
            uint32_t boot_number_b = slot_table[i + 1] == NULL ? 0 : slot_table[i + 1]->boot_number;

            if(boot_number_a < boot_number_b)
            {
                tmp = slot_table[i];
                slot_table[i] = slot_table[i + 1];
                slot_table[i + 1] = tmp;
            }
        }
    }
}

egl_result_t boot_mgr_init(void)
{
    slot_table[0] = egl_plat_info(PLATFORM);
    slot_table[1] = egl_plat_slot_info(PLATFORM, PLAT_SLOT_A);
    slot_table[2] = egl_plat_slot_info(PLATFORM, PLAT_SLOT_B);

    /* Sort slots (except boot slot) by it boot number */
    boot_mgr_slot_table_sort(&slot_table[1], sizeof(slot_table)/sizeof(slot_table[0]) - 1);

    /* Printout slot info */
    for(uint32_t i = 0; i < sizeof(slot_table)/sizeof(slot_table[0]); i++)
    {
        EGL_TRACE_INFO("Slot %u:", i);
        boot_mgr_slot_info_print(slot_table[i]);
    }

    return EGL_SUCCESS;
}

/* WORKAROUND */
static unsigned int boot_mgr_slot_index_get(egl_plat_info_t *slot_info)
{
    if(slot_info == egl_plat_slot_info(PLATFORM, PLAT_SLOT_A))
    {
        return PLAT_SLOT_A;
    }
    else if(slot_info == egl_plat_slot_info(PLATFORM, PLAT_SLOT_B))
    {
        return PLAT_SLOT_B;
    }

    return 0;
}

void boot_mgr_process(void)
{
    egl_result_t result;
    uint32_t slot_idx;

    for(slot_idx = 1 ; slot_idx < sizeof(slot_table)/sizeof(slot_table[0]); slot_idx++)
    {
        result = boot_mgr_slot_validate(slot_table[slot_idx]);
        if(result == EGL_SUCCESS)
        {
            EGL_TRACE_INFO("Slot %u successfully validated", slot_idx);
            break;
        }
        else
        {
            EGL_TRACE_WARN("Fail to validate slot %u. Result: %s", slot_idx, EGL_RESULT(result));
        }
    }

    result = egl_plat_boot(PLATFORM, boot_mgr_slot_index_get(slot_table[slot_idx]));
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to boot %u slot. Result: %s", EGL_RESULT(result));
    }
}