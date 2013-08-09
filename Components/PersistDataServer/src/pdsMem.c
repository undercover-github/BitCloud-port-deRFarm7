/**************************************************************************//**
  \file pdsMem.c

  \brief Memory of Persistent Data Server

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
   History:
    2012-01-16 A. Razinkov  - Created.
   Last change:
    $Id: pdsMem.c 20614 2012-03-23 09:37:37Z arazinkov $
 ******************************************************************************/
/******************************************************************************
                               Includes section
 ******************************************************************************/
#include <pdsMem.h>

/******************************************************************************
                           Global variables section
 ******************************************************************************/
#ifdef _COMMISSIONING_
PdsMem_t pdsMem;
#endif /* _COMMISSIONING_ */
/* eof pdsMem.c */

