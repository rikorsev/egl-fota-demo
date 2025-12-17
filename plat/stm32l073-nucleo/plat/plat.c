#include <string.h>

#include "egl_lib.h"
#include "plat.h"

__attribute__((section(".boot_info")))
static plat_boot_config_t boot_config = {0};

static egl_result_t init(void)
{
    __enable_irq();

    return EGL_SUCCESS;
}

static egl_result_t boot(plat_boot_config_t *info)
{
    egl_result_t result;
    uint32_t slot_addr;

    memcpy(&boot_config, info, sizeof(boot_config) - sizeof(boot_config.checksum));

    result = egl_crc_reset(PLAT_CRC);
    EGL_RESULT_CHECK(result);

    boot_config.checksum = egl_crc32_calc(PLAT_CRC, &boot_config, sizeof(boot_config) - sizeof(boot_config.checksum));

    switch(info->slot)
    {
        case PLAT_SLOT_BOOT:
            result = egl_value_u32_get(SLOT_BOOT_ADDR, &slot_addr);
            break;

        case PLAT_SLOT_A:
            result = egl_value_u32_get(SLOT_A_ADDR, &slot_addr);
            break;

        case PLAT_SLOT_B:
            result = egl_value_u32_get(SLOT_B_ADDR, &slot_addr);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }
    EGL_RESULT_CHECK(result);

    if(slot_addr == 0)
    {
        return EGL_FAIL;
    }

    /* Disable interrupts */
    __disable_irq();

    /* Set the vector table address */
    SCB->VTOR = (volatile uint32_t)(slot_addr);

    /* Set the main stack pointer to the application stack pointer */
    uint32_t app_stack = *(volatile uint32_t*)(slot_addr);
    __set_MSP(app_stack);

    /* Get the application's reset handler address */
    uint32_t app_entry = *(volatile uint32_t*)(slot_addr + 4);
    void (*app_reset_handler)(void) = (void (*)(void))app_entry;

    /* Start the application */
    app_reset_handler();

    return EGL_SUCCESS;
}

static egl_result_t boot_config_get(plat_boot_config_t *info, size_t *len)
{
    memcpy(info, &boot_config, sizeof(boot_config));

    return EGL_SUCCESS;
}

static egl_result_t cmd(unsigned int id, void *data, size_t *len)
{
    egl_result_t result;

    switch(id)
    {
        case PLAT_CMD_BOOT:
            result = boot(data);
            break;

        case PLAT_CMD_BOOT_CONFIG_GET:
            result = boot_config_get(data, len);
            break;

        default:
            result = EGL_NOT_SUPPORTED;
    }

    return result;
}

egl_platform_t platform_inst =
{
    .init      = init,
    .cmd       = cmd,
};

egl_platform_t *platform_get(void)
{
    return &platform_inst;
}
