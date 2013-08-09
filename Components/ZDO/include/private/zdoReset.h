/**************************************************************************//**
  \file zdoReset.h

  \brief Interface for ZDO reset routine

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    29.10.09 A. Taradov - Created
******************************************************************************/

#ifndef _ZDORESET_H
#define _ZDORESET_H

/******************************************************************************
                        Definitions section
******************************************************************************/
typedef struct
{
  void (*stackResetCallback)(ZDO_Status_t status);
} ZdoReset_t;

/******************************************************************************
                   Prototypes section
******************************************************************************/
/******************************************************************************
  \brief ZDO reset routine
******************************************************************************/
ZDO_PRIVATE void zdoReset(void);

/**************************************************************************//**
  \brief Stack resetting start routine.

  \param[in] powerFailureControl - stack restoring after power failure control bitfield;
                                  affects on initialization procedure.
  \param[out] callback - callback to stack reset finished handler.

  \return None.
 ******************************************************************************/
ZDO_PRIVATE void zdoStackReset(NWK_PowerFailureControl_t powerFailureControl, void (*callback)(ZDO_Status_t status));

/**************************************************************************//**
  \brief Reset component initialization routine.
 ******************************************************************************/
ZDO_PRIVATE void zdoInitResetComponent(void);

#endif // _ZDORESET_H

// eof zdoReset.h
