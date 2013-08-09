/**************************************************************************//**
  \file pdsMemIds.h

  \brief PDS file and directory memory identificators definitions.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
   History:
    2012-04-25 D. Kolmakov - Created.
   Last change:
    $Id: pdsMem.h 20820 2012-04-10 09:43:31Z arazinkov $
 ******************************************************************************/
#ifndef _PDS_MEM_IDS_H
#define _PDS_MEM_IDS_H

/******************************************************************************
                   Defines section
******************************************************************************/

/******************************************************************************
                               Types section
 ******************************************************************************/
typedef enum _PDS_MemIdsTable_t
{
  /** BitCloud file ids */
  CS_UID_MEM_ID,
  CS_RF_TX_POWER_MEM_ID,
  CS_EXT_PANID_MEM_ID,
  CS_CHANNEL_MASK_MEM_ID,
  CS_CHANNEL_PAGE_MEM_ID,
  CS_DEVICE_TYPE_MEM_ID,
  CS_RX_ON_WHEN_IDLE_MEM_ID,
  CS_COMPLEX_DESCRIPTOR_AVAILABLE_MEM_ID,
  CS_USER_DESCRIPTOR_AVAILABLE_MEM_ID,
  CS_ZDP_USER_DESCRIPTOR_MEM_ID,
  CS_NWK_PANID_MEM_ID,
  CS_NWK_PREDEFINED_PANID_MEM_ID,
  CS_NWK_ADDR_MEM_ID,
  CS_NWK_UNIQUE_ADDR_MEM_ID,
  CS_NWK_LEAVE_REQ_ALLOWED_MEM_ID,
  CS_DTR_WAKEUP_MEM_ID,
  CS_APS_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_SHORT_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_ZDO_SECURITY_STATUS_MEM_ID,
  CS_NWK_PARENT_ADDR_MEM_ID,
  CS_NWK_DEPTH_MEM_ID,
  CS_NWK_EXT_PANID_MEM_ID,
  CS_NWK_LOGICAL_CHANNEL_MEM_ID,
  CS_NEIB_TABLE_MEM_ID,
  CS_GROUP_TABLE_MEM_ID,
  CS_APS_BINDING_TABLE_MEM_ID,
  CS_NWK_SECURITY_IB_MEM_ID,
  CS_NWK_SECURITY_KEYS_MEM_ID,
  CS_APS_KEY_PAIR_DESCRIPTORS_MEM_ID,
  CS_TC_PERMISSION_TABLE_MEM_ID,

  /** BitCloud directory ids */
  BC_GENERAL_PARAMS_MEM_ID,
  BC_EXTENDED_PARAMS_MEM_ID,
  BC_NWK_SECURITY_TABLES_MEM_ID,
  BC_ALL_MEMORY_MEM_ID,
  BC_EXT_GEN_MEMORY_MEM_ID,
  BC_NETWORK_REJOIN_PARAMS_MEM_ID,

  /** Application ids */
  APP_ZLL_COLOR_SCENE_REMOTE_DATA_MEM_ID,
  APP_ZLL_COLOR_LIGHT_DATA_MEM_ID,
  APP_ZLL_SCENE_CLUSTER_SERVER_ATTR_MEM_ID,
  APP_ZLL_COLOR_CONTROL_CLUSTER_SERVER_ATTR_MEM_ID,
  APP_ZLL_LEVEL_CONTROL_CLUSTER_SERVER_ATTR_MEM_ID,
  APP_ZLL_ONOFF_CLUSTER_SERVER_ATTR_MEM_ID,
  APP_ZLL_MEMORY_MEM_ID,
  APP_ZLL_IB_MEMORY_MEM_ID,
  APP_ZLL_MEMORY_ALL_MEM_ID,

  /** Service values */
  PDS_MEM_IDS_COUNT,
  PDS_ANY_MEMORY_FROM,
  PDS_NO_MEMORY_SPECIFIED = 0xFFFF,
} PDS_MemIdsTable_t;

#endif /* _PDS_MEM_IDS_H */
/** eof pdsMemIds.h */

