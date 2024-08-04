#include "egl_lib.h"
#include "plat.h"

/* C0 */
static egl_result_t init(void)
{
    /* Enable GPIOC clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    /* Configure the GPIO as an output */
    GPIOC->MODER &= ~GPIO_MODER_MODE0_Msk;
    GPIOC->MODER |= GPIO_MODER_MODE0_0;

    /* Configure as push/pull output */
    GPIOC->OTYPER &= ~GPIO_OTYPER_OT0_Msk;

    /* Configure Low Speed */
    GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED0_Msk;

    /* Configure no pull up/down */
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        GPIOC->BSRR |= GPIO_BSRR_BS0_Msk;
    }
    else
    {
        GPIOC->BSRR |= GPIO_BSRR_BR0_Msk;
    }

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    return GPIOC->ODR & GPIO_ODR_OD0_Msk ? EGL_SET : EGL_RESET;
}

static egl_pio_t plat_rfm_rx_led_pin_inst =
{
    .init = init,
    .set  = set,
    .get  = get
};

egl_pio_t *plat_rfm_rx_led_pin_get(void)
{
    return &plat_rfm_rx_led_pin_inst;
}
