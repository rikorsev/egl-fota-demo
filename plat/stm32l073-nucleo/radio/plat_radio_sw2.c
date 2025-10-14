#include "egl_lib.h"
#include "plat.h"

#define PORT GPIOB
#define DEBONCE_THRESHOLD (100U)

static uint32_t dtime = 0; /* Debounce timestamp */

/* B0 */
static void init_pio(void)
{
    /* Enable GPIOB clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Configure the GPIO as an input */
    PORT->MODER &= ~GPIO_MODER_MODE0_Msk;

    /* Configure Low Speed */
    PORT->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED0_Msk;
}

static void init_exti(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    EXTI->IMR  |= EXTI_IMR_IM0;
    EXTI->FTSR |= EXTI_FTSR_FT0;

    SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PB;
}

static void init_nvic(void)
{
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 0); /* set Priority for Systick Interrupt */
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
    *state = PORT->ODR & GPIO_ODR_OD0_Msk;

    return EGL_SUCCESS;
}

static egl_pio_t plat_radio_sw2_inst =
{
    .init = init,
    .get  = get
};

egl_pio_t *plat_radio_sw2_get(void)
{
    return &plat_radio_sw2_inst;
}

void plat_radio_sw2_irq_handler(void)
{
    uint32_t ctime = egl_timer_get(SYSTIMER);

    if(ctime > DEBONCE_THRESHOLD + dtime)
    {
        dtime = ctime;

        if(plat_radio_sw2_inst.callback != NULL)
        {
            plat_radio_sw2_inst.callback(NULL);
        }
    }
}
