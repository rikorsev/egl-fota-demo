#include "egl_lib.h"
#include "plat.h"

static egl_result_t init(void)
{
    __enable_irq();

    return EGL_SUCCESS;
}

static egl_result_t boot(unsigned int slot_idx)
{
    if(slot_idx == PLAT_SLOT_BOOT)
    {
        return EGL_INVALID_PARAM;
    }

    const uint32_t slot_addr = slot_info_addr_get(slot_idx);

    if(slot_addr == 0)
    {
        return EGL_FAIL;
    }

    /* Disable interrupts */
    __disable_irq();

    /* Set the vector table address */
    SCB->VTOR = (volatile uint32_t)(slot_addr + CONFIG_PLAT_SLOT_INFO_SECTION_SIZE);

    /* Set the main stack pointer to the application stack pointer */
    uint32_t app_stack = *(volatile uint32_t*)(slot_addr + CONFIG_PLAT_SLOT_INFO_SECTION_SIZE);
    __set_MSP(app_stack);

    /* Get the application's reset handler address */
    uint32_t app_entry = *(volatile uint32_t*)(slot_addr + CONFIG_PLAT_SLOT_INFO_SECTION_SIZE + 4);
    void (*app_reset_handler)(void) = (void (*)(void))app_entry;

    /* Start the application */
    app_reset_handler();

    return EGL_SUCCESS;
}

static void *slot_info(unsigned int slot_idx)
{
    if(slot_idx == PLAT_SLOT_BOOT)
    {
        return NULL;
    }

    const uint32_t slot_addr = slot_info_addr_get(slot_idx);

    if(slot_addr == 0)
    {
        return NULL;
    }

    /* Check magic value */
    if(((slot_info_t *)slot_addr)->magic != CONFIG_PLAT_SLOT_INFO_MAGIC_VALUE)
    {
        return NULL;
    }

    return (slot_info_t *)slot_addr;
}

static void *info(void)
{
    slot_info_t *info = (slot_info_t *)slot_info_inst_get();

    if(info->magic != CONFIG_PLAT_SLOT_INFO_MAGIC_VALUE)
    {
        return NULL;
    }

    return info;
}

static egl_platform_t platform_inst =
{
    .init      = init,
    .boot      = boot,
    .info      = info,
    .slot_info = slot_info,
};

egl_platform_t *platform_get(void)
{
    return &platform_inst;
}
