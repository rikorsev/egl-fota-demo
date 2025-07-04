/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1232-Misc.h
 * \brief      SX1232 RF chip high level functions driver
 *
 * \remark     Optional support functions.
 *             These functions are defined only to easy the change of the
 *             parameters.
 *             For a final firmware the radio parameters will be known so
 *             there is no need to support all possible parameters.
 *             Removing these functions will greatly reduce the final firmware
 *             size.
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
#ifndef __SX1232_MISC_H__
#define __SX1232_MISC_H__

/*!
 * \brief Writes the new RF frequency value
 *
 * \param [IN] freq New RF frequency value in [Hz]
 */
void SX1232SetRFFrequency( uint32_t freq );

/*!
 * \brief Reads the current RF frequency value
 *
 * \retval freq Current RF frequency value in [Hz]
 */
uint32_t SX1232GetRFFrequency( void );

/*!
 * \brief Writes the new bitrate value
 *
 * \param [IN] bitrate New bitrate value in [bps]
 */
void SX1232SetBitrate( uint32_t bitrate );

/*!
 * \brief Reads the current bitrate value
 *
 * \retval bitrate Current bitrate value in [bps]
 */
uint32_t SX1232GetBitrate( void );

/*!
 * \brief Writes the new frequency deviation value
 *
 * \param [IN] fdev New frequency deviation value in [Hz]
 */
void SX1232SetFdev( uint32_t fdev );

/*!
 * \brief Reads the current frequency deviation value
 *
 * \retval fdev Current frequency deviation value in [Hz]
 */
uint32_t SX1232GetFdev( void );

/*!
 * \brief Writes the new RF output power value
 *
 * \param [IN] power New output power value in [dBm]
 */
void SX1232SetRFPower( int8_t power );

/*!
 * \brief Reads the current RF output power value
 *
 * \retval power Current output power value in [dBm]
 */
int8_t SX1232GetRFPower( void );

/*!
 * \brief Writes the DC offset canceller and Rx bandwidth values
 *
 * \remark For SX1232 there is no DCC setting. dccValue should be 0
 *         ie: SX1232SetDccBw( &SX1232.RegRxBw, 0, 62500 );
 *
 * \param [IN] reg Register pointer to either SX1231.RegRxBw or SX1231.RegAfcBw
 * \param [IN] dccValue New DC offset canceller value in [Hz] ( SX1231 only )
 * \param [IN] rxBwValue New Rx bandwidth value in [Hz]
 */
void SX1232SetDccBw( uint8_t* reg, uint32_t dccValue, uint32_t rxBwValue );

/*!
 * \brief Reads the current bandwidth setting
 *
 * \param [IN] reg Register pointer to either SX1231.RegRxBw or SX1231.RegAfcBw
 *
 * \retval bandwidth Bandwidth value
 */
uint32_t SX1232GetBw( uint8_t* reg );

/*!
 * \brief Enables/Disables CRC
 *
 * \param [IN] enable CRC enable/disable
 */
void SX1232SetPacketCrcOn( bool enable );

/*!
 * \brief Reads the current CRC Enable/Disbale value
 *
 * \retval enable Current CRC Enable/Disbale value
 */
bool SX1232GetPacketCrcOn( void );

/*!
 * \brief Enables/Disables AFC
 *
 * \param [IN] enable AFC enable/disable
 */
void SX1232SetAfcOn( bool enable );

/*!
 * \brief Reads the current AFC Enable/Disbale value
 *
 * \retval enable Current AFC Enable/Disbale value
 */
bool SX1232GetAfcOn( void );

/*!
 * \brief Writes the new payload length value
 *
 * \param [IN] value New payload length value
 */
void SX1232SetPayloadLength( uint8_t value );

/*!
 * \brief Reads the current payload length value
 *
 * \retval value Current payload length value
 */
uint8_t SX1232GetPayloadLength( void );

/*!
 * \brief Enables/Disables the 20 dBm PA
 *
 * \param [IN] enable [true, false]
 */
void SX1232SetPa20dBm( bool enale );

/*!
 * \brief Gets the current 20 dBm PA status
 *
 * \retval enable [true, false]
 */
bool SX1232GetPa20dBm( void );

/*!
 * \brief Writes the new PA rise/fall time of ramp up/down value
 *
 * \param [IN] value New PaRamp value
 */
void SX1232SetPaRamp( uint8_t value );

/*!
 * \brief Reads the current PA rise/fall time of ramp up/down value
 *
 * \retval value Current PaRamp value
 */
uint8_t SX1232GetPaRamp( void );

/*!
 * \brief Applies an offset to the RSSI. Compensates board components
 *
 * \param [IN] offset Offset to be applied (+/-)
 */
void SX1232SetRssiOffset( int8_t offset );

/*!
 * \brief Gets the current RSSI offset.
 *
 * \retval offset Current offset (+/-)
 */
int8_t SX1232GetRssiOffset( void );

/*!
 * \brief Writes the new value for the preamble size
 *
 * \param [IN] size New value of pramble size
 */
void SX1232SetPreambleSize( uint16_t size );

/*!
 * Reads the raw temperature
 * \retval temperature New raw temperature reading in 2's complement format
 */
int8_t SX1232GetRawTemp( void );

/*!
 * Computes the temperature compensation factor
 * \param [IN] actualTemp Actual temperature measured by an external device
 * \retval compensationFactor Computed compensation factor
 */
int8_t SX1232CalibreateTemp( int8_t actualTemp );

/*!
 * Gets the actual compensated temperature
 * \param [IN] compensationFactor Return value of the calibration function
 * \retval New compensated temperature value
 */
int8_t SX1232GetTemp( int8_t compensationFactor );

uint8_t SX1232GetFlags1( void );
uint8_t SX1232GetFlags2( void );

#endif //__SX1232_MISC_H__