/**************************************************************************//**
\file   isdImageStorage.h

\brief  The public API of the image storage driver.

The image storage driver (ISD) uses commands equivalent to the OTAU cluster's commands
sent from the OTAU client to the OTAU server while transferring an image.
In fact, the OTAU server uses ISD only to pass OTAU client's commands to the image
storage system.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    25.05.11 A. Khromykh - Created
*******************************************************************************/

#ifndef _ISDIMAGESTORAGE_H
#define _ISDIMAGESTORAGE_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include <types.h>
#include <zclOTAUCluster.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/

/******************************************************************************
                   Types section
******************************************************************************/
/** \brief Statuses returned by the driver */
typedef enum
{
  ISD_NO_COMMUNICATION,
  ISD_COMMUNICATION_ESTABLISHED,
  ISD_IDLE,
  ISD_BUSY,
  ISD_HARDWARE_FAULT
} ISD_Status_t;

typedef void (* IsdOpenCb_t)(ISD_Status_t);
typedef void (* IsdQueryNextImageCb_t)(ZCL_OtauQueryNextImageResp_t *);
typedef void (* IsdImageBlockCb_t)(ZCL_OtauImageBlockResp_t *);
typedef void (* IsdUpgradeEndCb_t)(ZCL_OtauUpgradeEndResp_t *);

/******************************************************************************
                   Prototypes section
******************************************************************************/
/**************************************************************************//**
\brief Opens the serial interface for connection with the image storage

The serial interface captured by executing this function may not be used
until OFD_Close() is called.

\param[in] cb - pointer to a callback function called once the interface is opened
******************************************************************************/
void ISD_Open(IsdOpenCb_t cb);

/**************************************************************************//**
\brief Closes the serial interface opened using ISD_Open()
******************************************************************************/
void ISD_Close(void);

/**************************************************************************//**
\brief Sends a query next image request to the storage system, which should decide
whether the request's issuer needs a new image

The response to this command received from the storage system should be somehow
delivered to the client. If the storage system decides that the client needs a new
image the client may then start sending image block requests to retrieve individual
blocks of the new image.

Whether the client device should download a new image is indicated by the \c status
field of the callback's argument (which is of ZCL_OtauQueryNextImageResp_t type)
having the ::ZCL_SUCCESS_STATUS value.

\param[in]
  addressing - pointer to structure that contains network information
  about the client including network address, endpoint, cluster ID and profile ID
\param[in]
  data - data payload containing information about client's firmware
\param[in]
  cb - pointer to a callback function called when the response is received
  from the storage system
******************************************************************************/
void ISD_QueryNextImageReq(ZCL_Addressing_t *addressing, ZCL_OtauQueryNextImageReq_t *data, IsdQueryNextImageCb_t cb);

/**************************************************************************//**
\brief Sends an image block request to the storage system, in order to retrieve
a certain block of data of the specified image

The function requests an individual block (specified by the offset inside the image
and having the OFD_BLOCK_SIZE length) of the specific image from the storage system.
The image is specified by its image type, manufacturer ID and firmware version.

The storage system replies with the response comman containing the requested data
(if all information is correct). Response reception is indicated by the callback
function.

\param[in]
  addressing - pointer to the structure that contains network information
  about the client including network address, endpoint, cluster ID and profile ID
\param[in]
  data - data payload containing information about the requested block of data and
  the image
\param[in]
  cb - pointer to a callback function called when the response is received
  from the storage system
******************************************************************************/
void ISD_ImageBlockReq(ZCL_Addressing_t *addressing, ZCL_OtauImageBlockReq_t *data, IsdImageBlockCb_t cb);

/**************************************************************************//**
\brief Sends an upgrade end request to the storage system, notifying it that
all blocks have been received

The function notifies the storage system that the client has received the whole
image. The storage system replies with the update end response that specifies the
time after which the client should switch to the new image.

\param[in]
  addressing - pointer to the structure that contains network information
  about the client including network address, endpoint, cluster ID and profile ID
\param[in]
  data - data payload containg information about the image that has been downloaded
  to the client
\param[in]
  cb - pointer to a callback function called when the response is received
  from the storage system
******************************************************************************/
void ISD_UpgradeEndReq(ZCL_Addressing_t *addressing, ZCL_OtauUpgradeEndReq_t *data, IsdUpgradeEndCb_t cb);

#endif /* _ISDIMAGESTORAGE_H */
