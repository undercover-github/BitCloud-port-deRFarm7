/**************************************************************************//**
\file  halAdc.h

\brief Declaration of hardware depended ADC interface.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    03/09/07 A. Khromykh - Created
*******************************************************************************/
/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/

#ifndef _HALADC_H
#define _HALADC_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include <halTaskManager.h>
#include <halClkCtrl.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/
#define ADC_0  AT91C_BASE_ADC

#define ALL_CHANNEL_DISABLE  ALL_PERIPHERIAL_INTERRUPT_DISABLE

/******************************************************************************
                   Types section
******************************************************************************/
typedef AT91PS_ADC AdcHwChannel_t;

/**************************************************************************//**
\brief adc sample rate
******************************************************************************/
typedef enum
{
  ADC_533KSPS,
  ADC_429KSPS,
  ADC_369KSPS,
  ADC_250KSPS,
  ADC_136KSPS,
  ADC_68KSPS,
  ADC_34KSPS
} HAL_AdcSampleRate_t;

/**************************************************************************//**
\brief adc resolution
******************************************************************************/
typedef enum
{
  RESOLUTION_8_BIT,
  RESOLUTION_10_BIT
} HAL_AdcResolution_t;

/**************************************************************************//**
\brief channel number.
******************************************************************************/
typedef enum
{
  HAL_ADC_CHANNEL0 = (1 <<  0),
  HAL_ADC_CHANNEL1 = (1 <<  1),
  HAL_ADC_CHANNEL2 = (1 <<  2),
  HAL_ADC_CHANNEL3 = (1 <<  3),
  HAL_ADC_CHANNEL4 = (1 <<  4),
  HAL_ADC_CHANNEL5 = (1 <<  5),
  HAL_ADC_CHANNEL6 = (1 <<  6),
  HAL_ADC_CHANNEL7 = (1 <<  7)
} HAL_AdcChannelNumber_t;

// fake types to be compatible with others platform
typedef uint8_t HAL_AdcVoltageReference_t;
typedef uint8_t HalAdcService_t;

/******************************************************************************
                   Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Set up parametres to ADC.
******************************************************************************/
void halOpenAdc(void);

/**************************************************************************//**
\brief starts convertion on the ADC channel.
\param[in]
  channel - channel number.
******************************************************************************/
void halStartAdc(HAL_AdcChannelNumber_t channel);

/**************************************************************************//**
\brief Closes the ADC.
******************************************************************************/
void halCloseAdc(void);

/******************************************************************************
                   Inline static functions section
******************************************************************************/
/**************************************************************************//**
\brief  adcHandler interrupt handler signal implementation
******************************************************************************/
static inline void halSigAdcInterrupt(void)
{
  halPostTask(HAL_ADC);
}
#endif /* _HALADC_H */

// eof halAdc.h
