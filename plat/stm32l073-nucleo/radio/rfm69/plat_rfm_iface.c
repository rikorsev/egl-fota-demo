#include "egl_lib.h"
#include "plat.h"

/* PA7 */
static inline void init_mosi_pin(void)
{
    /* Configure the GPIO as an alternative function */
    GPIOA->MODER &= ~GPIO_MODER_MODE7;
    GPIOA->MODER |= GPIO_MODER_MODE7_1;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_7;

    /* Configure Very High Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED7;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED7_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED7_1;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD7;

    /* Set alternative function number */
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7;
}

/* PA6 */
static inline void init_miso_pin(void)
{
    /* Configure the GPIO as an alternative function */
    GPIOA->MODER &= ~GPIO_MODER_MODE6;
    GPIOA->MODER |= GPIO_MODER_MODE6_1;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_6;

    /* Configure Very High Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED6;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED6_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED6_1;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD6;

    /* Set alternative function number */
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL6;
}

/* PA5 */
static inline void init_sck_pin(void)
{
    /* Configure the GPIO as an alternative function */
    GPIOA->MODER &= ~GPIO_MODER_MODE5;
    GPIOA->MODER |= GPIO_MODER_MODE5_1;

    /* Configure as push/pull output */
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT_5;

    /* Configure Very High Speed */
    GPIOA->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED5;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEED5_1;

    /* Configure no pull up/down */
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;

    /* Set alternative function number */
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
}

/* PB6 */
static inline void init_nss_pin(void)
{
    /* Configure the GPIO as an output */
    GPIOB->MODER &= ~GPIO_MODER_MODE6;
    GPIOB->MODER |= GPIO_MODER_MODE6_0;

    /* Configure as push/pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT_6;

    /* Configure Very High Speed */
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEED6;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED6_0;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEED6_1;

    /* Configure no pull up/down */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;

    /* Set to high */
    GPIOB->BSRR |= GPIO_BSRR_BS_6;
}

static inline void init_pio(void)
{
    /* Enable GPIOA/B clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    init_mosi_pin();
    init_miso_pin();
    init_sck_pin();
    init_nss_pin();
}

static inline void init_spi(void)
{
    /* Enable SPI clock */
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    /* Set SSN configuration */
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR1 |= SPI_CR1_SSI;

    /* Set master mode */
    SPI1->CR1 |= SPI_CR1_MSTR;

    /* Enable SPI */
    SPI1->CR1 |= SPI_CR1_SPE;
}

static egl_result_t init(void)
{
    init_pio();
    init_spi();

    return EGL_SUCCESS;
}

static size_t write_addr(uint32_t addr, void *data, size_t len)
{
    /* Set NSS */
    GPIOB->BSRR |= GPIO_BSRR_BR_6;

    /* Set address */
    while(!(SPI1->SR & SPI_SR_TXE))
    {
        /* Wait for data transmission */
    }
    *(volatile uint8_t *)&SPI1->DR = ((uint8_t)addr & 0x7F) | 0x80;
    while(!(SPI1->SR & SPI_SR_RXNE))
    {
        /* Wait for data reception */
    }
    *(volatile uint8_t *)&SPI1->DR; /* Dummy read */


    /* Write data */
    for(uint32_t i = 0; i < len; i++)
    {
        while(!(SPI1->SR & SPI_SR_TXE))
        {
            /* Wait for data transmission */
        }
        *(volatile uint8_t*)&SPI1->DR = ((uint8_t *)data)[i];

        while(!(SPI1->SR & SPI_SR_RXNE))
        {
            /* Wait for data reception */
        }
        /* Dummy read */
        *(volatile uint8_t *)&SPI1->DR;
    }

    /* Reset NSS */
    GPIOB->BSRR |= GPIO_BSRR_BS_6;

    return len;
}

static size_t read_addr(uint32_t addr, void *data, size_t len)
{
    uint8_t *data_ptr = data;

    /* Set NSS */
    GPIOB->BSRR |= GPIO_BSRR_BR_6;

    /* Set address */
    while(!(SPI1->SR & SPI_SR_TXE))
    {
        /* Wait for data transmission */
    }
    *(volatile uint8_t *)&SPI1->DR = ((uint8_t)addr & 0x7F);
    while(!(SPI1->SR & SPI_SR_RXNE))
    {
        /* Wait for data reception */
    }
    *(volatile uint8_t *)&SPI1->DR; /* Dummy read */

    /* Read data */
    for(uint32_t i = 0; i < len; i++)
    {
        while(!(SPI1->SR & SPI_SR_TXE))
        {
            /* Wait for data transmission */
        }
        *(volatile uint8_t*)&SPI1->DR = 0; /* Send dummy data */

        while(!(SPI1->SR & SPI_SR_RXNE))
        {
            /* Wait for data reception */
        }

        data_ptr[i] = *(volatile uint8_t*)&SPI1->DR;
    }

    /* Reset NSS */
    GPIOB->BSRR |= GPIO_BSRR_BS_6;

    return len;
}

const egl_interface_t plat_rfm_iface_inst =
{
    .init = init,
    .write_addr = write_addr,
    .read_addr = read_addr,
};
