#include "egl_lib.h"
#include "plat.h"

static egl_result_t init(void)
{
    __enable_irq();

    return EGL_SUCCESS;
}

static egl_result_t boot(unsigned int slot_idx)
{
    egl_result_t result;
    uint32_t slot_addr;

    switch(slot_idx)
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
            return EGL_INVALID_PARAM;
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

static egl_result_t cmd(unsigned int id, void *data, size_t *len)
{
    egl_result_t result;

    switch(id)
    {
        case PLAT_CMD_BOOT:
            result = boot((unsigned int)data);
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
