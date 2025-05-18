#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOB

static egl_result_t init(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Set input mode */
    PORT->MODER &= ~GPIO_MODER_MODE5;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD5;

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    egl_result_t result;

    if(PORT->IDR & GPIO_IDR_ID5)
    {
        result = EGL_SET;
    }
    else
    {
        result = EGL_RESET;
    }

    return result;
}

const egl_pio_t plat_rfm_dio4_pin_inst =
{
    .init = init,
    .get = get,
};
