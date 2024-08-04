#include "egl_lib.h"
#include "plat.h"

#define BAUDRATE_115200 (8889u)

static void init_tx_pin(void)
{
    /* Enable GPIOA clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    /* Configure the GPIO as an alternative function */
    GPIOA->MODER &= ~GPIO_MODER_MODE2_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE2_1;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT2_Msk;

    /* Configure Low Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED2_Msk;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD2_Msk;

    /* Set alternative function number */
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_Msk;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_3;
}

static void init_uart(void)
{
    /* Enable UART clock */
    RCC->APB1ENR2 |= RCC_APB1ENR2_LPUART1EN;

    /* Set baud rate */
    LPUART1->BRR = BAUDRATE_115200;

    /* Enable transmitter */
    LPUART1->CR1 |= USART_CR1_TE;

    /* Enable UART */
    LPUART1->CR1 |= USART_CR1_UE;
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
        while(!(LPUART1->ISR & USART_ISR_TXE))
        {
            /* Wait for character transmission */
        }
        LPUART1->TDR = ((uint8_t *)data)[i];
    }

    return len;
}

static egl_interface_t iface_trace_inst =
{
    .init = init,
    .write = write
};

egl_interface_t *iface_trace_inst_get(void)
{
    return &iface_trace_inst;
}

