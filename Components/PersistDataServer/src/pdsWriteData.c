/**************************************************************************//**
  \file pdsWriteData.c

  \brief Periodic data save implementation

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    22/01/08 A. Khromykh - Created
    01/11/10 A. Razinkov - Modified
  $Id: pdsWriteData.c 21048 2012-05-03 13:20:33Z dkolmakov $
*****************************************************************************/
/******************************************************************************
                   Includes section
******************************************************************************/
#include <pdsCrcService.h>
#include <pdsMemAccess.h>
#include <leds.h>
#include <pdsDbg.h>
#include <pdsWriteData.h>
#include <taskManager.h>
#include <pdsMem.h>
#include <bcEvents.h>

#ifdef _COMMISSIONING_
/******************************************************************************
                   Prototypes section
******************************************************************************/
static PDS_DataServerState_t pdsCommit(void);
static bool pdsCommitStarted(PDS_MemId_t memoryId, MEMORY_DESCRIPTOR *fileDataDescr);

/******************************************************************************
                   Implementation section
******************************************************************************/
/******************************************************************************
\brief Updates persistent items values in RAM from non-volatile memory.
\
\param[in] memoryToRestore - bitmask of non-volatile memory contents.
\
\return Operation result.
******************************************************************************/
PDS_DataServerState_t pdsUpdate(const uint8_t *const memoryToRestore)
{
  PDS_DataServerState_t status;
  MEMORY_DESCRIPTOR fileDataDescr;
  PdsDataAccessContext_t accessContext;

  pdsPrepareAccessContext(&accessContext, memoryToRestore);
  // Clear restored memory mask
  PDS_MEM_MASK_CLEAR_ALL(pdsMemory()->restoredMemory);
  
  while (pdsPrepareMemoryAccess(&accessContext, &fileDataDescr))
    if (PDS_SUCCESS == pdsCheckFile(accessContext.memoryId, &fileDataDescr))
    {
      status = pdsRead(&fileDataDescr, pdsDummyCallback);
      if (status != PDS_SUCCESS)
        return status;
      // Update restored memory mask
      PDS_MEM_MASK_SET_BIT(pdsMemory()->restoredMemory, accessContext.memoryId);
    }

  return PDS_SUCCESS;
}

/******************************************************************************
\brief Adds file or directory to the commitment list for further storing in 
       non-volatile memory.

\param[in] memoryId - file or directory memory identifier.
******************************************************************************/
void pdsAddDataForCommitment(PDS_MemId_t memoryId)
{
  PDS_MemMask_t memoryMask;
  
  PDS_MEM_MASK_CLEAR_ALL(memoryMask);
  PDS_MEM_MASK_SET_BIT(memoryMask, memoryId);
  pdsExpandMemoryMask(memoryMask);

  for (uint8_t i = 0; i < PDS_MEM_MASK_LENGTH; i++)
    pdsMemory()->currentCommitmentList[i] |= memoryMask[i];

  pdsMemory()->status |= PDS_COMMITMENT_OUT_OFF_DATE_STATUS;
}

/******************************************************************************
\brief Starts commitment to a non-volatile storage.
******************************************************************************/
void pdsStartCommitment(void)
{
#ifdef PDS_HIGHLIGHT_WRITING_PROCESS
  BSP_OnLed(LED_YELLOW);
#endif
  pdsMemory()->status |= PDS_COMMITMENT_INPROGRESS_STATUS;

  if (PDS_SUCCESS == pdsCommit())
  {
    pdsMemory()->status &= ~(PDS_COMMITMENT_INPROGRESS_STATUS);

#ifdef PDS_HIGHLIGHT_WRITING_PROCESS
    BSP_OffLed(LED_YELLOW);
#endif
    SYS_PostEvent(BC_EVENT_STORING_FINISHED, 0);
  }
}

/******************************************************************************
\brief Commit persistent memory changes (if any) from RAM to non-volatile memory.
\      Process comes iterative way - when some item stored, this function will
\      be called as a callback.
\
\return Operation result.
******************************************************************************/
static PDS_DataServerState_t pdsCommit(void)
{
  MEMORY_DESCRIPTOR fileDataDescr;
  static PdsDataAccessContext_t accessContext;

  /* If current commit is already out-of-date, restart operation */
  if (PDS_COMMITMENT_OUT_OFF_DATE_STATUS & pdsMemory()->status)
  {
    pdsPrepareAccessContext(&accessContext, pdsMemory()->currentCommitmentList);
    pdsMemory()->status &= ~PDS_COMMITMENT_OUT_OFF_DATE_STATUS;
  }

  while (pdsPrepareMemoryAccess(&accessContext, &fileDataDescr))
  {
    PDS_MEM_MASK_CLEAR_BIT(pdsMemory()->currentCommitmentList, accessContext.memoryId);

    if (pdsCommitStarted(accessContext.memoryId, &fileDataDescr))
      return PDS_COMMIT_IN_PROGRESS;
  }
  
  return PDS_SUCCESS;
}

