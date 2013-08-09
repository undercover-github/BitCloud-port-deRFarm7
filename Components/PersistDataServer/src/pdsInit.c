/**************************************************************************//**
  \file pdsInit.c

  \brief Persistent Data Server initialization module

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
   History:
    2012-01-16 A. Razinkov  - Created.
   Last change:
    $Id: pdsInit.c 21048 2012-05-03 13:20:33Z dkolmakov $
 ******************************************************************************/
/******************************************************************************
                   Includes section
 ******************************************************************************/
#include <pdsDataServer.h>
#include <pdsMem.h>
#include <pdsCrcService.h>
#include <pdsWriteData.h>
#include <pdsDbg.h>
#include <leds.h>
#include <pdsTimer.h>

#ifdef _COMMISSIONING_
/******************************************************************************
                   Definitions section
 ******************************************************************************/
#ifdef  __IAR_SYSTEMS_ICC__
#pragma segment="PDS_FF"
  #define PDS_FF_START (PDS_FileDescrRec_t) __sfb("PDS_FF")
  #define PDS_FF_END   (PDS_FileDescrRec_t) __sfe("PDS_FF")
  #define PDS_FF_SIZE                       __sfs("PDS_FF")

#pragma segment="PDS_FD"
  #define PDS_FD_START (PDS_DirDescrRec_t) __sfb("PDS_FD")
  #define PDS_FD_END   (PDS_DirDescrRec_t) __sfe("PDS_FD")
  #define PDS_FD_SIZE                      __sfs("PDS_FD")
#elif __GNUC__
  #define PDS_FF_START (PDS_FileDescrRec_t) &__pds_ff_start
  #define PDS_FF_END   (PDS_FileDescrRec_t) &__pds_ff_end
  #define PDS_FF_SIZE                       (&__pds_ff_end - &__pds_ff_start)

  #define PDS_FD_START (PDS_DirDescrRec_t) &__pds_fd_start
  #define PDS_FD_END   (PDS_DirDescrRec_t) &__pds_fd_end
  #define PDS_FD_SIZE                      (&__pds_fd_end - &__pds_fd_start)
#else
  #error "Unsupported compiler"
#endif

/******************************************************************************
                   External variables section
 ******************************************************************************/
extern PdsMem_t pdsMem;

#ifdef __GNUC__
extern uint8_t __pds_ff_start;
extern uint8_t __pds_ff_end;
extern uint8_t __pds_fd_start;
extern uint8_t __pds_fd_end;
#endif /* __GNUC__ */

/****************************************************************************
                   Static functions prototypes section
******************************************************************************/
static void pdsInitFileOffsetTable(void);

/******************************************************************************
                   Implementations section
******************************************************************************/
/******************************************************************************
\brief Persistent Data Server initialization routine.
*******************************************************************************/
void PDS_Init(void)
{
  pdsMemory()->ffStart = PDS_FF_START;
  pdsMemory()->fdStart = PDS_FD_START;
  assert(0 == (PDS_FF_SIZE % sizeof(PDS_FileDescr_t)), PDSINIT_PDSINIT0);
  pdsMemory()->ffSize |= (PDS_FF_SIZE / sizeof(PDS_FileDescr_t));
  assert(0 == (PDS_FD_SIZE % sizeof(PDS_DirDescr_t)), PDSINIT_PDSINIT1);
  pdsMemory()->fdSize |= (PDS_FD_SIZE / sizeof(PDS_DirDescr_t));

  {
    PDS_FileDescrRec_t fDescrRec = pdsMemory()->ffStart;
    PDS_FileDescr_t fDescr;

    pdsMemory()->totalMemorySize = 0;
    PDS_MEM_MASK_CLEAR_ALL(pdsMemory()->allowedForStoring);
  
    for (uint8_t i = 0; i < pdsMemory()->ffSize; i++)
    {
      memcpy_P(&fDescr, fDescrRec, sizeof(PDS_FileDescr_t));
      pdsMemory()->totalMemorySize += fDescr.size + sizeof(PDS_FileHeader_t);
      PDS_MEM_MASK_SET_BIT(pdsMemory()->allowedForStoring, fDescr.memoryId);
      fDescrRec++;
    }
  }

  pdsInitFileOffsetTable();
  pdsTimerInit();

  PDS_MemMask_t memoryMask;
  pdsCheckStorage(memoryMask);

#ifdef PDS_HIGHLIGHT_WRITING_PROCESS
  BSP_OpenLeds();
#endif
}

/******************************************************************************
\brief Filling file offset table in non-volatile memory.
*******************************************************************************/
static void pdsInitFileOffsetTable(void)
{
  MEMORY_DESCRIPTOR descriptor;
  PDS_FileOffsetInfo_t fileOffset;
  PDS_FileDescrRec_t fileDescrRec = NULL;
  PDS_FileDescr_t fileDescr;
  PDS_MemId_t memoryId;
  uint32_t currentFileOffset = SYSTEM_BASE_STORAGE_ADDRESS + pdsMemory()->ffSize * sizeof(PDS_FileOffsetInfo_t);
  uint16_t offsetTableIndex = 0;

  descriptor.data = (uint8_t *)&fileOffset;
  descriptor.length = sizeof(fileOffset);

  for (memoryId = 0; memoryId < PDS_MEM_IDS_COUNT; memoryId++)
    if (PDS_MEM_MASK_IS_BIT_SET(pdsMemory()->allowedForStoring, memoryId))
    {
      descriptor.address = SYSTEM_BASE_STORAGE_ADDRESS + offsetTableIndex * sizeof(PDS_FileOffsetInfo_t);
      pdsRead(&descriptor, pdsDummyCallback);

      if (fileOffset.memoryId != memoryId ||
          fileOffset.offset != currentFileOffset)
      {
        fileOffset.memoryId = memoryId;
        fileOffset.offset = currentFileOffset;
        pdsWrite(&descriptor, pdsDummyCallback);
      }

      fileDescrRec = pdsGetFileDescrRec(memoryId);
      assert(fileDescrRec, PDSINIT_PDSINIT2);
      memcpy_P(&fileDescr, fileDescrRec, sizeof(PDS_FileDescr_t));
      currentFileOffset += fileDescr.size + sizeof(PDS_FileHeader_t);
      offsetTableIndex++;
    }
}

#endif /* _COMMISSIONING_ */
/* eof pdsInit.c */

