/**************************************************************************//**
  \file pdsTimer.c

  \brief Persistent Data Server timer implementation.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
   History:
    2012-03-14 A. Razinkov  - Created.
   Last change:
    $Id: pdsTimer.c 19835 2012-01-24 16:51:08Z arazinkov $
 ******************************************************************************/
/******************************************************************************
                               Includes section
 ******************************************************************************/
#include <pdsDataServer.h>
#include <pdsDbg.h>
#include <pdsMem.h>
#include <taskManager.h>

#ifdef _COMMISSIONING_

/******************************************************************************
                    Types section
******************************************************************************/

/******************************************************************************
                    Prototypes section
******************************************************************************/
static void pdsTimerFired(void);

/******************************************************************************
                    Static variables section
******************************************************************************/

/******************************************************************************
                   Implementation section
******************************************************************************/
/******************************************************************************
\brief PDS timer initialization routine.
******************************************************************************/
void pdsTimerInit(void)
{
  pdsMemory()->periodicCommitTimer.mode = TIMER_REPEAT_MODE;
  pdsMemory()->periodicCommitTimer.callback = pdsTimerFired;
}

/***************************************************************************//**
\brief Enables periodic commitment of specified persistent data tables (PDTs) 
       to non-volatile memory

\ingroup pds

\param[in] interval - period of time between successive commitments
\param[in] memoryId - an identifier of PDS file or directory to be stored 
                      in non-volatile memory.
******************************************************************************/
void PDS_StoreByTimer(uint32_t interval, PDS_MemId_t memoryId)
{
  HAL_StopAppTimer(&pdsMemory()->periodicCommitTimer);

  /* Disable periodic commitment, if zero passed as interval */
  if (!interval)
  {
    pdsMemory()->memIdToStoreByTimer = PDS_NO_MEMORY_SPECIFIED;
    return;
  }

  /* Reconfigure and start periodic commitment */
  pdsMemory()->periodicCommitTimer.interval = interval;
  pdsMemory()->memIdToStoreByTimer = memoryId;
  HAL_StartAppTimer(&pdsMemory()->periodicCommitTimer);
}

/******************************************************************************
\brief PDS timer expiration callback.
******************************************************************************/
static void pdsTimerFired(void)
{
  PDS_Store(pdsMemory()->memIdToStoreByTimer);
}

/***************************************************************************//**
\brief Stops the Persistent Data Server. This doesn't affect a policy created
       through PDS_StoreByEvent() of  PDS_StoreByTimer() API.
       After calling PDS_Run() function PDS continues it's work, as before the
       stopping.
*******************************************************************************/
void PDS_Stop(void)
{
  HAL_StopAppTimer(&pdsMemory()->periodicCommitTimer);

  while (pdsMemory()->status & PDS_COMMITMENT_INPROGRESS_STATUS)
  {
    SYS_ForceRunTask();
  }

  pdsMemory()->status |= PDS_STOPPED_STATUS;
}

/***************************************************************************//**
\brief Continue Persistent Data Server operation the same way, as before a
       stopping.
*******************************************************************************/
void PDS_Run(void)
{
  if (pdsMemory()->status & PDS_STOPPED_STATUS)
  {
    HAL_StartAppTimer(&pdsMemory()->periodicCommitTimer);
    pdsMemory()->status ^= PDS_STOPPED_STATUS;
  }
}

#endif /* _COMMISSIONING_ */
/* eof pdsTimer.c */

