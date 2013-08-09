/***************************************************************************//**
  \file pdsDataServer.h

  \brief The header file describes the Persistence Data Server interface

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    28/09/07 A. Khromykh - Created
    01/11/10 A. Razinkov - Modified
  Last change:
    $Id: pdsDataServer.h 21048 2012-05-03 13:20:33Z dkolmakov $
*****************************************************************************/

#ifndef _PERSISTDATASERVER_H
#define _PERSISTDATASERVER_H

/******************************************************************************
                    Description section
******************************************************************************/
/*
   PERSISTENT DATA SERVER (PDS)
   User can specifiy parameters he(she) wants to backup in non-volatile memory and
   restore in case of power failure. This service is provided by Persistent Data
   Server (PDS) module. The BitCloud (TM) stack also uses the same service for
   its internal structures.
   User is able to control which parameters should be restored via
   PDS_Restore() function.

   PERSISTENT DATA SERVER FILE
   Let's call 'Persistent Data' or PD the parameters which user wants to backup
   in non-volatile memory and restore in case of power failure. Persistent data
   file it's PD with auxillary service fields.

   PERSISTENT DATA SERVER FILE DESCRIPTOR
   Each persistent data file should be described with appropriate descriptor of
   PDS_FileDescr_t type. This descriptor keeps information about the parameter's 
   displacment in RAM and in non-volatile memory. The PDS_DECLARE_FILE() macros
   is used to declare PDS FD and the PDS_FILE_DESCR() macros is used to initialize
   it.

   PERSISTENT DATA SERVER DIRECTORY DESCRIPTOR
   PDS is able to operate with separate files or with file lists called 
   PDS directory. It's used to perform similar operation on a number of files.
   The PDS_DECLARE_DIR() macros is used to declare PDS directory.

   DIRECT ACCESS TO NVM
   User can access a non-volatle storage directly via PDS_WriteUserData() and
   PDS_ReadUserData API.
*/

/******************************************************************************
                   Includes section
******************************************************************************/
#include <mnUtils.h>
#include <pdsMemIds.h>
/******************************************************************************
                   Defines section
******************************************************************************/
/* Highlight non-volatile memory writing with onboard LED */
//#define PDS_HIGHLIGHT_WRITING_PROCESS

/* Persistent Data Base segment accessory keyword */
#ifdef __IAR_SYSTEMS_ICC__
  #define PDS_DECLARE_FILE(object) \
         _Pragma("location=\"PDS_FF\"") __root PROGMEM_DECLARE (object)
  #define PDS_DECLARE_DIR(object) \
         _Pragma("location=\"PDS_FD\"") __root PROGMEM_DECLARE (object)
#elif __GNUC__
  #define PDS_DECLARE_FILE(object) \
         __attribute__((__section__(".pds_ff"))) object
  #define PDS_DECLARE_DIR(object) \
         __attribute__((__section__(".pds_fd"))) object
#else
  #error "Unsupported compiler"
#endif

/* Macros to initialize file descriptor */
#define PDS_FILE_DESCR(id, dataSize, addr) {.memoryId = id, .size = dataSize, .RAMAddr = addr}

/* PDS working status */
#define PDS_STOPPED_STATUS                 (1U << 0U)
#define PDS_COMMITMENT_INPROGRESS_STATUS   (1U << 1U)
#define PDS_COMMITMENT_OUT_OFF_DATE_STATUS (1U << 2U)

/******************************************************************************
                   Types section
******************************************************************************/
/* PDS state result after a command call */
typedef enum
{
  PDS_SUCCESS,            // Command completed successfully
  PDS_STORAGE_ERROR,      // NVM error occurred
  PDS_CRC_ERROR,          // Wrong CRC
  PDS_OUT_OF_DATE_ERROR,  // Data in RAM does not equal data in non-volatile memory
  PDS_COMMIT_IN_PROGRESS, // Commit to external memory started
} PDS_DataServerState_t;

/* Type of PDS file or directory unique identifier. */
typedef uint16_t PDS_MemId_t;

/* Memory identifier record pointer */
typedef const PDS_MemId_t FLASH_PTR *PDS_MemIdRec_t;

/* Type of PDS file data size value. */
typedef uint16_t PDS_FileSize_t;

/* Type of PDS file data CRC. */
typedef uint8_t PDS_FileCrc_t;

/* PDS file descriptor structure. */
typedef struct _PDS_FileDescr_t
{
  /* Pointer to file data located in RAM */
  void *RAMAddr;
  /* File data size */
  PDS_FileSize_t size;
  /* File unique identifier */
  PDS_MemId_t memoryId;
} PDS_FileDescr_t;

/* File descriptor record pointer */
typedef const PDS_FileDescr_t FLASH_PTR *PDS_FileDescrRec_t;

/* PDS directory descriptor structure. */
typedef struct _PDS_DirDescr_t
{
  /* Pointer to list of file identifiers */
  PDS_MemIdRec_t list;
  /* Nuber of files in derectory */
  uint16_t    filesCount;
  /* Directory unique identifier */
  PDS_MemId_t memoryId;
} PDS_DirDescr_t;

/* Directory descriptor record pointer */
typedef const PDS_DirDescr_t FLASH_PTR *PDS_DirDescrRec_t;

/* PDS file header structure. */
typedef struct _PDS_FileHeader_t
{
  /* File data CRC */
  PDS_FileCrc_t crc;
  /* File data size */
  PDS_FileSize_t size;
  /* File unique identifier */
  PDS_MemId_t memoryId;
} PDS_FileHeader_t;

