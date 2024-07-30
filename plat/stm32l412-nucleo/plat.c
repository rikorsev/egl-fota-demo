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
    static const uint32_t slot_table[] = 
    {
        0x08000000,
        0x08010000,
        0x08020000
    };

    if(slot_idx >= sizeof(slot_table)/sizeof(slot_table[0]))
    {
        return EGL_OUT_OF_BOUNDARY;
    }

    const uint32_t app_start = slot_table[slot_idx];

    /* Disable interrupts */
    __disable_irq();

    /* Set the vector table address */
    SCB->VTOR = (volatile uint32_t)app_start;

    /* Set the main stack pointer to the application stack pointer */
    uint32_t app_stack = *(volatile uint32_t*)app_start;
    __set_MSP(app_stack);

    /* Get the application's reset handler address */
    uint32_t app_entry = *(volatile uint32_t*)(app_start + 4);
    void (*app_reset_handler)(void) = (void (*)(void))app_entry;

    /* Start the application */
    app_reset_handler();

    return EGL_SUCCESS;
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

static char *info(void)
{
    return "NUCLEO64 (STM32L412)";
}

static egl_platform_t platform_inst =
{
    .init     = init,
    .boot     = boot,
    .reboot   = reboot,
    .sleep    = sleep,
    .shutdown = shutdown,
    .info     = info,
    .clock    = clock
};

egl_platform_t *platform_get(void)
{
    return &platform_inst;
}
