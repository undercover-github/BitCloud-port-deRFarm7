/******************************************************************************
  \file csPersistentMem.c

  \brief
    Configuration Server persistent memory to store in non-volatile memory.

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2010 , Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
    28.10.10 A. Razinkov - Created.
******************************************************************************/

/******************************************************************************
                    Includes section
******************************************************************************/
#include <pdsDataServer.h>
#include <macenvMem.h>
#include <csSIB.h>
#include <csBuffers.h>
#ifndef _MAC2_
#include <apsConfigServer.h>
#include <nwkConfigServer.h>
#include <zdoZib.h>
#endif

#ifdef _COMMISSIONING_
/******************************************************************************
                    External variables section
******************************************************************************/
#if !defined(_USE_KF_MAC_)
extern PIB_t csPIB;
#endif // !defined(_USE_KF_MAC_)
extern SIB_t csSIB;
extern NIB_t csNIB;
extern ZIB_t csZIB;

extern CS_StackBuffers_t stackBuffers;
#ifdef _LINK_SECURITY_
extern CS_ReadOnlyItems_t PROGMEM_DECLARE(csReadOnlyItems);
#endif

#if defined(_USE_KF_MAC_)
extern uint64_t tal_pib_IeeeAddress;
#endif // defined(_USE_KF_MAC_)

/******************************************************************************
                    Files definitions
******************************************************************************/
/*******************************************
         BitCloud General parameters
********************************************/
/* File descriptors definitions for BitCloud General parameters.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csGeneralFileDescriptors[]) =
{
#ifdef _USE_KF_MAC_
  PDS_FILE_DESCR(CS_UID_MEM_ID, sizeof(tal_pib_IeeeAddress), &tal_pib_IeeeAddress),
#else
  PDS_FILE_DESCR(CS_UID_MEM_ID, sizeof(csPIB.macAttr.extAddr), &csPIB.macAttr.extAddr),
#endif /* _USE_KF_MAC_ */
  PDS_FILE_DESCR(CS_RF_TX_POWER_MEM_ID, sizeof(csSIB.csRfTxPower), &csSIB.csRfTxPower),
#ifndef _MAC2_
  PDS_FILE_DESCR(CS_EXT_PANID_MEM_ID, sizeof(csSIB.csExtPANID), &csSIB.csExtPANID),
  PDS_FILE_DESCR(CS_CHANNEL_MASK_MEM_ID, sizeof(csZIB.channelMask), &csZIB.channelMask),
  PDS_FILE_DESCR(CS_CHANNEL_PAGE_MEM_ID, sizeof(csNIB.channelPage), &csNIB.channelPage),
  PDS_FILE_DESCR(CS_DEVICE_TYPE_MEM_ID, sizeof(csNIB.deviceType), &csNIB.deviceType),
  PDS_FILE_DESCR(CS_RX_ON_WHEN_IDLE_MEM_ID, sizeof(csSIB.csRxOnWhenIdle), &csSIB.csRxOnWhenIdle),
  PDS_FILE_DESCR(CS_COMPLEX_DESCRIPTOR_AVAILABLE_MEM_ID, sizeof(csSIB.csComplexDescriptorAvailable), &csSIB.csComplexDescriptorAvailable),
  PDS_FILE_DESCR(CS_USER_DESCRIPTOR_AVAILABLE_MEM_ID, sizeof(csSIB.csUserDescriptorAvailable), &csSIB.csUserDescriptorAvailable),
  PDS_FILE_DESCR(CS_ZDP_USER_DESCRIPTOR_MEM_ID, sizeof(csSIB.csUserDescriptor), &csSIB.csUserDescriptor),
  PDS_FILE_DESCR(CS_NWK_PANID_MEM_ID, sizeof(csSIB.csNwkPanid), &csSIB.csNwkPanid),
  PDS_FILE_DESCR(CS_NWK_PREDEFINED_PANID_MEM_ID, sizeof(csSIB.csNwkPredefinedPanid), &csSIB.csNwkPredefinedPanid),
  PDS_FILE_DESCR(CS_NWK_ADDR_MEM_ID, sizeof(csNIB.networkAddress), &csNIB.networkAddress),
  PDS_FILE_DESCR(CS_NWK_UNIQUE_ADDR_MEM_ID, sizeof(csNIB.uniqueAddr), &csNIB.uniqueAddr),
  PDS_FILE_DESCR(CS_NWK_LEAVE_REQ_ALLOWED_MEM_ID, sizeof(csNIB.leaveReqAllowed), &csNIB.leaveReqAllowed),
  PDS_FILE_DESCR(CS_DTR_WAKEUP_MEM_ID, sizeof(csSIB.csDtrWakeup), &csSIB.csDtrWakeup),
