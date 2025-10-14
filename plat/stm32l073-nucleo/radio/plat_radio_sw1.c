#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOA

/* A4 */
static void init_pio(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Configure the GPIO as an input */
    PORT->MODER &= ~GPIO_MODER_MODE4_Msk;

    /* Configure Low Speed */
    PORT->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED4_Msk;

    /* Configure pull up */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD4_Msk;
    PORT->PUPDR |= GPIO_PUPDR_PUPD4_0;
}

static void init_exti(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    EXTI->IMR  |= EXTI_IMR_IM4;
    EXTI->FTSR |= EXTI_FTSR_FT4;

    SYSCFG->EXTICR[1] &= ~SYSCFG_EXTICR2_EXTI4;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA;
}

static void init_nvic(void)
{
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 0); /* set Priority for Systick Interrupt */
}

/* C13 */
static egl_result_t init(void)
{
    init_pio();
    init_exti();
    init_nvic();

    return EGL_SUCCESS;
}

static egl_result_t get(bool *state)
{
    *state = PORT->ODR & GPIO_ODR_OD4_Msk;

    return EGL_SUCCESS;
}

static egl_pio_t plat_radio_sw1_inst =
{
    .init = init,
    .get  = get
};

egl_pio_t *plat_radio_sw1_get(void)
{
    return &plat_radio_sw1_inst;
}

void plat_radio_sw1_irq_handler(void)
{
    if(plat_radio_sw1_inst.callback != NULL)
    {
        plat_radio_sw1_inst.callback(NULL);
    }
}
