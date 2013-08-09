/**************************************************************************//**
\file  sysEvents.h

\brief Interface for subscription to event facility

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    28/02/12 D. Loskutnikov - Created
*******************************************************************************/
#ifndef _SYS_EVENTS_H
#define _SYS_EVENTS_H

#include <types.h>
#include <limits.h>

/******************************************************************************
                   Defines section
******************************************************************************/
// Should be enough for now
#define SYS_MAX_EVENTS 32U

/******************************************************************************
                   Types section
******************************************************************************/
typedef uint8_t SYS_EventId_t;

// Data for event. Arbitrary unsigned integer, but large enough to
// hold pointer if required (such use is generally discouraged)
typedef uintptr_t SYS_EventData_t;

// Select native types for 8-bit and 32-bit machines
#if UINT_MAX < 0xFFFFFFFFULL
typedef uint8_t sysEvWord_t;
#else
typedef uint32_t sysEvWord_t;
#endif

// Structure for declaring event receiver
typedef struct _SYS_EventReceiver_t
{
  // Internal service fields for handler.
  // Do not rely on them.
  // They may be changed at future releases.
  struct sysEventService_
  {
    struct _SYS_EventReceiver_t *next;
    sysEvWord_t evmask[SYS_MAX_EVENTS/sizeof(sysEvWord_t)/8U];
  } service;

  // Function to be called on event
  void (*func)(SYS_EventId_t id, SYS_EventData_t data);
} SYS_EventReceiver_t;

/******************************************************************************
                   Prototypes section
******************************************************************************/

/**************************************************************************//**
\brief Subscribe receiver to event. Same receiver may be subscribed to
multiple events by calling this function several times

\param[in] id - event id
\param[in] recv - receiver description
******************************************************************************/
void SYS_SubscribeToEvent(SYS_EventId_t id, SYS_EventReceiver_t *recv);

/**************************************************************************//**
\brief Unsubscribe receiver from event

\param[] id - event id
\param[] recv - receiver description
******************************************************************************/
void SYS_UnsubscribeFromEvent(SYS_EventId_t id, SYS_EventReceiver_t *recv);

/**************************************************************************//**
\brief Post an event to be delivered to all subscribed receivers

\param[in] id - event id
\param[in] data - associated data
******************************************************************************/
void SYS_PostEvent(SYS_EventId_t id, SYS_EventData_t data);

/**************************************************************************//**
\brief Check if event have at least one subscriber

\param[in] id - event if
\return result
******************************************************************************/
bool SYS_IsEventDeliverable(SYS_EventId_t id);

/**************************************************************************//**
\brief Check if specified receiver is subscribed to event.

\param[in] id - event if
\param[in] recv - receiver description

\return result
******************************************************************************/
bool SYS_IsEventSubscriber(SYS_EventId_t id, SYS_EventReceiver_t *recv);


#endif  // _SYS_EVENTS_H
//eof sysEvents.h
