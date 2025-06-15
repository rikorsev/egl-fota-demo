#include "egl_lib.h"
#include "plat.h"

#define BAUDRATE_115200 (0x12U)

extern egl_timer_t plat_systimer_inst;


static void init_tx_pin(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Configure the GPIO as an alternative function */
    GPIOA->MODER &= ~GPIO_MODER_MODE2;
    GPIOA->MODER |= GPIO_MODER_MODE2_1;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_2;

    /* Configure Low Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED2;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2;

    /* Set alternative function number */
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2;
    GPIOA->AFR[0] |= 0x4 << GPIO_AFRL_AFSEL2_Pos;
}

static void init_uart(void)
{
    /* Enable UART clock */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Set baud rate */
    USART2->BRR = BAUDRATE_115200;

    /* Enable transmitter */
    USART2->CR1 |= USART_CR1_TE;

    /* Enable UART */
    USART2->CR1 |= USART_CR1_UE;
}

/* PA2 PA3 */
static egl_result_t init(void)
{
    init_tx_pin();
    init_uart();

    return EGL_SUCCESS;
}

static size_t write(void *data, size_t len)
{
    for(uint32_t i = 0; i < len; i++)
    {
        while(!(USART2->ISR & USART_ISR_TXE))
        {
            /* Wait for character transmission */
        }
        USART2->TDR = ((uint8_t *)data)[i];
    }

    return len;
}

static egl_interface_t plat_syslog_iface =
{
    .init = init,
    .write = write
};

egl_log_t plat_syslog_inst =
{
    .iface = &plat_syslog_iface,
    .timer = &plat_systimer_inst
};

egl_log_t *plat_syslog_get(void)
{
    return &plat_syslog_inst;
}

