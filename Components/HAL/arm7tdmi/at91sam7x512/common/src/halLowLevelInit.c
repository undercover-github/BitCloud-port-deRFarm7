/**************************************************************************//**
\file  halLowLevelInit.c

\brief Implementation of low level initialization.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    5/12/07 A. Khromykh - Created
*******************************************************************************/
/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/

/******************************************************************************
                   Includes section
******************************************************************************/
#include <AT91SAM7X512.h>
#include <dbgPort.h>
#include <types.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/
//#define HAL_ENABLE_AIC_DEBUG_MODE

#if defined(RF)
  #define PLL_MULTIPLIER       95
  #define PLL_DIVIDER          1
#elif defined(CRYSTAL_8MHz)
  #define PLL_MULTIPLIER       95
  #define PLL_DIVIDER          8
#elif defined(CRYSTAL_18d432MHz)
  #define PLL_MULTIPLIER       25
  #define PLL_DIVIDER          5
#else
  #error 'Unsupported reference clock source.'
#endif

#define RESET_ERSTL             0x01
#define RESET_KEY               0xA5

#define FLASH_USEC_CYCLE_NUM    75ul

/* After reset clock chip needs 240 us for exit from sleep
   or about 150 us (practice 143 us) for setup clock if it is in RX_ON_NOCLK state.
   1 nop is 24 us min (internal RC has 22..42 kHz) */
#define DELAY_FOR_RF_EXTCLCK_STABILIZE    NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP; \
                                          NOP
                                          
// Startup time of main oscillator (in number of slow clock ticks).
#define BOARD_OSCOUNT           (AT91C_CKGR_OSCOUNT & (0x40 << 8))

// USB PLL divisor value to obtain a 48MHz clock.
#define BOARD_USBDIV            AT91C_CKGR_USBDIV_1

// PLL frequency range.
#define BOARD_CKGR_PLL          AT91C_CKGR_OUT_0

// PLL startup time (in number of slow clock ticks).
#define BOARD_PLLCOUNT          (16 << 8)

// PLL MUL value.
#define BOARD_MUL               (AT91C_CKGR_MUL & (124 << 16))

// PLL DIV value.
#define BOARD_DIV               (AT91C_CKGR_DIV & 24) 

/***************************************************************************//*
 \brief This function performs very low level HW initialization
        this function can be use a Stack, depending the compilation
        optimization mode
******************************************************************************/
void halLowLevelInit(void)
{
  //configure_dbgu();
  //dbgu_print_ascii("HAL_Init():: ");
  uint8_t i = 0;

  /* EFC Init. Maximum Operating Frequency 48000000(Hz) > 30 MHz. Read operation is 2 cycle.
     Master clock is 48000000 Hz. It's approximately 50 cycle for 1 us or about 75 cycle in 1.5 us */

  /* Set flash wait states in the EFC
   **********************************/
  /* 48MHz = 1 wait state */
  AT91C_BASE_EFC0->EFC_FMR = AT91C_MC_FWS_1FWS;
  AT91C_BASE_EFC1->EFC_FMR = AT91C_MC_FWS_1FWS;

  /* enable clock on port A */
  AT91C_BASE_PMC->PMC_PCER = (1 << AT91C_ID_PIOA);


  /* Initialize main oscillator
   ****************************/

  /* Set MCK at 47 923 200 Hz. Crystal 18,432 MHz. */
  /* 1 Enabling the Main Oscillator: */
  /* Typically startup time 1.4 ms for 16 MHz crystal.
     Slow clock = 1/32768 = 30.51 us approximately.
     Start up time = 8 * 6 / SCK = 56 * 30.51 = 1,46484375 ms */
  AT91C_BASE_PMC->PMC_MOR = BOARD_OSCOUNT | AT91C_CKGR_MOSCEN;
  // Wait the startup time
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MOSCS));

  /* 2 Checking the Main Oscillator Frequency (Optional) */
  /* 3 Setting PLL and divider: */
  AT91C_BASE_PMC->PMC_PLLR = BOARD_CKGR_PLL | BOARD_PLLCOUNT
                             | BOARD_MUL | BOARD_DIV;

  AT91C_BASE_PMC->PMC_PLLR |= BOARD_USBDIV;

  // Wait the startup time
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_LOCK));

  /* 4. Selection of Master Clock and Processor Clock */
  /* select the PLL clock divided by 2 */
  AT91C_BASE_PMC->PMC_MCKR = AT91C_PMC_PRES_CLK_2 ;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY)); // <-- STOPS ON THI LINE !!! -->

  AT91C_BASE_PMC->PMC_MCKR |= AT91C_PMC_CSS_PLL_CLK ;
  while (!(AT91C_BASE_PMC->PMC_SR & AT91C_PMC_MCKRDY));


  /* Reset AIC: assign default handler for each interrupt source */
  AT91C_BASE_AIC->AIC_SVR[0] = (int)default_fiq_handler ;
  for (i = 1; i < 31; i++) {
    AT91C_BASE_AIC->AIC_SVR[i] = (int)default_irq_handler ;
  }
  AT91C_BASE_AIC->AIC_SPU = (int)default_spurious_handler;
  // Perform 8 IT acknowledge (write any value in EOICR)
  for (i = 0; i < 8 ; i++) {
    AT91C_BASE_AIC->AIC_EOICR = 0;
  }

#if !defined(HAL_USE_WDT)
  /* Watchdog Disable */
  AT91C_BASE_WDTC->WDTC_WDMR = AT91C_WDTC_WDDIS;
#endif

#ifdef HAL_ENABLE_AIC_DEBUG_MODE
  // Enable the Debug mode
  AT91C_BASE_AIC->AIC_DCR = AT91C_AIC_DCR_PROT;
#endif

  /* Enable hardware reset on RST pin. */
  /* The external reset is asserted during a time of 2^(RESET_ERSTL+1) slow clock cycles. */
  AT91C_BASE_RSTC->RSTC_RMR = AT91C_RSTC_URSTEN | (RESET_ERSTL << 8) | (RESET_KEY << 24); 
}

