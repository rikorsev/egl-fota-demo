#include "egl_lib.h"
#include "plat.h"

/* PB5 */
static egl_result_t init(void)
{
    /* Enable GPIOB clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    /* Configure the GPIO as an input */
    GPIOB->MODER &= ~GPIO_MODER_MODE5_Msk;

    /* Configure as push/pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT5_Msk;

    /* Configure Low Speed */
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED5_Msk;

    /* Configure no pull up/down */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5_Msk;

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    egl_result_t result;

    if(GPIOB->IDR & GPIO_IDR_ID5)
    {
        result = EGL_SET;
    }
    else
    {
        result = EGL_RESET;
    }

    return result;
}

static egl_pio_t plat_rfm_dio4_pin_inst =
{
    .init = init,
    .get = get,
};

egl_pio_t *plat_rfm_dio4_get(void)
{
    return &plat_rfm_dio4_pin_inst;
}