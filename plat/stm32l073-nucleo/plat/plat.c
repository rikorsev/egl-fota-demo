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

    egl_result_t result;
    uint32_t slot_addr;

    switch(slot_idx)
    {
        case SLOT_A:
            result = egl_value_u32_get(SLOT_A_ADDR, &slot_addr);
            break;

        case SLOT_B:
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

egl_platform_t platform_inst =
{
    .init      = init,
    .boot      = boot,
};

egl_platform_t *platform_get(void)
{
    return &platform_inst;
}
