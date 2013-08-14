/**************************************************************************//**
\file  eeprom.c

\brief Implementation of the I2C EEPROM interface.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    17/09/07 A. Khromykh - Created
*******************************************************************************/
#if APP_DISABLE_BSP != 1

/******************************************************************************
                   Includes section
******************************************************************************/
#include <i2ceeprom.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/
#define I2C_EEPROM_ADDRESS   0x50

/******************************************************************************
                   Types section
******************************************************************************/
typedef enum
{
  EEPROM_IDLE = 0,
  EEPROM_BUSY
} BSP_EepromState_t;

/******************************************************************************
                          Prototypes section
******************************************************************************/
static void bspEepromDone(bool result);

/******************************************************************************
                   Global variables section
******************************************************************************/
BSP_EepromState_t bspEepromState = EEPROM_IDLE;
BSP_I2c_EepromParams_t bspEepromParams;
void (* bspUserCallbackFunctions)(bool) = NULL;
static HAL_I2cDescriptor_t bspI2cEepromDesc =
{
  .tty = TWI_CHANNEL_0,
  .clockRate = I2C_CLOCK_RATE_62,
  .f = bspEepromDone,
  .id = I2C_EEPROM_ADDRESS,
  .lengthAddr = HAL_TWO_BYTE_SIZE
};

/******************************************************************************
                   Implementations section
******************************************************************************/
/******************************************************************************
Open and init i2c bus for eeprom

Retrun
  result
******************************************************************************/
int BSP_OpenI2cForEeprom(void)
{
  return HAL_OpenI2cPacket(&bspI2cEepromDesc);
}

/******************************************************************************
Close i2c bus for eeprom
Retrun
  result
******************************************************************************/
int BSP_CloseI2cForEeprom(void)
{
  return HAL_CloseI2cPacket(&bspI2cEepromDesc);
}

/******************************************************************************
BSP callback for correct work with i2c
Parameters:
  result - result i2c transaction
           successful TRUE
           other FALSE
******************************************************************************/
static void bspEepromDone(bool result)
{
  if (NULL == bspUserCallbackFunctions)
    return;
  bspEepromState = EEPROM_IDLE;
  bspUserCallbackFunctions(result);
}

/******************************************************************************
Reads some number of bytes defined by BSP_I2c_EepromParams_t from the EEPROM.
Parameters:
  params - address of BSP_I2c_EepromParams_t defined by user.
  readDone - address of callback.
Returns:
  -1 - the EEPROM has request that was not completed
******************************************************************************/
int BSP_ReadEeprom(BSP_I2c_EepromParams_t *params, void (*readDone)(bool result))
{
  if (EEPROM_IDLE != bspEepromState)
    return -1;
  if (((uint32_t)params->address + (uint32_t)params->length) > I2C_EEPROM_DATA_MEMORY_SIZE)
    return -1;

  bspI2cEepromDesc.length = params->length;
  bspI2cEepromDesc.data = params->data;
  bspI2cEepromDesc.internalAddr = params->address;

  bspUserCallbackFunctions = readDone;
  bspEepromState = EEPROM_BUSY;
  HAL_ReadI2cPacket(&bspI2cEepromDesc);
  return 0;
}

/******************************************************************************
Writes number of bytes defined by BSP_I2c_EepromParams_t to EEPROM.
Parameters:
  params - address of BSP_I2c_EepromParams_t defined by user.
  writeDone - address of callback.
Returns:
  -1 - the EEPROM has request that was not completed.
******************************************************************************/
int BSP_WriteEeprom(BSP_I2c_EepromParams_t *params, void (*writeDone)(bool result))
{
  if (EEPROM_IDLE != bspEepromState)
    return -1;
  if (((uint32_t)params->address + (uint32_t)params->length) > I2C_EEPROM_DATA_MEMORY_SIZE)
    return -1;

  bspI2cEepromDesc.length = params->length;
  bspI2cEepromDesc.data = params->data;
  bspI2cEepromDesc.internalAddr = params->address;

  bspUserCallbackFunctions = writeDone;
  bspEepromState = EEPROM_BUSY;
  HAL_WriteI2cPacket(&bspI2cEepromDesc);
  return 0;
}

#endif // APP_DISABLE_BSP != 1

//eof i2ceeprom.c
