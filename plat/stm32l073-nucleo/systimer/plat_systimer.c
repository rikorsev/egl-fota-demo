#include "egl_lib.h"
#include "plat.h"

static volatile uint32_t timer_ms = 0;

static egl_result_t init(void)
{
    static bool is_inited = false;

    if(is_inited)
    {
        return EGL_SUCCESS;
    }

    is_inited = true;

    /* Init SysTick timer */
    uint32_t ret = SysTick_Config(egl_clock_get(SYSCLOCK) / 1000);
    EGL_ASSERT_CHECK(ret == 0, EGL_FAIL);

    NVIC_SetPriority(SysTick_IRQn, 0);
    NVIC_EnableIRQ(SysTick_IRQn);

    return EGL_SUCCESS;
}

static egl_result_t wait(uint32_t delay)
{
    uint32_t target = timer_ms + delay;

    while(timer_ms < target)
    {
        /* Wait */
    }

    return EGL_SUCCESS;
}

static uint32_t get(void)
{
    return timer_ms;
}

egl_timer_t plat_systimer_inst =
{
    .init = init,
    .get = get,
    .wait = wait
};

egl_timer_t *plat_systimer_get(void)
{
    return &plat_systimer_inst;
}

void plat_systimer_irq_handler(void)
{
    timer_ms++;
}