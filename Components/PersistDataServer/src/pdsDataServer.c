/***************************************************************************//**
  \file pdsDataServer.c

  \brief Persistence Data Server implementation

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    22/01/08 A. Khromykh - Created
    01/11/10 A. Razinkov - Modified
  Last change:
    $Id: pdsDataServer.c 21048 2012-05-03 13:20:33Z dkolmakov $
*****************************************************************************/
/******************************************************************************
                   Includes section
******************************************************************************/
#include <pdsDataServer.h>
#include <pdsCrcService.h>
#include <pdsMemAccess.h>
#include <pdsWriteData.h>
#include <taskManager.h>
#include <pdsMem.h>
#include <pdsDbg.h>
#include <configServer.h>

#ifdef _COMMISSIONING_

/*****************************************************************************
                               Types section
******************************************************************************/
/* Enumeration of memory by it's ability to be stored/restored */
typedef enum _PDS_MemoryAllowedType_t
{
  ALLOWED_FOR_STORING,
  ALLOWED_FOR_RESTORING
} PDS_MemoryAllowedType_t;

/****************************************************************************
                              Static functions prototypes section
******************************************************************************/
static bool pdsGetMemoryAllowed(PDS_MemoryAllowedType_t type, PDS_MemMask_t memoryMask);

/******************************************************************************
                   Implementations section
******************************************************************************/
/***************************************************************************//**
\brief Restores data from non-volatile memory (EEPROM)

\ingroup pds

PDS files not included in the current build configuration will be ignored. 
Restoring process will be performed only for PDS files present in EEPROM.

\param[in] memoryId - an identifier of PDS file or directory to be restored 
                      from non-volatile memory.

\return true, if at leeast one file has been restored, false - otherwise.
*******************************************************************************/
bool PDS_Restore(PDS_MemId_t memoryId)
{
  PDS_MemMask_t memoryMask;
  
  PDS_MEM_MASK_CLEAR_ALL(memoryMask);
  PDS_MEM_MASK_SET_BIT(memoryMask, memoryId);
  pdsExpandMemoryMask(memoryMask);

  /* Ignore all memory, unexistent for current build configuration */
  pdsGetMemoryAllowed(ALLOWED_FOR_STORING, memoryMask);

  if (pdsMemMaskIsAnyBitSet(memoryMask))
  {
    /* Obtain a non-volatile storage content */
    pdsGetMemoryAllowed(ALLOWED_FOR_RESTORING, memoryMask);

    /* If required data exists - try to update from non-volatile memory */
    if (pdsMemMaskIsAnyBitSet(memoryMask))
      return PDS_SUCCESS == pdsUpdate(memoryMask);
  }

  /* Required region(s) wasn't found in non-volatile memory or error occured */
  return false;
}

/***************************************************************************//**
\brief Stores data in non-volatile memory in background, in such a way that
       this application execution is not blocked and continues

\ingroup pds

PDS files not included in the current build configuration will be ignored. 
Storing process will be performed only for PDS files present in EEPROM.

\param[in] memoryId - an identifier of PDS file or directory to be stored 
                      in non-volatile memory.

\return True, if storing process has begun, false - otherwise.
******************************************************************************/
bool PDS_Store(PDS_MemId_t memoryId)
{
  pdsAddDataForCommitment(memoryId);

  if (!(pdsMemory()->status & PDS_COMMITMENT_INPROGRESS_STATUS))
    pdsStartCommitment();

  return true;
}

/***************************************************************************//**
\brief Stores data in non-volatile memory in background in a synchronous way -
       the application execution will be blocked until the process is completed

\ingroup pds
       
PDS files not included in the current build configuration will be ignored. 
Storing process will be performed only for PDS files present in EEPROM.

\param[in] memoryId - an identifier of PDS file or directory to be stored 
                      in non-volatile memory.

\return True, if storing process has been performed successfully, false - otherwise.
******************************************************************************/
bool PDS_BlockingStore(PDS_MemId_t memoryId)
{
  if (PDS_Store(memoryId))
  {
    while (pdsMemory()->status & PDS_COMMITMENT_INPROGRESS_STATUS)
      SYS_ForceRunTask();

    return true;
  }

  return false;
}

/***************************************************************************//**
\brief Checks if the specified PDS file or directory can be restored
       from non-volatile memory

\ingroup pds

PDS files not included in the current build configuration will be ignored. 

\param[in] memoryId - an identifier of PDS file or directory to be checked. 

\return true, if the specified memory can be restored; false - otherwise.
******************************************************************************/
bool PDS_IsAbleToRestore(PDS_MemId_t memoryId)
{
  PDS_MemMask_t memoryMask;
  
  pdsInitMemMask(memoryId, memoryMask);
  /* Ignore all memory, unexistent for current build configuration */
  pdsGetMemoryAllowed(ALLOWED_FOR_STORING, memoryMask);

  if (pdsMemMaskIsAnyBitSet(memoryMask))
  {
    /* Obtain a non-volatile storage content */
    pdsGetMemoryAllowed(ALLOWED_FOR_RESTORING, memoryMask);

    if (pdsMemMaskIsAnyBitSet(memoryMask))
      return true;
  }

  return false;
}

/***************************************************************************//**
\brief Checks if the specified PDS file or directory can be stored
       in non-volatile memory

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be checked. 

\return true, if the specified memory can be stored; false - otherwise.
*******************************************************************************/
bool PDS_IsAbleToStore(PDS_MemId_t memoryId)
{
  PDS_MemMask_t memoryMask;
  
  pdsInitMemMask(memoryId, memoryMask);
  /* Ignore all memory, unexistent for current build configuration */
  pdsGetMemoryAllowed(ALLOWED_FOR_STORING, memoryMask);

  if (pdsMemMaskIsAnyBitSet(memoryMask))
    return true;

  return false;
}

