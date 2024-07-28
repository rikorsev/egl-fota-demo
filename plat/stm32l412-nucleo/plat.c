#include <assert.h>

#include "egl_lib.h"
#include "plat.h"

#if !defined  (HSE_VALUE)
  #define HSE_VALUE    8000000U  /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (MSI_VALUE)
  #define MSI_VALUE    4000000U  /*!< Value of the Internal oscillator in Hz*/
#endif /* MSI_VALUE */

#if !defined  (HSI_VALUE)
  #define HSI_VALUE    16000000U /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

static volatile uint32_t timer_ms = 0;

void SysTick_Handler(void)
{
    timer_ms++;
}

static uint32_t clock(void)
{
    return SystemCoreClock;
}

static egl_result_t init(void)
{
    uint32_t result;

    SystemCoreClockUpdate();

    /* Init SysTick timer */
    result = SysTick_Config(clock() / 1000);
    assert(result == 0);

    return EGL_SUCCESS;
}

static uint32_t time(void)
{
    return timer_ms;
}

static egl_result_t sleep(uint32_t delay)
{
    uint32_t target = timer_ms + delay;

    while(timer_ms < target)
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
    .time     = time,
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
