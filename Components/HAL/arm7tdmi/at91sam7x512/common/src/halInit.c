/**************************************************************************//**
\file  halInit.c

\brief HAL start up module.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    31/08/07 A. Khromykh - Created
*******************************************************************************/
/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/

/******************************************************************************
                   Includes section
******************************************************************************/
#include <halRfCtrl.h>
#include <halAppClock.h>
#include <wdtCtrl.h>
#include <dbgPort.h>
#include <dbgu.h>

/******************************************************************************
                   Prototypes section
******************************************************************************/
/******************************************************************************
 Reads uid from external devices.
******************************************************************************/
void halReadUid(void);

/******************************************************************************
                   Implementations section
******************************************************************************/
#if defined(HAL_USE_WDT)
/******************************************************************************
wdt device interrupt handler.
******************************************************************************/
void halWdtHandler(void)
{
  if ((AT91C_BASE_WDTC->WDTC_WDSR & (AT91C_WDTC_WDUNF | AT91C_WDTC_WDERR)) && (AT91C_BASE_WDTC->WDTC_WDMR & AT91C_WDTC_WDFIEN))
  {
    wdtHandler();
  }
}

/******************************************************************************
  Performs hardware initialization of system controller device.
******************************************************************************/
void halInitSystemDevice(void)
{
  /* wdt initialization */
  HAL_StartWdt(WDT_INTERVAL_1000);

  /* Enable interrupts */
  /* Disable the interrupt on the interrupt controller */
  AT91C_BASE_AIC->AIC_IDCR = (1 << AT91C_ID_SYS);
  /* Save the interrupt handler routine pointer and the interrupt priority */
  AT91C_BASE_AIC->AIC_SVR[AT91C_ID_SYS] = (uint32_t)halWdtHandler;
  /* Store the Source Mode Register */
  AT91C_BASE_AIC->AIC_SMR[AT91C_ID_SYS] = AT91C_AIC_SRCTYPE_HIGH_LEVEL | AT91C_AIC_PRIOR_HIGHEST;
  /* Clear the interrupt on the interrupt controller */
  AT91C_BASE_AIC->AIC_ICCR = (1 << AT91C_ID_SYS);
  /* Enable the interrupt on the interrupt controller */
  AT91C_BASE_AIC->AIC_IECR = (1 << AT91C_ID_SYS);
}
#endif // #if defined(HAL_USE_WDT)

/******************************************************************************
Performs start up HAL initialization.
******************************************************************************/
void HAL_Init(void)
{
  //configure_dbgu();
  ////dbgu_print_ascii("HAL_Init() - entry\n");
  // start timer 0
  
  //HAL_OpenDbgu();
  
  //HAL_SendDbguMessage("HAL_Init:: ");
  ////dbgu_print_ascii("HAL_Init()::AppClock\n");
  halStartAppClock();
  // init spi0
  ////dbgu_print_ascii("HAL_Init()::RfSpi\n");
  HAL_InitRfSpiMode();
  // Reads unique ID
  ////dbgu_print_ascii("HAL_Init()::ReadUid - entry\n");
  halReadUid();
  ////dbgu_print_ascii("HAL_Init()::ReadUid - exit\n");
#if defined(HAL_USE_WDT)
  #if defined(FREE_RTOS)
    // wdt start, handler is calling freeRTOS
    ////dbgu_print_ascii("HAL_Init()::startWdt\n");
    HAL_StartWdt(WDT_INTERVAL_1000);
  #else
    // wdt initialization
    ////dbgu_print_ascii("HAL_Init()::initSystemDevice\n");
    halInitSystemDevice();
  #endif
#endif // #if defined(HAL_USE_WDT)
    ////dbgu_print_ascii("HAL_Init() - exit\n");
}
// eof halInit.c