/***************************************************************************//**
\brief Returns mask of PDS files, which allowed to be stored in
       (or restored from) non-volatile storage.
       Particular set depends on build configuration and actual storage content.

\param[in] type - request type (ALLOWED_FOR_STORING / ALLOWED_FOR_RESTORING).
\param[out] memoryMask - mask of PDS files.

\return true if operation has been successful, false otherwise.
*******************************************************************************/
static bool pdsGetMemoryAllowed(PDS_MemoryAllowedType_t type, PDS_MemMask_t memoryMask)
{
  switch (type)
  {
    case ALLOWED_FOR_STORING:
      for (uint8_t i = 0; i < PDS_MEM_MASK_LENGTH; i++)
        memoryMask[i] &= pdsMemory()->allowedForStoring[i];
      return true;
      break;

    case ALLOWED_FOR_RESTORING:
      /* Check if required data exists in non-volatile memory */
      return (PDS_SUCCESS == pdsCheckStorage(memoryMask));
      break;

    default:
      break;
  }

  return false;
}

/***************************************************************************//**
\brief Checks if files specified have been restored from non-volatile memory
       during the latest restoring procedure

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be checked. 

\return true, if the specified PDT(s) have been restored; false - otherwise.
******************************************************************************/
bool PDS_IsMemoryRestored(PDS_MemId_t memoryId)
{
  PDS_MemMask_t memoryMask;
  
  pdsInitMemMask(memoryId, memoryMask);
  /* Ignore all memory, unexistent for current build configuration */
  pdsGetMemoryAllowed(ALLOWED_FOR_STORING, memoryMask);

  if (pdsMemMaskIsAnyBitSet(memoryMask))
  {
    for (uint8_t i = 0; i < PDS_MEM_MASK_LENGTH; i++)
      memoryMask[i] &= pdsMemory()->restoredMemory[i];

    if (pdsMemMaskIsAnyBitSet(memoryMask))
      return true;
  }

  return false;
}

/***************************************************************************//**
\brief Marks the specified restored PDS files to be cleared during a reset phase
      (during ZDO_StartNetwork() execution)

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be cleared. 
******************************************************************************/
void PDS_ClearRestoredMemory(PDS_MemId_t memoryId)
{
  PDS_MemMask_t memoryMask;
  
  pdsInitMemMask(memoryId, memoryMask);

  for (uint8_t i = 0; i < PDS_MEM_MASK_LENGTH; i++)
    pdsMemory()->restoredMemory[i] &= ~memoryMask[i];
}

/*****************************************************************************//**
\brief Resets the specified areas in non-volatile memory. All data stored in
       these areas will be lost.

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be cleared 
                      in non-volatile memory.

\return - PDS state as an operation result.
*******************************************************************************/
PDS_DataServerState_t PDS_ResetStorage(PDS_MemId_t memoryId)
{
  PDS_MemMask_t memoryMask;
  
  PDS_MEM_MASK_CLEAR_ALL(memoryMask);
  PDS_MEM_MASK_SET_BIT(memoryMask, memoryId);

  return (PDS_SUCCESS == pdsCorruptFiles(memoryMask));
}

/******************************************************************************
\brief This function is deprecated - use PDS_BlockingStore() instead.
******************************************************************************/
void PDS_FlushData(PDS_MemId_t memoryId)
{
  PDS_BlockingStore(memoryId);
}

#endif /* _COMMISSIONING_ */

/******************************************************************************
\brief Must be called from the ::APL_TaskHandler() function only.\n
Reads data from the user area of EEPROM.

\param[in] offset   - data offset
\param[in] data     - pointer to user data area
\param[in] length   - data length
\param[in] callback - pointer to a callback function; if callback is NULL,
                      then data will be read syncronously

\return - PDS state as a result of data reading operation
*******************************************************************************/
PDS_DataServerState_t PDS_ReadUserData(uint16_t offset, uint8_t *data,
                                       uint16_t length, void (*callback)(void))
{
  MEMORY_DESCRIPTOR descriptor;
  PDS_DataServerState_t status;

  descriptor.address = USER_BASE_STORAGE_ADDRESS + offset;
  descriptor.length = length;
  descriptor.data = data;

  while (pdsMemory()->status & PDS_COMMITMENT_INPROGRESS_STATUS)
  {
    SYS_ForceRunTask();
  }

  if (NULL == callback)
    callback = pdsDummyCallback;

  status = pdsRead(&descriptor, callback);
  if (status != PDS_SUCCESS)
    return status;

  return PDS_SUCCESS;
}

/******************************************************************************
\brief Must be called only from ::APL_TaskHandler() function.\n
Writes data to user area of EEPROM.

\param[in] offset   - data offset
\param[in] data     - pointer to user data area
\param[in] length   - data length
\param[in] callback - pointer to a callback function; if callback is NULL,
                      then data will be written syncronously

\return - PDS state as a result of data writing operation
*******************************************************************************/
PDS_DataServerState_t PDS_WriteUserData(uint16_t offset, uint8_t *data,
                                        uint16_t length, void (*callback)(void))
{
  MEMORY_DESCRIPTOR descriptor;
  PDS_DataServerState_t status;

  descriptor.address = USER_BASE_STORAGE_ADDRESS + offset;
  descriptor.length = length;
  descriptor.data = data;

  while (pdsMemory()->status & PDS_COMMITMENT_INPROGRESS_STATUS)
  {
    SYS_ForceRunTask();
  }

  if (NULL == callback)
    callback = pdsDummyCallback;

  status = pdsWrite(&descriptor, callback);
  if (status != PDS_SUCCESS)
    return status;

  return PDS_SUCCESS;
}
// eof pdsDataServer.c
