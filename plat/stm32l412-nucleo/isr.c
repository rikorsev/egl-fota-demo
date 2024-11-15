#include "plat_systimer.h"

void SysTick_Handler(void)
{
    plat_systimer_irq_handler();
}
