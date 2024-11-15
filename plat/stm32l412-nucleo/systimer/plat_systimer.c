#include "egl_lib.h"
#include "plat.h"

static volatile uint32_t timer_ms = 0;

static egl_result_t init(void)
{
    /* Init SysTick timer */
    return SysTick_Config(egl_clock_get(SYSCLOCK) / 1000) == 0 ? EGL_SUCCESS : EGL_FAIL;
}

static uint32_t get(void)
{
    return timer_ms;
}

static egl_timer_t plat_systimer_inst =
{
    .init = init,
    .get = get
};

egl_timer_t *plat_systimer_get(void)
{
    return &plat_systimer_inst;
}

void plat_systimer_irq_handler(void)
{
    timer_ms++;
}