#include "egl_lib.h"
#include "plat.h"

/* PC7 */
static egl_result_t init(void)
{
    /* Enable GPIOC clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    /* Configure the GPIO as an input */
    GPIOC->MODER &= ~GPIO_MODER_MODE7_Msk;

    /* Configure as push/pull output */
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT7_Msk;

    /* Configure Low Speed */
    GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED7_Msk;

    /* Configure no pull up/down */
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD7_Msk;

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    egl_result_t result;

    if(GPIOC->IDR & GPIO_IDR_ID7)
    {
        result = EGL_SET;
    }
    else
    {
        result = EGL_RESET;
    }

    return result;
}

static egl_pio_t plat_rfm_dio0_pin_inst =
{
    .init = init,
    .get = get,
};

egl_pio_t *plat_rfm_dio0_get(void)
{
    return &plat_rfm_dio0_pin_inst;
}