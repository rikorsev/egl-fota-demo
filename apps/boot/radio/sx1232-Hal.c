#include <stdint.h>
#include <stdbool.h> 

#include "sx1232-Hal.h"

#include "egl_lib.h"
#include "plat.h"

#define EGL_MODULE_NAME "radio-hal"

void SX1232InitIo( void )
{
    egl_result_t result;

    result = egl_pio_init(PLAT_RFM_RESET_PIN);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm reset pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_NSS_PIN);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm reset pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_set(PLAT_RFM_NSS_PIN, true);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set rfm nss pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_DIO0);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm dio0 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_DIO2);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm dio2 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_DIO4);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm dio4 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_init(PLAT_RFM_DIO5);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm dio5 pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_itf_init(PLAT_RFM_IFACE);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to init rfm interface. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }
}

void SX1232SetReset( uint8_t state )
{
    egl_result_t result = egl_pio_set(PLAT_RFM_RESET_PIN, (bool)state);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set rfm reset pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }
}

void SX1232Write( uint8_t addr, uint8_t data )
{
    SX1232WriteBuffer( addr, &data, 1 );
}

void SX1232Read( uint8_t addr, uint8_t *data )
{
    SX1232ReadBuffer( addr, data, 1 );
}

void SX1232WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    egl_result_t result;
    size_t addr_size = sizeof(addr);
    size_t data_size = (size_t)size;

    /* Set write bit */
    addr |= 0x80;

    result = egl_pio_set(PLAT_RFM_NSS_PIN, false);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set rfm nss pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_itf_write(PLAT_RFM_IFACE, &addr, &addr_size);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to write address. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_itf_write(PLAT_RFM_IFACE, buffer, &data_size);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to write data. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_set(PLAT_RFM_NSS_PIN, true);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set rfm nss pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }
}

void SX1232ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    egl_result_t result;
    size_t addr_size = sizeof(addr);
    size_t data_size = (size_t)size;

    /* Clear write bit */
    addr &= 0x7F;

    result = egl_pio_set(PLAT_RFM_NSS_PIN, false);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set rfm nss pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_itf_write(PLAT_RFM_IFACE, &addr, &addr_size);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to write address. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_itf_read(PLAT_RFM_IFACE, buffer, &data_size);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to read data. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }

    result = egl_pio_set(PLAT_RFM_NSS_PIN, true);
    if(result != EGL_SUCCESS)
    {
        EGL_TRACE_ERROR("Fail to set rfm nss pin. Result %s", EGL_RESULT(result));
        EGL_RESULT_FATAL();
    }
}

void SX1232WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1232WriteBuffer( 0, buffer, size );
}

void SX1232ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1232ReadBuffer( 0, buffer, size );
}

uint8_t SX1232ReadDio0( void )
{
    egl_result_t result = egl_pio_get(PLAT_RFM_DIO0);
    if(result != EGL_SET || result != EGL_RESET)
    {
        EGL_TRACE_ERROR("Fail to Get DIO0 pin. Result: %s", EGL_RESULT(result));
    }

    return result == EGL_SET ? 1 : 0;
}

uint8_t SX1232ReadDio1( void )
{
    /* Not connected */
    return 0;
}

uint8_t SX1232ReadDio2( void )
{
    egl_result_t result = egl_pio_get(PLAT_RFM_DIO2);
    if(result != EGL_SET || result != EGL_RESET)
    {
        EGL_TRACE_ERROR("Fail to Get DIO0 pin. Result: %s", EGL_RESULT(result));
    }

    return result == EGL_SET ? 1 : 0;
}

uint8_t SX1232ReadDio3( void )
{
    /* Not connected */
    return 0;
}

uint8_t SX1232ReadDio4( void )
{
    egl_result_t result = egl_pio_get(PLAT_RFM_DIO4);
    if(result != EGL_SET || result != EGL_RESET)
    {
        EGL_TRACE_ERROR("Fail to Get DIO0 pin. Result: %s", EGL_RESULT(result));
    }

    return result == EGL_SET ? 1 : 0;
}

uint8_t SX1232ReadDio5( void )
{
    egl_result_t result = egl_pio_get(PLAT_RFM_DIO5);
    if(result != EGL_SET || result != EGL_RESET)
    {
        EGL_TRACE_ERROR("Fail to Get DIO0 pin. Result: %s", EGL_RESULT(result));
    }

    return result == EGL_SET ? 1 : 0;
}
