/******************************************************************************
  \file pdsWriteData.h

  \brief
    Persistent Periodic data save implementation header

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2010 , Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    29.10.10 A. Razinkov - Created.
  Last change:
    $Id: pdsWriteData.h 21048 2012-05-03 13:20:33Z dkolmakov $
******************************************************************************/

#ifndef _PDSWRITEDATA_H_
#define _PDSWRITEDATA_H_

/******************************************************************************
                    Includes section
******************************************************************************/
#include <pdsMemAbstract.h>

#ifdef _COMMISSIONING_
/******************************************************************************
                    Functions prototypes section
******************************************************************************/
/******************************************************************************
\brief Updates persistent items values in RAM from non-volatile memory.
\
\param[in] memoryToRestore - bitmask of non-volatile memory contents.
\
\return Operation result.
******************************************************************************/
PDS_DataServerState_t pdsUpdate(const uint8_t *const memoryToRestore);

/******************************************************************************
\brief Adds file or directory to the commitment list for further storing in 
       non-volatile memory.

\param[in] memoryId - file or directory memory identifier.
******************************************************************************/
void pdsAddDataForCommitment(PDS_MemId_t memoryId);

/******************************************************************************
\brief Starts commitment to a non-volatile storage.
******************************************************************************/
void pdsStartCommitment(void);

/******************************************************************************
\brief Resets specified area in non-volatile memory. All stored data will be lost.

\return Operation result.
******************************************************************************/
PDS_DataServerState_t pdsCorruptFiles(const uint8_t *const memoryMask);

#endif /* _COMMISSIONING_ */

/******************************************************************************
\brief Writes data to non-volatile memory.
\
\param[in] descriptor - memory descriptor to store parameter value
\param[out]callback - callback to write-finidhed event handler.
******************************************************************************/
PDS_DataServerState_t pdsWrite(MEMORY_DESCRIPTOR* descriptor, void (*callback)(void));

/******************************************************************************
\brief Read data from non-volatile memory.
\
\param[in] descriptor - memory descriptor to read parameter value.
\param[out]callback - callback to read-finished event handler.
******************************************************************************/
PDS_DataServerState_t pdsRead(MEMORY_DESCRIPTOR* descriptor, void (*callback)(void));

/******************************************************************************
\brief Wait until memory be ready for transaction.
******************************************************************************/
void pdsWaitMemoryFree(void);

/*******************************************************************************
\brief Dummy callback.
*******************************************************************************/
void pdsDummyCallback(void);

#endif /* _PDSWRITEDATA_H_ */
