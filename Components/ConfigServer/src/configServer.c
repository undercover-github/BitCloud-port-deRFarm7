/******************************************************************************
  \file configServer.c

  \brief
    Configuration Server implementation

  \author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2010 , Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

  \internal
    History:
    19.10.10 A. Razinkov - Created.
******************************************************************************/

/******************************************************************************
                    Includes section
******************************************************************************/
#include <configServer.h>
#include <macenvMem.h>
#include <csSIB.h>
#include <csDbg.h>
#include <csBuffers.h>
#ifdef _COMMISSIONING_
#include <pdsDataServer.h>
#endif /* _COMMISSIONING_ */
#ifndef _MAC2_
#include <zdoZib.h>
#else
#include <macenvPib.h>
#endif

/******************************************************************************
                    Definitions section
******************************************************************************/
#define IS_MEMORY_AREA(item) (0 == (item).size)

/******************************************************************************
                    Types section
******************************************************************************/
typedef enum _CS_MemoryLocation_t
{
  CS_MEMORY_LOCATION_FLASH,
  CS_MEMORY_LOCATION_RAM
} CS_MemoryLocation_t;


/******************************************************************************
                    External variables section
******************************************************************************/
#if !defined(_USE_KF_MAC_)
extern PIB_t csPIB;
#endif // !defined(_USE_KF_MAC_)

extern SIB_t csSIB;
#ifndef _MAC2_
extern NIB_t csNIB;
extern ZIB_t csZIB;
#endif
extern CS_MemoryItem_t PROGMEM_DECLARE(csMemoryItems[]);
extern CS_StackBuffers_t stackBuffers;

#if defined(_USE_KF_MAC_)
extern uint64_t tal_pib_IeeeAddress;
#if defined(_MAC_BAN_NODE_)
extern CS_ReadOnlyItems_t PROGMEM_DECLARE(csReadOnlyItems);
#endif
#endif

/******************************************************************************
                    Functions prototypes section
******************************************************************************/
static CS_MemoryItem_t csGetItem(CS_MemoryItemId_t itemId);
static CS_MemoryLocation_t csGetItemLocation(CS_MemoryItemId_t itemId);

/******************************************************************************
                    Implementation section
******************************************************************************/
/******************************************************************************
\brief Configuration Server initialization
******************************************************************************/
void CS_Init(void)
{
  csSetToDefault();
#ifdef _COMMISSIONING_
  PDS_Init();
#endif /* _COMMISSIONING_ */
}

/******************************************************************************
\brief Gets the parameter specified by it's identifier

\param[in] parameterId - ID of the parameter being read
\param[out] parameterValue - pointer to the memory

******************************************************************************/
void CS_ReadParameter(CS_MemoryItemId_t parameterId, void *parameterValue)
{
  if (parameterValue)
  {
    CS_MemoryItem_t item = csGetItem(parameterId);

    /* Read item value from memory it's placed. This is not valid for memory areas -
     * use CS_GetMemory() instead */
    if (CS_MEMORY_LOCATION_FLASH == csGetItemLocation(parameterId))
      memcpy_P(parameterValue, item.value.flashValue, item.size);
    else
      memcpy(parameterValue, item.value.ramValue, item.size);
  }
}

/******************************************************************************
\brief Sets the parameter specified by it's identifier

\param[in] parameterId - ID of the parameter being written
\param[out] parameterValue - pointer to the parameter

******************************************************************************/
void CS_WriteParameter(CS_MemoryItemId_t parameterId, const void *parameterValue)
{
  if (parameterValue && (CS_MEMORY_LOCATION_RAM == csGetItemLocation(parameterId)))
  {
    CS_MemoryItem_t item = csGetItem(parameterId);

    if (!IS_MEMORY_AREA(item))
      memcpy(item.value.ramValue, parameterValue, item.size);
  }
}

/******************************************************************************
\brief Gets allocated memory specified by memory identifier

\param[in] memoryId - memory area Id
\param[out] memoryPtr - pointer to the memory

******************************************************************************/
void CS_GetMemory(CS_MemoryItemId_t memoryId, void **memoryPtr)
{
  CS_MemoryItem_t item = csGetItem(memoryId);

  if (IS_MEMORY_AREA(item))
    *((void **)memoryPtr) = item.value.ramValue;
}

