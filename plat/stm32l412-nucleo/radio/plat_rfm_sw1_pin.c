#include "egl_lib.h"
#include "plat.h"

/* PC3 */
static void init_pio(void)
{
    /* Enable GPIOC clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

    /* Configure the GPIO as an input */
    GPIOC->MODER &= ~GPIO_MODER_MODE3_Msk;

    /* Configure Low Speed */
    GPIOC->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED3_Msk;

    /* Configure pull up */
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD3_Msk;
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD3_0;
}

static void init_exti(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    EXTI->IMR1  |= EXTI_IMR1_IM3;
    EXTI->FTSR1 |= EXTI_FTSR1_FT3;

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI3;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;
}

static void init_nvic(void)
{
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_SetPriority(EXTI3_IRQn, 0); /* set Priority for Systick Interrupt */
}

/* C2 */
static egl_result_t init(void)
{
    init_pio();
    init_exti();
    init_nvic();

    return EGL_SUCCESS;
}

static egl_result_t get(void)
{
    return GPIOC->IDR & GPIO_IDR_ID3 ? EGL_SET : EGL_RESET;
}

static egl_pio_t plat_rfm_sw1_pin_inst =
{
    .init = init,
    .get  = get
};

egl_pio_t *plat_rfm_sw1_pin_get(void)
{
    return &plat_rfm_sw1_pin_inst;
}

void EXTI3_IRQHandler(void)
{
    /* Clear pending */
    EXTI->PR1 |= EXTI_PR1_PIF3;

    if(plat_rfm_sw1_pin_inst.callback != NULL)
    {
        plat_rfm_sw1_pin_inst.callback(&plat_rfm_sw1_pin_inst);
    }
}