#ifdef _SECURITY_
  PDS_FILE_DESCR(CS_APS_TRUST_CENTER_ADDRESS_MEM_ID, sizeof(csAIB.trustCenterAddress), &csAIB.trustCenterAddress),
  PDS_FILE_DESCR(CS_SHORT_TRUST_CENTER_ADDRESS_MEM_ID, sizeof(csAIB.tcNwkAddr), &csAIB.tcNwkAddr),
  PDS_FILE_DESCR(CS_ZDO_SECURITY_STATUS_MEM_ID, sizeof(csSIB.csZdoSecurityStatus), &csSIB.csZdoSecurityStatus),
#endif /* _SECURITY_ */
#endif /* _MAC2_ */
};

#ifndef _MAC2_
/*******************************************
         BitCloud Extended parameters
********************************************/
/* File descriptors definitions for BitCloud Extneded parameters.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csExtendedFileDescriptors[]) =
{
  PDS_FILE_DESCR(CS_NWK_PARENT_ADDR_MEM_ID, sizeof(csNIB.parentNetworkAddress), &csNIB.parentNetworkAddress),
  PDS_FILE_DESCR(CS_NWK_DEPTH_MEM_ID, sizeof(csNIB.depth), &csNIB.depth),
  PDS_FILE_DESCR(CS_NWK_EXT_PANID_MEM_ID, sizeof(csNIB.extendedPanId), &csNIB.extendedPanId),
  PDS_FILE_DESCR(CS_NWK_LOGICAL_CHANNEL_MEM_ID, sizeof(csSIB.csNwkLogicalChannel), &csSIB.csNwkLogicalChannel),
};

/*******************************************
         BitCloud Neighbor table
********************************************/
/* File descriptor definition for Neighbor table.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csNeighborTableFileDescriptor) =
  PDS_FILE_DESCR(CS_NEIB_TABLE_MEM_ID, (CS_NEIB_TABLE_SIZE * sizeof(Neib_t)), &stackBuffers.csNeibTable);

/*******************************************
         BitCloud Group table
********************************************/
#ifdef _GROUP_TABLE_
/* File descriptor definition for Group table.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csGroupTableFileDescriptor) =
  PDS_FILE_DESCR(CS_GROUP_TABLE_MEM_ID, (CS_GROUP_TABLE_SIZE * sizeof(NWK_GroupTableEntry_t)), &stackBuffers.csGroupTable);
#endif /* _GROUP_TABLE_ */

/*******************************************
         BitCloud Binding table
********************************************/
#if defined(_BINDING_) && (CS_APS_BINDING_TABLE_SIZE > 0)
/* File descriptor definition for Binding table.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csBindingTableFileDescriptor) =
  PDS_FILE_DESCR(CS_APS_BINDING_TABLE_MEM_ID, (CS_APS_BINDING_TABLE_SIZE * sizeof(ApsBindingEntry_t)), &stackBuffers.csApsBindingTable);
#endif /* _BINDING_ && CS_APS_BINDING_TABLE_SIZE > 0 */

#ifdef _SECURITY_
/*******************************************
         BitCloud NWK security tables
********************************************/
/* File descriptors definitions for BitCloud NWK security parameters.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csNwkSecurityFileDescriptors[]) =
{
  PDS_FILE_DESCR(CS_NWK_SECURITY_IB_MEM_ID, sizeof(csNIB.securityIB), &csNIB.securityIB),
  PDS_FILE_DESCR(CS_NWK_SECURITY_KEYS_MEM_ID, CS_NWK_SECURITY_KEYS_AMOUNT * sizeof(NWK_SecurityKey_t), &stackBuffers.csNwkSecKeys),
};
/*******************************************
         BitCloud APS key pair descroptors table
********************************************/
#if defined(_LINK_SECURITY_) && CS_APS_KEY_PAIR_DESCRIPTORS_AMOUNT > 0
/* File descriptor definition for APS security tables.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csApsSecurityTablesFileDescriptor) =
  PDS_FILE_DESCR(CS_APS_KEY_PAIR_DESCRIPTORS_MEM_ID, \
                  (CS_APS_KEY_PAIR_DESCRIPTORS_AMOUNT * sizeof(ApsKeyPairDescriptor_t)), \
                  &stackBuffers.csApsKeyPairDescriptors);
#endif /* _LINK_SECURITY_ && CS_APS_KEY_PAIR_DESCRIPTORS_AMOUNT > 0 */

