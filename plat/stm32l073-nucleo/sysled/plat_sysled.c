#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOB

/* B7 */
static egl_result_t init(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Configure the GPIO as an output */
    PORT->MODER &= ~GPIO_MODER_MODE7;
    PORT->MODER |= GPIO_MODER_MODE7_0;

    /* Configure as push/pull output */
    PORT->OTYPER &= ~GPIO_OTYPER_OT_7;

    /* Configure Low Speed */
    PORT->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED7;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD7;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        PORT->BSRR |= GPIO_BSRR_BS_7;
    }
    else
    {
        PORT->BSRR |= GPIO_BSRR_BR_7;
    }

    return EGL_SUCCESS;
}

static egl_result_t get(bool *state)
{
    *state = PORT->ODR & GPIO_ODR_OD7_Msk;

    return EGL_SUCCESS;
}

static egl_result_t toggle(void)
{
    bool state = PORT->ODR & GPIO_ODR_OD7_Msk;
    if(state)
    {
        PORT->BSRR |= GPIO_BSRR_BR_7;
    }
    else
    {
        PORT->BSRR |= GPIO_BSRR_BS_7;
    }

    return EGL_SUCCESS;
}

static egl_pio_t plat_sysled_inst =
{
    .init   = init,
    .set    = set,
    .get    = get,
    .toggle = toggle
};

egl_pio_t *plat_sysled_get(void)
{
    return &plat_sysled_inst;
}
