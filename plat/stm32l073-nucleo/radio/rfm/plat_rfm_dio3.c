#include "egl_lib.h"
#include "plat.h"

#if CONFIG_APP_TARGET_RFM_69

#define PORT GPIOA

/* PA10 */
static egl_result_t init(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Set input mode */
    PORT->MODER &= ~GPIO_MODER_MODE10;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD10;

    return EGL_SUCCESS;
}

static egl_result_t get(bool *state)
{
    *state = PORT->IDR & GPIO_IDR_ID10;

    return EGL_SUCCESS;
}

#elif CONFIG_APP_TARGET_RFM_66

#define PORT GPIOB

/* PB3 */
static egl_result_t init(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Set input mode */
    PORT->MODER &= ~GPIO_MODER_MODE3;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD3;

    return EGL_SUCCESS;
}

static egl_result_t get(bool *state)
{
    *state = PORT->IDR & GPIO_IDR_ID3;

    return EGL_SUCCESS;
}

#else
#error "Target RFM is not set"
#endif

const egl_pio_t plat_rfm_dio3_inst =
{
    .init = init,
    .get = get,
};
