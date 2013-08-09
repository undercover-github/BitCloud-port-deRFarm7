/*****************************************************************************//**
\file  halTwi.h

\brief Declarations of twi interface.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    29/08/07 A. Khromykh - Created
**********************************************************************************/
/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/

#ifndef _HALTWI_H
#define _HALTWI_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include <halClkCtrl.h>
#include <types.h>
#include <halTaskManager.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/
#define TWI_CHANNEL_0  AT91C_BASE_TWI

// send only one byte
#define HAL_SINGLE_DATA_BYTE 1

/******************************************************************************
                   Types section
******************************************************************************/
typedef AT91PS_TWI I2cChannel_t;

/** \brief i2c baud rate */
typedef enum
{
  I2C_CLOCK_RATE_250 = F_CPU/(2*250000) - 3, // 200 Kb/s clock rate
  I2C_CLOCK_RATE_125 = F_CPU/(2*125000) - 3, // 125 Kb/s clock rate
  I2C_CLOCK_RATE_100 = F_CPU/(2*100000) - 3, // 100 Kb/s clock rate
  I2C_CLOCK_RATE_62  = F_CPU/(2*62500) - 3   // 62.5 Kb/s clock rate
} I2cClockRate_t;

/** \brief internal address size */
typedef enum
{
  /* AT91C_TWI_IADRSZ_NO */
  HAL_NO_INTERNAL_ADDRESS = (0x0ul << 8),
  /* AT91C_TWI_IADRSZ_1_BYTE */
  HAL_ONE_BYTE_SIZE       = (0x1ul << 8),
  /* AT91C_TWI_IADRSZ_2_BYTE */
  HAL_TWO_BYTE_SIZE       = (0x2ul << 8),
  /* AT91C_TWI_IADRSZ_3_BYTE */
  HAL_THREE_BYTE_SIZE     = (0x3ul << 8)
} I2cInternalAddrSize_t;

// i2c virtual state
typedef enum
{
  I2C_PACKET_CLOSE = 0,
  I2C_PACKET_IDLE,
  I2C_PACKET_WRITE_ADDRESS,
  I2C_PACKET_WRITE_DATA,
  I2C_PACKET_READ_ADDRESS,
  I2C_PACKET_READ_DATA,
  I2C_PACKET_TRANSAC_SUCCESS,
  I2C_PACKET_TRANSAC_FAIL
} I2cPacketStates_t;

typedef struct
{
  volatile uint16_t index;             // current index of read/write byte
  volatile I2cPacketStates_t state;
} HalI2cPacketService_t;

/******************************************************************************
                   Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Inits TWI module. Setup the speed of TWI.

\param[in]
  rate - the speed of TWI.
******************************************************************************/
void halInitI2c(I2cClockRate_t rate);

/******************************************************************************
start write transaction
Parameters:
  none.
Returns:
  none
******************************************************************************/
void halSendStartWriteI2c(void);

/******************************************************************************
start read transaction
Parameters:
  none.
Returns:
  none
******************************************************************************/
void halSendStartReadI2c(void);

#endif /* _I2C_H*/
// eof i2c.h
