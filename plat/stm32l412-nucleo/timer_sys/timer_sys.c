#include <assert.h>

#include "egl_lib.h"
#include "plat.h"

static volatile uint32_t timer_ms = 0;

void SysTick_Handler(void)
{
    timer_ms++;
}

static egl_result_t init(void)
{
    uint32_t result;

    /* TBD move to clock section */
    SystemCoreClockUpdate();

    /* Init SysTick timer */
    result = SysTick_Config(egl_plat_clock(PLATFORM) / 1000);
    assert(result == 0);

    return EGL_SUCCESS;
}

static uint32_t get(void)
{
    return timer_ms;
}

static egl_timer_t plat_timer_sys_inst =
{
    .init = init,
    .get = get
};

egl_timer_t *plat_timer_sys_get(void)
{
    return &plat_timer_sys_inst;
}
