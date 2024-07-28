#include "egl_lib.h"
#include "plat.h"

/* B13 */
static egl_result_t init(void)
{
    /* Enable GPIOB clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    /* Configure the GPIO as an output */
    GPIOB->MODER &= ~GPIO_MODER_MODE13_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE13_0;

    /* Configure as push/pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT13_Msk;

    /* Configure Low Speed */
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED13_Msk;

    /* Configure no pull up/down */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        GPIOB->BSRR |= GPIO_BSRR_BS13_Msk;
    }
    else
    {
        GPIOB->BSRR |= GPIO_BSRR_BR13_Msk;
    }

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    return GPIOB->ODR & GPIO_ODR_OD13_Msk ? EGL_SET : EGL_RESET;
}

static egl_pio_t pio_led_green_inst =
{
    .init = init,
    .set  = set,
    .get  = get
};

egl_pio_t *pio_led_green_get(void)
{
    return &pio_led_green_inst;
}
