#include "egl_lib.h"
#include "plat.h"

static inline void init_reset_pin(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Configure the GPIO as an output */
    GPIOA->MODER &= ~GPIO_MODER_MODE9;
    GPIOA->MODER |= GPIO_MODER_MODE9_0;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_9;

    /* Configure Low Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED9;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;
}

static egl_result_t init(void)
{
    init_reset_pin();

    return EGL_SUCCESS;
}

static egl_result_t poweron(void)
{
    return EGL_SUCCESS;
}

static egl_result_t reset(void)
{
    egl_result_t result;

    /* up 1 ms */
    GPIOA->BSRR |= GPIO_BSRR_BS_9;
    result = egl_sys_delay(2);
    EGL_RESULT_CHECK(result);

    /* down 5 ms */
    GPIOA->BSRR |= GPIO_BSRR_BR_9;
    result = egl_sys_delay(5);
    EGL_RESULT_CHECK(result);

    return result;
}

const egl_pm_t plat_rfm_pm_inst =
{
    .init  = init,
    .poweron = poweron,
    .reset = reset
};