/******************************************************************************
\brief Set PDS default

******************************************************************************/
void CS_PdsDefaultValue(void)
{
#if defined(_USE_KF_MAC_)
  tal_pib_IeeeAddress = CCPU_TO_LE64(CS_UID);
#else
  csPIB.macAttr.extAddr = CCPU_TO_LE64(CS_UID);
#endif // defined(_USE_KF_MAC_)

  csSIB.csRfTxPower = CS_RF_TX_POWER;
#ifndef _MAC2_
  csSIB.csExtPANID = CCPU_TO_LE64(CS_EXT_PANID);
  csZIB.channelMask = CS_CHANNEL_MASK;
  csNIB.channelPage = CS_CHANNEL_PAGE;
  csNIB.deviceType = CS_DEVICE_TYPE;
  csSIB.csRxOnWhenIdle = (CS_DEVICE_TYPE == DEVICE_TYPE_END_DEVICE) ? CS_RX_ON_WHEN_IDLE : true;
  csSIB.csComplexDescriptorAvailable = CS_COMPLEX_DESCRIPTOR_AVAILABLE;
  csSIB.csUserDescriptorAvailable = CS_USER_DESCRIPTOR_AVAILABLE;
  csSIB.csNwkPanid = CCPU_TO_LE16(CS_NWK_PANID);
  csSIB.csNwkPredefinedPanid = CS_NWK_PREDEFINED_PANID,
  csNIB.networkAddress = CCPU_TO_LE16(CS_NWK_ADDR);
  csNIB.uniqueAddr = CS_NWK_UNIQUE_ADDR;
  csNIB.leaveReqAllowed = CS_NWK_LEAVE_REQ_ALLOWED,
  csSIB.csDtrWakeup = CS_DTR_WAKEUP;
#if defined(_COMMISSIONING_)
  csNIB.parentNetworkAddress = CCPU_TO_LE16(0xFFFF);
  csNIB.depth = 0;
  csNIB.extendedPanId = CCPU_TO_LE64(CS_NWK_EXT_PANID);
  csSIB.csNwkLogicalChannel = 0;
#endif /* _COMMISSIONING_ */
#if defined(_SECURITY_)
  csAIB.trustCenterAddress = CCPU_TO_LE64(CS_APS_TRUST_CENTER_ADDRESS);
  csAIB.tcNwkAddr = CCPU_TO_LE16(CS_SHORT_TRUST_CENTER_ADDRESS);
  csSIB.csZdoSecurityStatus = CS_ZDO_SECURITY_STATUS;
#endif /* _SECURITY_ */

  /* Parameters not to store in EEPROM */
  csNIB.maxNeighborRouteCost = CS_MAX_NEIGHBOR_ROUTE_COST;
  csNIB.maxLinkRouteCost = CS_MAX_LINK_ROUTE_COST;
  csSIB.csNwkMaxLinkStatusFailures = CS_NWK_MAX_LINK_STATUS_FAILURES;
  csSIB.csNwkEndDeviceMaxFailures = CS_NWK_END_DEVICE_MAX_FAILURES;
#ifdef AT86RF212
  csSIB.csLbtMode = CS_LBT_MODE;
#endif /* AT86RF212 */
#if defined(_SECURITY_)
  csSIB.csSecurityOn = CS_SECURITY_ON;
#endif /* _SECURITY_ */

  /* Buffers cleaning */
#if defined(_COMMISSIONING_)
  memset(&stackBuffers.csNeibTable, 0, CS_NEIB_TABLE_SIZE);
  #if defined(_BINDING_) && (CS_APS_BINDING_TABLE_SIZE > 0)
    memset(&stackBuffers.csApsBindingTable, 0, CS_APS_BINDING_TABLE_SIZE);
  #endif /* _BINDING_ */
  #if defined(_SECURITY_)
  {
    csNIB.securityIB.securityLevel = CS_SECURITY_LEVEL;
    csNIB.securityIB.secureAllFrames = CS_SECURITY_ALL_FRAMES;
    NWK_ResetSecurityIB(NWK_GetDefaultPowerFailureControl());
  }
  #endif /* _SECURITY_ */
#endif /* _COMMISSIONING_ */
#if defined (_GROUP_TABLE_)
  memset(&stackBuffers.csGroupTable, 0, CS_GROUP_TABLE_SIZE);
#endif /* _GROUP_TABLE_ */
#if defined(_SECURITY_)
  #if defined(_TC_PERMISSION_TABLE_) && CS_MAX_TC_ALLOWED_DEVICES_AMOUNT > 0
    memset(&stackBuffers.csTcDevicePermissionTable, 0, CS_MAX_TC_ALLOWED_DEVICES_AMOUNT);
  #endif /* _TC_PERMISSION_TABLE_ */
  #if defined _LINK_SECURITY_ && CS_APS_KEY_PAIR_DESCRIPTORS_AMOUNT > 0

    memset(&stackBuffers.csApsKeyPairDescriptors, 0, CS_APS_KEY_PAIR_DESCRIPTORS_AMOUNT);
  #endif /* _LINK_SECURITY_ */

#endif /* _SECURITY_ */
#endif /* _MAC2_ */
}

/******************************************************************************
\brief Returns Configuration Server item by it's identifier

\param[in] itemId - ID of the parameter

\return - Configuration Server item
******************************************************************************/
static CS_MemoryItem_t csGetItem(CS_MemoryItemId_t itemId)
{
  CS_MemoryItem_t item;

  /* Read item info from the FLASH */
  memcpy_P(&item, &csMemoryItems[itemId], sizeof(CS_MemoryItem_t));
  assert(item.value.flashValue, CS_CSGETITEM0);
  return item;
}

/******************************************************************************
\brief Returns the location where item placed

\param[in] itemId - ID of the parameter

\return - item location: FLASH or RAM
******************************************************************************/
static CS_MemoryLocation_t csGetItemLocation(CS_MemoryItemId_t itemId)
{
  if (CS_FLASH_PARAMETERS_START_ID < itemId)
    return CS_MEMORY_LOCATION_FLASH;
  else
    return CS_MEMORY_LOCATION_RAM;
}

#if defined(_USE_KF_MAC_) && defined(_MAC_BAN_NODE_)
/******************************************************************************
\brief Returns the BAN table size

\return - the BAN table size
******************************************************************************/
MAC_BanTableSize_t csGetBanTableSize(void)
{
  MAC_BanTableSize_t size;

  memcpy_P(&size, &csReadOnlyItems.csMacBanTableSize, sizeof(MAC_BanTableSize_t));

  return size;
}

/******************************************************************************
\brief Returns the BAN table entry

\return - pointer to the BAN table
******************************************************************************/
MAC_BanTableEntry_t *csGetBanTableEntry(void)
{
  return stackBuffers.csBanTable;
}

#endif // defined(_USE_KF_MAC_) && defined(_MAC_BAN_NODE_)

/* eof configServer.c */