/* PDS file offset information. */
typedef struct _PDS_FileOffsetInfo_t
{
  /* File offset in non-volatile memory */
  uint32_t offset;
  /* File unique identifier */
  PDS_MemId_t memoryId;
} PDS_FileOffsetInfo_t;

#ifdef _COMMISSIONING_
//! \cond internal
/***************************************************************************//**
\brief Persistent Data Server initialization routine.
*******************************************************************************/
void PDS_Init(void);
//! \endcond

/***************************************************************************//**
\brief Restores data from non-volatile memory (EEPROM)

\ingroup pds

PDS files not included in the current build configuration will be ignored. 
Restoring process will be performed only for PDS files present in EEPROM.

\param[in] memoryId - an identifier of PDS file or directory to be restored 
                      from non-volatile memory.

\return true, if at leeast one file has been restored, false - otherwise.
*******************************************************************************/
bool PDS_Restore(PDS_MemId_t memoryId);

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
bool PDS_Store(PDS_MemId_t memoryId);

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
bool PDS_BlockingStore(PDS_MemId_t memoryId);

/***************************************************************************//**
\brief Provides PDS with information about memory that should be kept up-to-date
       in the non-volatile memory.

\ingroup pds

Data will be saved im non-volatile memory upon specific events.

\param[in] memoryId - an identifier of PDS file or directory to be stored 
                      in non-volatile memory.
******************************************************************************/
void PDS_StoreByEvents(PDS_MemId_t memoryId);

/***************************************************************************//**
\brief Enables periodic commitment of specified persistent data tables (PDTs) 
       to non-volatile memory

\ingroup pds

\param[in] interval - period of time between successive commitments
\param[in] memoryId - an identifier of PDS file or directory to be stored 
                      in non-volatile memory.
******************************************************************************/
void PDS_StoreByTimer(uint32_t interval, PDS_MemId_t memoryId);

/*****************************************************************************//**
\brief Resets the specified areas in non-volatile memory. All data stored in
       these areas will be lost.

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be cleared 
                      in non-volatile memory.

\return - PDS state as an operation result.
*******************************************************************************/
PDS_DataServerState_t PDS_ResetStorage(PDS_MemId_t memoryId);

/***************************************************************************//**
\brief Continue Persistent Data Server operation the same way as before stopping

\ingroup pds
*******************************************************************************/
void PDS_Run(void);

/***************************************************************************//**
\brief Stops the Persistent Data Server

\ingroup pds

This doesn't affect a policy created through PDS_StoreByEvent() of 
PDS_StoreByTimer() functions. After calling the PDS_Run() function PDS 
continues working as before the stop.
*******************************************************************************/
void PDS_Stop(void);

/***************************************************************************//**
\brief Checks if the specified PDS file or directory can be restored
       from non-volatile memory

\ingroup pds

PDS files not included in the current build configuration will be ignored. 

\param[in] memoryId - an identifier of PDS file or directory to be checked. 

\return true, if the specified memory can be restored; false - otherwise.
******************************************************************************/
bool PDS_IsAbleToRestore(PDS_MemId_t memoryId);

/***************************************************************************//**
\brief Checks if the specified PDS file or directory can be stored
       in non-volatile memory

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be checked. 

\return true, if the specified memory can be stored; false - otherwise.
*******************************************************************************/
bool PDS_IsAbleToStore(PDS_MemId_t memoryId);

/***************************************************************************//**
\brief Checks if files specified have been restored from non-volatile memory
       during the latest restoring procedure

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be checked. 

\return true, if the specified PDT(s) have been restored; false - otherwise.
******************************************************************************/
bool PDS_IsMemoryRestored(PDS_MemId_t memoryId);

/***************************************************************************//**
\brief Marks the specified restored PDS files to be cleared during a reset phase
      (during ZDO_StartNetwork() execution)

\ingroup pds

\param[in] memoryId - an identifier of PDS file or directory to be cleared. 
******************************************************************************/
void PDS_ClearRestoredMemory(PDS_MemId_t memoryId);

/******************************************************************************
\brief This function is deprecated - use PDS_BlockingStore() instead
******************************************************************************/
void PDS_FlushData(PDS_MemId_t memoryIds);

#endif /* _COMMISSIONING_ */

/***************************************************************************//**
\brief Must be called from the ::APL_TaskHandler() function only.\n
Reads data from the user's area of EEPROM.

\ingroup pds

\param[in] offset   - data offset - 0 values points to the start of the user's section, 
                      outside the area occupied by stack's and application's PDTs
\param[in] data     - pointer to user data area
\param[in] length   - data length
\param[in] callback - pointer to a callback function; if callback is NULL,
                      then data will be read syncronously

\return - PDS state as a result of data reading operation
*******************************************************************************/
PDS_DataServerState_t PDS_ReadUserData(uint16_t offset, uint8_t *data,
                                       uint16_t length, void (*callback)(void));

/***************************************************************************//**
\brief Can be called only from ::APL_TaskHandler() function.\n
Writes data to the user's area of EEPROM.

\ingroup pds

\param[in] offset   - data offset - 0 values points to the start of the user's section, 
                      outside the area occupied by stack's and application's PDTs
\param[in] data     - pointer to user data area
\param[in] length   - data length
\param[in] callback - pointer to a callback function; if callback is NULL,
                      then data will be written syncronously

\return - PDS state as a result of data writing operation
*******************************************************************************/
PDS_DataServerState_t PDS_WriteUserData(uint16_t offset, uint8_t *data,
                                        uint16_t length, void (*callback)(void));

#endif  //#ifndef _PERSISTDATASERVER_H

