#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOC

/* C1 */
static egl_result_t init(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    /* Configure the GPIO as an output */
    PORT->MODER &= ~GPIO_MODER_MODE1;
    PORT->MODER |= GPIO_MODER_MODE1_0;

    /* Configure as push/pull output */
    PORT->OTYPER &= ~GPIO_OTYPER_OT_1;

    /* Configure Low Speed */
    PORT->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED1;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD1;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        PORT->BSRR |= GPIO_BSRR_BS_1;
    }
    else
    {
        PORT->BSRR |= GPIO_BSRR_BR_1;
    }

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    return PORT->ODR & GPIO_ODR_OD1_Msk ? EGL_SET : EGL_RESET;
}

static egl_pio_t plat_radio_tx_led_inst =
{
    .init = init,
    .set  = set,
    .get  = get
};

egl_pio_t *plat_radio_tx_led_get(void)
{
    return &plat_radio_tx_led_inst;
}
