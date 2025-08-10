#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOC

/* C0 */
static egl_result_t init(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    /* Configure the GPIO as an output */
    PORT->MODER &= ~GPIO_MODER_MODE0;
    PORT->MODER |= GPIO_MODER_MODE0_0;

    /* Configure as push/pull output */
    PORT->OTYPER &= ~GPIO_OTYPER_OT_0;

    /* Configure Low Speed */
    PORT->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED0;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD0;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        PORT->BSRR |= GPIO_BSRR_BS_0;
    }
    else
    {
        PORT->BSRR |= GPIO_BSRR_BR_0;
    }

    return EGL_SUCCESS;
}

static egl_result_t get(bool *state)
{
    *state = PORT->ODR & GPIO_ODR_OD0_Msk;

    return EGL_SUCCESS;
}

static egl_pio_t plat_radio_rx_led_inst =
{
    .init = init,
    .set  = set,
    .get  = get
};

egl_pio_t *plat_radio_rx_led_get(void)
{
    return &plat_radio_rx_led_inst;
}
