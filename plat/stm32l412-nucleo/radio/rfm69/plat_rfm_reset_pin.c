#include "egl_lib.h"
#include "plat.h"

/* PB6 */
static egl_result_t init(void)
{
    /* Enable GPIOB clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    /* Configure the GPIO as an output */
    GPIOB->MODER &= ~GPIO_MODER_MODE6_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE6_0;

    /* Configure as push/pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT6_Msk;

    /* Configure Low Speed */
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED6_Msk;

    /* Configure no pull up/down */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6_Msk;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        GPIOB->BSRR |= GPIO_BSRR_BS6_Msk;
    }
    else
    {
        GPIOB->BSRR |= GPIO_BSRR_BR6_Msk;
    }

    return EGL_SUCCESS;
}

static egl_pio_t plat_rfm_reset_inst =
{
    .init = init,
    .set  = set,
};

egl_pio_t *plat_rfm_reset_get(void)
{
    return &plat_rfm_reset_inst;
}