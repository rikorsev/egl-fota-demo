#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOA

static egl_result_t init(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Set input mode */
    PORT->MODER &= ~GPIO_MODER_MODE8;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD8;

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    egl_result_t result;

    if(PORT->IDR & GPIO_IDR_ID8)
    {
        result = EGL_SET;
    }
    else
    {
        result = EGL_RESET;
    }

    return result;
}

const egl_pio_t plat_rfm_dio0_inst =
{
    .init = init,
    .get = get,
};
