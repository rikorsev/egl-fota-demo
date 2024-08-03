#include "egl_lib.h"
#include "plat.h"

egl_result_t init(uint32_t poly, uint32_t init)
{
    /* Enable clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;

    CRC->POL = poly;
    CRC->INIT = init;

    /* Revert input */
    CRC->CR &= ~CRC_CR_REV_IN_Msk;
    CRC->CR |= CRC_CR_REV_IN_0;
    CRC->CR |= CRC_CR_REV_IN_1;

    /* Revert output */
    CRC->CR |= CRC_CR_REV_OUT;

    /* Reset the CRC value */
    CRC->CR |= CRC_CR_RESET;

    return EGL_SUCCESS;
}

uint32_t calc32(void *data, size_t len)
{
    for(int i = 0; i < len/sizeof(uint32_t); i++)
    {
        CRC->DR = ((uint32_t *)data)[i];
    }

    return CRC->DR;
}

egl_result_t reset(void)
{
    CRC->CR |= CRC_CR_RESET;

    return EGL_SUCCESS;
}

static const egl_crc_t plat_crc_inst =
{
    .init = init,
    .reset = reset,
    .calc32 = calc32
};

egl_crc_t *plat_crc_get(void)
{
    return (egl_crc_t *)&plat_crc_inst;
}
