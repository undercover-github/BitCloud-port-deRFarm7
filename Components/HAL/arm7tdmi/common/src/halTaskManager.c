/**************************************************************************//**
\file  halTaskManager.c

\brief Implemenattion of HAL task manager.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    22.08.07 A. Khromykh - Created
*******************************************************************************/
/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/

/******************************************************************************
                   Includes section
******************************************************************************/
#include <halTaskManager.h>
#include <atomic.h>
#include <dbgPort.h>

/******************************************************************************
                   Prototypes section
******************************************************************************/
extern void halAppTimerHandler(void);
extern void halSig2WireSerialHandler(void);
extern void halSigAdcHandler(void);
extern void halRfTxCompleteHandler(void);
extern void halRfRxCompleteHandler(void);
extern void halMemIrqCompleteHandler(void);
extern void halEndPointHandler(void);
extern void halSuspendHandler(void);
extern void halResumeHandler(void);
extern void halBusResetHandler(void);
extern void halSigUsartHandler(void);

/******************************************************************************
                   Global variables section
******************************************************************************/
volatile uint32_t halTaskFlags = 0;

/******************************************************************************
                   Implementations section
******************************************************************************/
void HAL_TaskHandler(void)
{  
  if (halTaskFlags & HAL_TIMER4_COMPA)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_TIMER4_COMPA);
    ATOMIC_SECTION_LEAVE
    halAppTimerHandler();
  }
  else if (halTaskFlags & HAL_SPI1_TXBUFE)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_SPI1_TXBUFE);
    ATOMIC_SECTION_LEAVE
    halRfTxCompleteHandler();
  }
  else if (halTaskFlags & HAL_SPI1_RXBUFF)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_SPI1_RXBUFF);
    ATOMIC_SECTION_LEAVE
    halRfRxCompleteHandler();
  }
  else if (halTaskFlags & HAL_SPI0_TXBUFE)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_SPI0_TXBUFE);
    ATOMIC_SECTION_LEAVE
    halMemIrqCompleteHandler();
  }
  else if (halTaskFlags & HAL_SPI0_RXBUFF)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_SPI0_RXBUFF);
    ATOMIC_SECTION_LEAVE
    halMemIrqCompleteHandler();
  }
#ifdef HAL_USE_USB
  else if (halTaskFlags & HAL_USB_ENDPOINTS)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_USB_ENDPOINTS);
    ATOMIC_SECTION_LEAVE
    halEndPointHandler();
  }
  else if (halTaskFlags & HAL_USB_SUSPEND)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_USB_SUSPEND);
    ATOMIC_SECTION_LEAVE
    halSuspendHandler();
  }
  else if (halTaskFlags & HAL_USB_RESUME)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_USB_RESUME);
    ATOMIC_SECTION_LEAVE
    halResumeHandler();
  }
  else if (halTaskFlags & HAL_USB_BUS_RESET)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_USB_BUS_RESET);
    ATOMIC_SECTION_LEAVE
    halBusResetHandler();
  }
#endif // HAL_USE_USB

#ifdef HAL_USE_USART
  else if (halTaskFlags & HAL_TASK_USART)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_TASK_USART);
    ATOMIC_SECTION_LEAVE
    halSigUsartHandler();
  }
#endif

#ifdef HAL_USE_ADC
  else if (halTaskFlags & HAL_ADC)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &=  (~HAL_ADC);
    ATOMIC_SECTION_LEAVE
    halSigAdcHandler();
  }
#endif

#ifdef HAL_USE_TWI
  else if (halTaskFlags & HAL_TWI)
  {
    ATOMIC_SECTION_ENTER
      halTaskFlags &= (~HAL_TWI);
    ATOMIC_SECTION_LEAVE
    halSig2WireSerialHandler();
  }
#endif

  if (halTaskFlags)
  {
    SYS_PostTask(HAL_TASK_ID);
  }
}
// eof halTaskManager.c
