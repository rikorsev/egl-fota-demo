#include "egl_lib.h"
#include "plat.h"

/* PA15 */
static egl_result_t init(void)
{
    /* Enable GPIOA clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* Configure the GPIO as an input */
    GPIOA->MODER &= ~GPIO_MODER_MODE15_Msk;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT15_Msk;

    /* Configure Low Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED15_Msk;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD15_Msk;

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    egl_result_t result;

    if(GPIOA->IDR & GPIO_IDR_ID15)
    {
        result = EGL_SET;
    }
    else
    {
        result = EGL_RESET;
    }

    return result;
}

static egl_pio_t plat_rfm_dio5_pin_inst =
{
    .init = init,
    .get = get,
};

egl_pio_t *plat_rfm_dio5_get(void)
{
    return &plat_rfm_dio5_pin_inst;
}