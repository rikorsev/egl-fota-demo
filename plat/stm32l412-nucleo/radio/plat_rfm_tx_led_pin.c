#include "egl_lib.h"
#include "plat.h"

/* C1 */
static egl_result_t init(void)
{
    /* Enable GPIOC clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    /* Configure the GPIO as an output */
    GPIOC->MODER &= ~GPIO_MODER_MODE1_Msk;
    GPIOC->MODER |= GPIO_MODER_MODE1_0;

    /* Configure as push/pull output */
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT1_Msk;

    /* Configure Low Speed */
    GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1_Msk;

    /* Configure no pull up/down */
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD1_Msk;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        GPIOC->BSRR |= GPIO_BSRR_BS1_Msk;
    }
    else
    {
        GPIOC->BSRR |= GPIO_BSRR_BR1_Msk;
    }

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    return GPIOC->ODR & GPIO_ODR_OD1_Msk ? EGL_SET : EGL_RESET;
}

static egl_pio_t plat_rfm_tx_led_pin_inst =
{
    .init = init,
    .set  = set,
    .get  = get
};

egl_pio_t *plat_rfm_tx_led_pin_get(void)
{
    return &plat_rfm_tx_led_pin_inst;
}