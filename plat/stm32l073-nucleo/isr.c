#include "stm32l0xx.h"
#include "plat_systimer.h"
#include "plat_usrbtn.h"

void SysTick_Handler(void)
{
    plat_systimer_irq_handler();
}

void EXTI4_15_IRQHandler(void)
{
    if(EXTI->PR & EXTI_PR_PIF13) // Check if the interrupt is for pin 13
    {
        EXTI->PR = EXTI_PR_PIF13; // Clear the pending interrupt
        plat_usrbtn_irq_handler(); // Call the user button interrupt handler
    }
}
