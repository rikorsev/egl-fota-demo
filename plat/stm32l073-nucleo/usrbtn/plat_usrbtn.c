#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOC

/* C13 */
static void init_pio(void)
{
    /* Enable GPIOC clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

    /* Configure the GPIO as an input */
    PORT->MODER &= ~GPIO_MODER_MODE13_Msk;

    /* Configure Low Speed */
    PORT->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED13_Msk;

    /* Configure pull up */
    PORT->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;
    PORT->PUPDR |= GPIO_PUPDR_PUPD13_0;
}

static void init_exti(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    EXTI->IMR  |= EXTI_IMR_IM13;
    EXTI->FTSR |= EXTI_FTSR_FT13;

    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
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
    *state = PORT->ODR & GPIO_ODR_OD13_Msk;

    return EGL_SUCCESS;
}

static egl_pio_t plat_usrbtn_inst =
{
    .init = init,
    .get  = get
};

egl_pio_t *plat_usrbtn_get(void)
{
    return &plat_usrbtn_inst;
}

void plat_usrbtn_irq_handler(void)
{
    if(plat_usrbtn_inst.callback != NULL)
    {
        plat_usrbtn_inst.callback(NULL);
    }
}
