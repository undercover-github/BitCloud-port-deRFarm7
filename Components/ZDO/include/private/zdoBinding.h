/**************************************************************************//**
  \file zdoBinding.h

  \brief Internal interface of ZDO binding functionality.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
   History:
    2011-03-17 Max Gekk - Created.
   Last change:
    $Id: zdoBinding.h 21000 2012-04-27 09:29:54Z mgekk $
 ******************************************************************************/
#if !defined _ZDO_BINDING_H
#define _ZDO_BINDING_H

/******************************************************************************
                               Prototypes section
 ******************************************************************************/
#if defined _BINDING_
/**************************************************************************//**
  \brief Prepare Bind or Unbind ZDP request.
 ******************************************************************************/
ZDO_PRIVATE void zdoUnbindBindRequestCreate(void);

/**************************************************************************//**
  \brief Process an incoming Bind or Unbind request and send response.
 ******************************************************************************/
ZDO_PRIVATE void zdoUnbindBindInd(void);

#if defined _CERTIFICATION_
/**************************************************************************//**
  \brief Prepare ZDP Mgmt Bind request. Init APS_DataReq_t structure.
 ******************************************************************************/
ZDO_PRIVATE void zdoMgmtBindRequestCreate(void);

/**************************************************************************//**
  \brief Process an incoming Mgmt Bind request and send response.
 ******************************************************************************/
ZDO_PRIVATE void zdoMgmtBindInd(void);

#endif /* _CERTIFICATION_ */
#endif /* _BINDING_ */
#endif /* _ZDO_BINDING_H */
/** eof zdoBinding.h */
