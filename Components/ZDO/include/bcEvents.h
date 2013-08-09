/**************************************************************************//**
  \file bcEvents.h

  \brief List of events for Bitcloud(tm) events facility

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
  History:
    28.02.12 D. Loskutnikov - Created
******************************************************************************/
#ifndef _BC_EVENTS_H
#define _BC_EVENTS_H

/******************************************************************************
                               Includes section
******************************************************************************/
#include <sysEvents.h>

/******************************************************************************
                                Types section
******************************************************************************/
typedef enum
{
  BC_EVENT_NETWORK_LOST,
  BC_EVENT_TC_OFFLINE,
  BC_EVENT_CHILD_JOINED,
  BC_EVENT_CHILD_REMOVED,
  BC_EVENT_NETWORK_LEFT,
  BC_EVENT_NETWORK_STARTED,
  BC_EVENT_NETWORK_UPDATE,
  BC_EVENT_CHILD_KEY_ABSENT,
  BC_EVENT_ADDRESS_CONFLICT,
  BC_EVENT_KEY_PAIR_DELETED,
  BC_EVENT_NEW_CONCENTRATOR,
  BC_EVENT_DEVICE_ANNCE,
  BC_EVENT_UPDATE_LINK_KEY,
  BC_EVENT_DEVICE_LEFT,
  BC_EVENT_NETWORK_ENTERED,
  BC_EVENT_STORING_FINISHED,
  BC_EVENT_BIND_TABLE_UPDATED,
  BC_EVENT_GROUP_TABLE_UPDATED,
  BC_EVENT_GROUPS_REMOVED,
  BC_EVENT_PERMISSION_TABLE_UPDATED,
  BC_EVENT_NWK_SECURITY_TABLES_UPDATED,
  BC_EVENT_APS_SECURITY_TABLES_UPDATED,
} BcEvents_t;

#endif // _BC_EVENTS_H
// eof bcEvents.h
