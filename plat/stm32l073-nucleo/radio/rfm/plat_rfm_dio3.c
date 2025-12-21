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
static egl_result_t init_pio(void)
{
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Set input mode */
    PORT->MODER &= ~GPIO_MODER_MODE3;

    /* Configure no pull up/down */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD3;

    return EGL_SUCCESS;
}

static void init_exti(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    EXTI->IMR  |= EXTI_IMR_IM3;
    EXTI->FTSR |= EXTI_FTSR_FT3;

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PB;
}

static void init_nvic(void)
{
    NVIC_EnableIRQ(EXTI2_3_IRQn);
    NVIC_SetPriority(EXTI2_3_IRQn, 0); /* set Priority for Systick Interrupt */
}

static egl_result_t init(void)
{
    init_pio();
    init_exti();
    init_nvic();

    return EGL_SUCCESS;
}

static egl_result_t get(bool *state)
{
    *state = PORT->IDR & GPIO_IDR_ID3;

    return EGL_SUCCESS;
}

#else
#error "Target RFM is not set or not correct"
#endif

egl_pio_t plat_rfm_dio3_inst =
{
    .init = init,
    .get = get,
};

#if CONFIG_APP_TARGET_RFM_66
void plat_rfm_dio3_irq_handler(void)
{
    if(plat_rfm_dio3_inst.callback != NULL)
    {
        plat_rfm_dio3_inst.callback(NULL);
    }
}
#endif