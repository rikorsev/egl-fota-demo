#include "egl_lib.h"
#include "plat.h"

/* PB15 */
static void init_mosi_pin(void)
{
    /* Configure the GPIO as an alternative function */
    GPIOB->MODER &= ~GPIO_MODER_MODE15_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE15_1;

    /* Configure as push/pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT15_Msk;

    /* Configure Very High Speed */
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED15_Msk;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED15_0;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED15_1;

    /* Configure no pull up/down */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD15_Msk;

    /* Set alternative function number */
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL15_Msk;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL15_0;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL15_2;
}

/* PB14 */
static void init_miso_pin(void)
{
    /* Configure the GPIO as an alternative function */
    GPIOB->MODER &= ~GPIO_MODER_MODE14_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE14_1;

    /* Configure as push/pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT14_Msk;

    /* Configure Very High Speed */
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED14_Msk;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED14_0;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED14_1;

    /* Configure no pull up/down */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD14_Msk;

    /* Set alternative function number */
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL14_Msk;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL14_0;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL14_2;
}

/* PB13 */
static void init_sck_pin(void)
{
    /* Configure the GPIO as an alternative function */
    GPIOB->MODER &= ~GPIO_MODER_MODE13_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE13_1;

    /* Configure as push/pull output */
    GPIOB->OTYPER &= ~GPIO_OTYPER_OT13_Msk;

    /* Configure Very High Speed */
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED13_Msk;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED13_0;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED13_1;

    /* Configure no pull up/down */
    GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD13_Msk;

    /* Set alternative function number */
    GPIOB->AFR[1] &= ~GPIO_AFRH_AFSEL13_Msk;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL13_0;
    GPIOB->AFR[1] |= GPIO_AFRH_AFSEL13_2;
}

static void init_pio(void)
{
    /* Enable GPIOB clock */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    init_mosi_pin();
    init_miso_pin();
    init_sck_pin();
}

static void init_spi(void)
{
    /* Enable SPI clock */
    RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;

    /* Set SSN configuration */
    SPI2->CR1 |= SPI_CR1_SSM;
    SPI2->CR1 |= SPI_CR1_SSI;

    /* Set master mode */
    SPI2->CR1 |= SPI_CR1_MSTR;

    /* Set Data size */
    SPI2->CR2 &= ~SPI_CR2_DS;
    SPI2->CR2 |= SPI_CR2_DS_0;
    SPI2->CR2 |= SPI_CR2_DS_1;
    SPI2->CR2 |= SPI_CR2_DS_2;

    SPI2->CR2 |= SPI_CR2_FRXTH;

    /* Enable SPI */
    SPI2->CR1 |= SPI_CR1_SPE;
}

static egl_result_t init(void)
{
    init_pio();
    init_spi();

    return EGL_SUCCESS;
}

static size_t write(void *data, size_t len)
{
    uint8_t dummy;

    for(uint32_t i = 0; i < len; i++)
    {
        while(!(SPI2->SR & SPI_SR_TXE))
        {
            /* Wait for data transmission */
        }
        *(volatile uint8_t*)&SPI2->DR = ((uint8_t *)data)[i];

        while(!(SPI2->SR & SPI_SR_RXNE))
        {
            /* Wait for data reception */
        }
        dummy = *(volatile uint8_t*)&SPI2->DR;
    }

    return len;
}

static size_t read(void *data, size_t len)
{
    uint8_t *data_ptr = data;

    for(uint32_t i = 0; i < len; i++)
    {
        while(!(SPI2->SR & SPI_SR_TXE))
        {
            /* Wait for data transmission */
        }
        *(volatile uint8_t*)&SPI2->DR = 0; /* Send dummy data */

        while(!(SPI2->SR & SPI_SR_RXNE))
        {
            /* Wait for data reception */
        }

        data_ptr[i] = *(volatile uint8_t*)&SPI2->DR;
    }

    return len;
}

static const egl_interface_t iface_rfm_iface_inst =
{
    .init = init,
    .write = write,
    .read = read,
};

egl_interface_t *iface_rfm_iface_get(void)
{
    return (egl_interface_t *)&iface_rfm_iface_inst;
}

