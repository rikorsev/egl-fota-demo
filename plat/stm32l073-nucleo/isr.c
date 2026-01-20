#include "stm32l0xx.h"
#include "plat_systimer.h"
#include "plat_usrbtn.h"

#if CONFIG_APP_TARGET_RFM_69 || CONFIG_APP_TARGET_RFM_66
#include "plat_radio_iface.h"
#endif

void SysTick_Handler(void)
{
#if CONFIG_EGL_OS_ENABLED
    extern void _tx_timer_interrupt(void);
    _tx_timer_interrupt();
#endif
    plat_systimer_irq_handler();
}

void EXTI4_15_IRQHandler(void)
{
#if CONFIG_APP_TARGET_RFM_69 || CONFIG_APP_TARGET_RFM_66
    if(EXTI->PR & EXTI_PR_PIF4)
    {
        EXTI->PR = EXTI_PR_PIF4;
        plat_radio_sw1_irq_handler();
    }
#endif

    if(EXTI->PR & EXTI_PR_PIF13)
    {
        EXTI->PR = EXTI_PR_PIF13;
        plat_usrbtn_irq_handler();
    }
}

#if CONFIG_APP_TARGET_RFM_69 || CONFIG_APP_TARGET_RFM_66
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

#endif