/*******************************************
         BitCloud TC permission table
********************************************/
#if defined(_TC_PERMISSION_TABLE_) && CS_MAX_TC_ALLOWED_DEVICES_AMOUNT > 0
/* File descriptor definition for TC permission table.
   Shall be placed in the PDS_FF code segment. */
PDS_DECLARE_FILE(const PDS_FileDescr_t csPermissionTableFileDescriptor) =
  PDS_FILE_DESCR(CS_TC_PERMISSION_TABLE_MEM_ID, \
                  (CS_MAX_TC_ALLOWED_DEVICES_AMOUNT * sizeof(ExtAddr_t)), \
                  &stackBuffers.csTcDevicePermissionTable);
#endif /* _TC_PERMISSION_TABLE_ && CS_MAX_TC_ALLOWED_DEVICES_AMOUNT > 0 */

#endif /* _SECURITY_ */
#endif /* _MAC2_ */

/****************************************************************
         Directories definitions
*****************************************************************/
/* BitCloud General parameters static iNodes table.
   Will be placed in flash. */
PROGMEM_DECLARE(const PDS_MemId_t CsGeneralMemoryIdsTable[]) =
{
  CS_UID_MEM_ID,
  CS_RF_TX_POWER_MEM_ID,
#ifndef _MAC2_
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
#if defined(_SECURITY_)
  CS_APS_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_SHORT_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_ZDO_SECURITY_STATUS_MEM_ID,
#endif /* _SECURITY_ */
#endif /* _MAC2_ */
};

/* BitCloud General parameters directory descriptor.
   Shall be placed in the PDS_FD code segment. */
PDS_DECLARE_DIR(const PDS_DirDescr_t csGeneralParamsDirDescr) =
{
  .list       = CsGeneralMemoryIdsTable,
  .filesCount = ARRAY_SIZE(CsGeneralMemoryIdsTable),
  .memoryId     = BC_GENERAL_PARAMS_MEM_ID
};
//---------------------------------------------------------------
/* BitCloud Extneded parameters static iNodes table.
   Will be placed in flash. */
PROGMEM_DECLARE(const PDS_MemId_t CsExtendedMemoryIdsTable[]) =
{
  CS_NWK_PARENT_ADDR_MEM_ID,
  CS_NWK_DEPTH_MEM_ID,
  CS_NWK_EXT_PANID_MEM_ID,
  CS_NWK_LOGICAL_CHANNEL_MEM_ID,
};

/* BitCloud Extneded parameters directory descriptor.
   Shall be placed in the PDS_FD code segment. */
PDS_DECLARE_DIR(const PDS_DirDescr_t csExtendedParamsDirDescr) =
{
  .list       = CsExtendedMemoryIdsTable,
  .filesCount = ARRAY_SIZE(CsExtendedMemoryIdsTable),
  .memoryId     = BC_EXTENDED_PARAMS_MEM_ID
};
//---------------------------------------------------------------
/* BitCloud NWK security parameters static iNodes table.
   Will be placed in flash. */
PROGMEM_DECLARE(const PDS_MemId_t CsNwkSecurityMemoryIdsTable[]) =
{
  CS_NWK_SECURITY_IB_MEM_ID,
  CS_NWK_SECURITY_KEYS_MEM_ID,
};

/* BitCloud NWK security tables directory descriptor.
   Shall be placed in the PDS_FD code segment. */
PDS_DECLARE_DIR(const PDS_DirDescr_t csNwkSecurityTablesDirDescr) =
{
  .list       = CsNwkSecurityMemoryIdsTable,
  .filesCount = ARRAY_SIZE(CsNwkSecurityMemoryIdsTable),
  .memoryId     = BC_NWK_SECURITY_TABLES_MEM_ID
};
//---------------------------------------------------------------
/* BitCloud system parameters static iNodes table.
   Will be placed in flash. */