/******************************************************************************
\brief Commit item value changes (if any) from Configuration Server to persist
\      memory.
\
\param[in] descriptor - memory descriptor to store parameter value.

\return true - if changes found and commit started; false - otherwise.
******************************************************************************/
static bool pdsCommitStarted(PDS_MemId_t memoryId, MEMORY_DESCRIPTOR *fileDataDescr)
{
  uint8_t data;
  bool commitNeeded = false;
  PDS_FileCrc_t ramDataCrc;
  PDS_FileHeader_t header;
  MEMORY_DESCRIPTOR accessDescriptor;

  ramDataCrc = pdsCalculateRAMDataCrc(PDS_CRC_INITIAL_VALUE, fileDataDescr->data, fileDataDescr->length);

  accessDescriptor.data = (uint8_t *)&header;
  accessDescriptor.length = sizeof(PDS_FileHeader_t);
  accessDescriptor.address = fileDataDescr->address - sizeof(PDS_FileHeader_t);
  pdsRead(&accessDescriptor, pdsDummyCallback);

  if (ramDataCrc == header.crc &&
      memoryId == header.memoryId &&
      fileDataDescr->length == header.size)
  {
    accessDescriptor.data = &data;
    accessDescriptor.length = 1;
    /* Check for any changes in byte-by-byte order */
    for (uint16_t i = 0; i < fileDataDescr->length; i++)
    {
      accessDescriptor.address = fileDataDescr->address + i;
      pdsRead(&accessDescriptor, pdsDummyCallback);
      if (*accessDescriptor.data != *((fileDataDescr->data) + i))
      {
        commitNeeded = true;
        break;
      }
    }
  }
  else
  {
    commitNeeded = true;
  }
  
  /* Start rewrite out of date data in persist memory*/
  if (commitNeeded)
  {
    header.crc = ramDataCrc;
    header.memoryId = memoryId;
    header.size = fileDataDescr->length;
    accessDescriptor.data = (uint8_t *)&header;
    accessDescriptor.length = sizeof(PDS_FileHeader_t);
    accessDescriptor.address = fileDataDescr->address - sizeof(PDS_FileHeader_t);
    pdsWrite(&accessDescriptor, pdsDummyCallback);
    pdsWrite(fileDataDescr, pdsStartCommitment);
  }

  return commitNeeded;
}

/******************************************************************************
\brief Resets specified area in non-volatile memory. All stored data will be lost.

\return Operation result.
******************************************************************************/
PDS_DataServerState_t pdsCorruptFiles(const uint8_t *const memoryMask)
{
  uint8_t byteToWrite = 0;
  PDS_FileCrc_t dataCrc = PDS_CRC_INITIAL_VALUE;
  PDS_DataServerState_t status;
  MEMORY_DESCRIPTOR fileDataDescr;
  PdsDataAccessContext_t accessContext;

  pdsPrepareAccessContext(&accessContext, memoryMask);

  while (pdsPrepareMemoryAccess(&accessContext, &fileDataDescr))
  {
    fileDataDescr.data = &byteToWrite;
    fileDataDescr.length = 1;
    status = pdsRead(&fileDataDescr, pdsDummyCallback);
    if (status != PDS_SUCCESS)
      return status;

    byteToWrite++;

    status = pdsWrite(&fileDataDescr, pdsDummyCallback);
    if (status != PDS_SUCCESS)
      return status;

    fileDataDescr.data = &dataCrc;
    fileDataDescr.length = sizeof(PDS_FileCrc_t);
    fileDataDescr.address = fileDataDescr.address - sizeof(PDS_FileHeader_t) + offsetof(PDS_FileHeader_t, crc);
    status = pdsWrite(&fileDataDescr, pdsDummyCallback);
    if (status != PDS_SUCCESS)
      return status;
  }
  
  return PDS_SUCCESS;
}

#endif /* _COMMISSIONING_ */

/******************************************************************************
\brief Writes data to non-volatile memory.
\
\param[in] descriptor - memory descriptor to store parameter value
\param[out]callback - callback to write-finidhed event handler.
******************************************************************************/
PDS_DataServerState_t pdsWrite(MEMORY_DESCRIPTOR *descriptor, void (*callback)(void))
{
  pdsWaitMemoryFree();
  if (STORAGE_ERROR == WRITE_MEMORY(descriptor, callback))
    return PDS_STORAGE_ERROR;

  if (callback == pdsDummyCallback)
    pdsWaitMemoryFree();

  return PDS_SUCCESS;
}

/******************************************************************************
\brief Reads data from non-volatile memory.
\
\param[in] descriptor - memory descriptor to read parameter value.
\param[out]callback - callback to read-finished event handler.
******************************************************************************/
PDS_DataServerState_t pdsRead(MEMORY_DESCRIPTOR *descriptor, void (*callback)(void))
{
  pdsWaitMemoryFree();
  if (STORAGE_ERROR == READ_MEMORY(descriptor, callback))
    return PDS_STORAGE_ERROR;

  if (callback == pdsDummyCallback)
    pdsWaitMemoryFree();

  return PDS_SUCCESS;
}
/******************************************************************************
\brief Wait until memory be ready for transaction.
******************************************************************************/
void pdsWaitMemoryFree(void)
{
  while (IS_MEMORY_BUSY())
  {
    SYS_ForceRunTask();
  }
}

/*******************************************************************************
\brief Dummy callback.
*******************************************************************************/
void pdsDummyCallback(void)
{}

// eof pdsWriteData.c
