#include "stm32l0xx.h"
#include "plat_systimer.h"
#include "plat_usrbtn.h"
#include "plat_radio_iface.h"

void SysTick_Handler(void)
{
    plat_systimer_irq_handler();
}

void EXTI4_15_IRQHandler(void)
{
    if(EXTI->PR & EXTI_PR_PIF4)
    {
        EXTI->PR = EXTI_PR_PIF4;
        plat_radio_sw1_irq_handler();
    }

    if(EXTI->PR & EXTI_PR_PIF13)
    {
        EXTI->PR = EXTI_PR_PIF13;
        plat_usrbtn_irq_handler();
    }
}

void EXTI0_1_IRQHandler(void)
{
    if(EXTI->PR & EXTI_PR_PIF0)
    {
        EXTI->PR = EXTI_PR_PIF0;
        plat_radio_sw2_irq_handler();
    }
}

void EXTI2_3_IRQHandler(void)
{
    if(EXTI->PR & EXTI_PR_PIF3)
    {
#if CONFIG_APP_TARGET_RFM_69
        plat_rfm_dio2_irq_handler();
#elif CONFIG_APP_TARGET_RFM_66
        plat_rfm_dio3_irq_handler();
#endif
        EXTI->PR = EXTI_PR_PIF3;
    }
}

