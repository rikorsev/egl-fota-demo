#include "egl_lib.h"
#include "plat.h"

/* PA11 */
static egl_result_t init(void)
{
    /* Enable GPIOA clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* Configure the GPIO as an output */
    GPIOA->MODER &= ~GPIO_MODER_MODE11_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE11_0;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT11_Msk;

    /* Configure Low Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED11_Msk;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD11_Msk;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        GPIOA->BSRR |= GPIO_BSRR_BS11_Msk;
    }
    else
    {
        GPIOA->BSRR |= GPIO_BSRR_BR11_Msk;
    }

    return EGL_SUCCESS;
}

static egl_pio_t plat_rfm_nss_pin_inst =
{
    .init = init,
    .set  = set,
};

egl_pio_t *plat_rfm_nss_get(void)
{
    return &plat_rfm_nss_pin_inst;
}
