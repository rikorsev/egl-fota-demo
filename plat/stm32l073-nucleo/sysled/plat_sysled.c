#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOA

/* A5 */
static egl_result_t init(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Configure the GPIO as an output */
    PORT->MODER &= ~GPIO_MODER_MODE5;
    PORT->MODER |= GPIO_MODER_MODE5_0;

    /* Configure as push/pull output */
    PORT->OTYPER &= ~GPIO_OTYPER_OT_5;

    /* Configure Low Speed */
    PORT->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED5;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD5;

    return EGL_SUCCESS;
}

static egl_result_t set(bool state)
{
    if(state)
    {
        PORT->BSRR |= GPIO_BSRR_BS_5;
    }
    else
    {
        PORT->BSRR |= GPIO_BSRR_BR_5;
    }

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    return PORT->ODR & GPIO_ODR_OD5_Msk ? EGL_SET : EGL_RESET;
}

static egl_pio_t plat_sysled_inst =
{
    .init = init,
    .set  = set,
    .get  = get
};

egl_pio_t *plat_sysled_get(void)
{
    return &plat_sysled_inst;
}