PROGMEM_DECLARE(const PDS_MemId_t BCSystemMemoryIdsTable[]) =
{
  CS_UID_MEM_ID,
  CS_RF_TX_POWER_MEM_ID,
#ifndef _MAC2_
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
#if defined(_SECURITY_)
  CS_APS_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_SHORT_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_ZDO_SECURITY_STATUS_MEM_ID,
#endif /* _SECURITY_ */
  CS_NWK_PARENT_ADDR_MEM_ID,
  CS_NWK_DEPTH_MEM_ID,
  CS_NWK_EXT_PANID_MEM_ID,
  CS_NWK_LOGICAL_CHANNEL_MEM_ID,
  CS_NEIB_TABLE_MEM_ID,
  CS_GROUP_TABLE_MEM_ID,
  CS_APS_BINDING_TABLE_MEM_ID,
#ifdef _SECURITY_
  CS_NWK_SECURITY_IB_MEM_ID,
  CS_NWK_SECURITY_KEYS_MEM_ID,
#if defined(_LINK_SECURITY_) && CS_APS_KEY_PAIR_DESCRIPTORS_AMOUNT > 0
  CS_APS_KEY_PAIR_DESCRIPTORS_MEM_ID,
#endif /* _LINK_SECURITY_ && CS_APS_KEY_PAIR_DESCRIPTORS_AMOUNT > 0 */
#if defined(_TC_PERMISSION_TABLE_) && CS_MAX_TC_ALLOWED_DEVICES_AMOUNT > 0
  CS_TC_PERMISSION_TABLE_MEM_ID,
#endif /* _TC_PERMISSION_TABLE_ && CS_MAX_TC_ALLOWED_DEVICES_AMOUNT > 0 */
#endif /* _SECURITY_ */
#endif /* _MAC2_ */
};

/* BitCloud system parameters directory descriptor.
   Shall be placed in the PDS_FD code segment. */
PDS_DECLARE_DIR(const PDS_DirDescr_t BCSystemDirDescr) =
{
  .list       = BCSystemMemoryIdsTable,
  .filesCount = ARRAY_SIZE(BCSystemMemoryIdsTable),
  .memoryId     = BC_ALL_MEMORY_MEM_ID
};

//---------------------------------------------------------------
/* BitCloud extended general parameters static iNodes table.
   Will be placed in flash. */
PROGMEM_DECLARE(const PDS_MemId_t BCExtGenParamsMemoryIdsTable[]) =
{
  CS_UID_MEM_ID,
  CS_RF_TX_POWER_MEM_ID,
#ifndef _MAC2_
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
#if defined(_SECURITY_)
  CS_APS_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_SHORT_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_ZDO_SECURITY_STATUS_MEM_ID,
#endif /* _SECURITY_ */
  CS_NWK_PARENT_ADDR_MEM_ID,
  CS_NWK_DEPTH_MEM_ID,
  CS_NWK_EXT_PANID_MEM_ID,
  CS_NWK_LOGICAL_CHANNEL_MEM_ID,
#endif /* _MAC2_ */
};

/* BitCloud system parameters directory descriptor.
   Shall be placed in the PDS_FD code segment. */
PDS_DECLARE_DIR(const PDS_DirDescr_t BCExtGenParamsDirDescr) =
{
  .list       = BCExtGenParamsMemoryIdsTable,
  .filesCount = ARRAY_SIZE(BCExtGenParamsMemoryIdsTable),
  .memoryId     = BC_EXT_GEN_MEMORY_MEM_ID
};

//---------------------------------------------------------------
/* A set of parameters to be stored to provide a network
   rejoin after power failure.
   Will be placed in flash. */
PROGMEM_DECLARE(const PDS_MemId_t BCRejoinParamsMemoryIdsTable[]) =
{
  CS_UID_MEM_ID,
  CS_RF_TX_POWER_MEM_ID,
#ifndef _MAC2_
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
#if defined(_SECURITY_)
  CS_APS_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_SHORT_TRUST_CENTER_ADDRESS_MEM_ID,
  CS_ZDO_SECURITY_STATUS_MEM_ID,
#endif /* _SECURITY_ */
  CS_GROUP_TABLE_MEM_ID,
#ifdef _SECURITY_
  CS_NWK_SECURITY_IB_MEM_ID,
  CS_NWK_SECURITY_KEYS_MEM_ID,
#if defined(_TC_PERMISSION_TABLE_) && CS_MAX_TC_ALLOWED_DEVICES_AMOUNT > 0
  CS_TC_PERMISSION_TABLE_MEM_ID,
#endif /* _TC_PERMISSION_TABLE_ && CS_MAX_TC_ALLOWED_DEVICES_AMOUNT > 0 */
#endif /* _SECURITY_ */
#endif /* _MAC2_ */
};

/* BitCloud rejoin parameters directory descriptor.
   Shall be placed in the PDS_FD code segment. */
PDS_DECLARE_DIR(const PDS_DirDescr_t BCRejoinParamsDirDescr) =
{
  .list       = BCRejoinParamsMemoryIdsTable,
  .filesCount = ARRAY_SIZE(BCRejoinParamsMemoryIdsTable),
  .memoryId     = BC_NETWORK_REJOIN_PARAMS_MEM_ID
};


#endif /* _COMMISSIONING_ */

/* eof csPersistentMem.c */
