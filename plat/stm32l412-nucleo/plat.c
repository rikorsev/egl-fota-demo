#include "egl_lib.h"
#include "plat.h"

static uint32_t clock(void)
{
    return SystemCoreClock;
}

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

    const uint32_t slot_addr = plat_info_slot_addr_get(slot_idx);

    if(slot_addr == 0)
    {
        return EGL_FAIL;
    }

    /* Disable interrupts */
    __disable_irq();

    /* Set the vector table address */
    SCB->VTOR = (volatile uint32_t)(slot_addr + CONFIG_PLAT_INFO_SECTION_SIZE);

    /* Set the main stack pointer to the application stack pointer */
    uint32_t app_stack = *(volatile uint32_t*)(slot_addr + CONFIG_PLAT_INFO_SECTION_SIZE);
    __set_MSP(app_stack);

    /* Get the application's reset handler address */
    uint32_t app_entry = *(volatile uint32_t*)(slot_addr + CONFIG_PLAT_INFO_SECTION_SIZE + 4);
    void (*app_reset_handler)(void) = (void (*)(void))app_entry;

    /* Start the application */
    app_reset_handler();

    return EGL_SUCCESS;
}

static egl_plat_info_t *slot_info(unsigned int slot_idx)
{
    if(slot_idx == PLAT_SLOT_BOOT)
    {
        return NULL;
    }

    const uint32_t slot_addr = plat_info_slot_addr_get(slot_idx);

    if(slot_addr == 0)
    {
        return NULL;
    }

    /* Check magic value */
    if(((egl_plat_info_t *)slot_addr)->magic != CONFIG_PLAT_INFO_MAGIC_VALUE)
    {
        return NULL;
    }

    return (egl_plat_info_t *)slot_addr;
}

static egl_result_t sleep(uint32_t delay)
{
    uint32_t target = egl_timer_get(SYSTIMER) + delay;

    while(egl_timer_get(SYSTIMER) < target)
    {
        /* Wait */
    }

    return EGL_SUCCESS;
}

static egl_result_t reboot(void)
{
    __NVIC_SystemReset();
    return EGL_SUCCESS;
}

static egl_result_t shutdown(void)
{
    return EGL_SUCCESS;
}

static egl_plat_info_t *info(void)
{
    egl_plat_info_t *info = plat_info_inst_get();

    if(info->magic != CONFIG_PLAT_INFO_MAGIC_VALUE)
    {
        return NULL;
    }

    return info;
}

static egl_platform_t platform_inst =
{
    .init      = init,
    .boot      = boot,
    .reboot    = reboot,
    .sleep     = sleep,
    .shutdown  = shutdown,
    .info      = info,
    .slot_info = slot_info,
    .clock     = clock
};

egl_platform_t *platform_get(void)
{
    return &platform_inst;
}
