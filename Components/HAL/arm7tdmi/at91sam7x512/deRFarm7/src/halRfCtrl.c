/**************************************************************************//**
\file   halRfCtrl.c

\brief  mac pin interface implementation.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    28/05/07 ALuzhetsky - Created.
    06/08/07 A. Mandychev, E. Ivanov - Modified.
    24/08/07 A. Khromykh - modified (for arm)
******************************************************************************/
/******************************************************************************
 *   WARNING: CHANGING THIS FILE MAY AFFECT CORE FUNCTIONALITY OF THE STACK.  *
 *   EXPERT USERS SHOULD PROCEED WITH CAUTION.                                *
 ******************************************************************************/

/******************************************************************************
                        Includes section.
******************************************************************************/
#include <halClkCtrl.h>
#include <halRfSpi.h>
#include <halRfPio.h>
#include <halRfCtrl.h>
#include <derfBoardDefinitions.h>
#include <stddef.h>
#include <halW1.h>
#include <halMacIsr.h>
#include <logs.h>
/******************************************************************************
                   Define(s) section
******************************************************************************/
#if defined(PLATFORM_deRFarm7) || defined(PLATFORM_CUSTOM_1) || defined(PLATFORM_CUSTOM_2)
  #define IRQ_RF_NPC  AT91C_PIO_PA29
  #define RF_ID_IRQ   AT91C_ID_FIQ
 
#elif defined(PLATFORM_ZIGBIT_LAN) || defined(PLATFORM_ZIGBIT_TURBO)
  #define IRQ_RF_NPC  AT91C_PIO_PA29
  #define RF_ID_IRQ   AT91C_ID_FIQ
#endif

/******************************************************************************
                   Prototypes section
******************************************************************************/
#ifdef _HAL_RF_RX_TX_INDICATOR_
/**************************************************************************//**
\brief  Turn on pin 1 (DIG3) and pin 2 (DIG4) to indicate the transmit state of
the radio transceiver.
******************************************************************************/
void phyRxTxSwitcherOn(void);

#endif //_HAL_RF_RX_TX_INDICATOR_

#ifdef _HAL_ANT_DIVERSITY_
/**************************************************************************//**
\brief  Enable antenna diversity feature.
******************************************************************************/
void phyAntennaDiversityInit(void);

/**************************************************************************//**
\brief  Enable antenna diversity in the receive state of the radio transceiver.
******************************************************************************/
void phyRxAntennaDiversity(void);

/**************************************************************************//**
\brief  Enable antenna diversity in the transmit state of the radio transceiver.
******************************************************************************/
void phyTxAntennaDiversity(void);

#endif //_HAL_ANT_DIVERSITY_

/******************************************************************************
                    Implementations section
******************************************************************************/
/******************************************************************************
  Delay in us
******************************************************************************/
void HAL_Delay(uint8_t us)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  halDelayUs(us);
}

/******************************************************************************
  Clear the irq.
******************************************************************************/
void HAL_ClearRfIrqFlag(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  /* Clear the interrupt on the interrupt controller */
  AT91C_BASE_AIC->AIC_ICCR = (1ul << RF_ID_IRQ);
}

/******************************************************************************
  Enables the irq.
******************************************************************************/
void HAL_EnableRfIrq(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  /* Enable the interrupt on the interrupt controller */
  AT91C_BASE_AIC->AIC_IECR = (1ul << RF_ID_IRQ);
}

/******************************************************************************
  Disables the irq.
******************************************************************************/
uint8_t HAL_DisableRfIrq(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  /* Disable the interrupt on the interrupt controller */
  if (AT91C_BASE_AIC->AIC_IMR & (1ul << RF_ID_IRQ))
  { /* interrupt was enabled */
    /* disable interrupt */
    AT91C_BASE_AIC->AIC_IDCR = (1ul << RF_ID_IRQ);
    return 1;
  }
  else
    return 0;
}

/******************************************************************************
  Sets SLP_TR pin to 1.
******************************************************************************/
void HAL_SetRfSlpTr(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_RF_SLP_TR_set();
}

/******************************************************************************
  Clears SLP_TR pin to 0.
******************************************************************************/
void HAL_ClearRfSlpTr(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_RF_SLP_TR_clr();
}

/******************************************************************************
  Makes SLP_TR pin as input.
******************************************************************************/
void HAL_MakeInRfSlpTr(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_RF_SLP_TR_make_in();
}

/******************************************************************************
  Makes SLP_TR pin as input.
******************************************************************************/
void HAL_MakeOutRfSlpTr(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_RF_SLP_TR_make_out();
}

/******************************************************************************
  Sets RST_TR pin to 1.
******************************************************************************/
void HAL_SetRfRst(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_RF_RST_set();
}

/******************************************************************************
  Clears RST_TR pin to 1.
******************************************************************************/
void HAL_ClearRfRst(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_RF_RST_clr();
}

/******************************************************************************
  Deselects a slave device.
******************************************************************************/
void HAL_DeselectRfSpi(void)
{
	////dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_SPI_CS_set();
}

/******************************************************************************
  Selects a slave device.
******************************************************************************/
void HAL_SelectRfSpi(void)
{
	////dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  GPIO_SPI_CS_clr();
}

/******************************************************************************
  Init pins that controls RF chip.
******************************************************************************/
void HAL_InitRfPins(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");

  //PERIPHERAL_CLOCK_ENABLE(AT91C_ID_PIOA);
  //PERIPHERAL_CLOCK_ENABLE(AT91C_ID_PIOB);

  GPIO_RF_SLP_TR_clr();
  GPIO_RF_SLP_TR_make_out();


  GPIO_SPI_CS_make_out();

  GPIO_RF_RST_set();
  GPIO_RF_RST_make_out();

}

/******************************************************************************
  Initialization rf irq.
******************************************************************************/
void HAL_InitRfIrq(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  /* enable pio peripheral functions */
  AT91C_BASE_PIOA->PIO_ASR = IRQ_RF_NPC;
  AT91C_BASE_PIOA->PIO_PDR = IRQ_RF_NPC;

  //AT91C_BASE_PIOA->PIO_ODR = IRQ_RF_NPC;
  //AT91C_BASE_PIOA->PIO_PER = IRQ_RF_NPC;

  /* Enable interrupts */
  /* Disable the interrupt on the interrupt controller */
  AT91C_BASE_AIC->AIC_IDCR = (1ul << RF_ID_IRQ);
  /* Save the interrupt handler routine pointer and the interrupt priority */
  AT91C_BASE_AIC->AIC_SVR[RF_ID_IRQ] = (uint32_t)irqRfHandler;
  /* Store the Source Mode Register */
  AT91C_BASE_AIC->AIC_SMR[RF_ID_IRQ] = AT91C_AIC_SRCTYPE_POSITIVE_EDGE | AT91C_AIC_PRIOR_LOWEST;
  /* Clear the interrupt on the interrupt controller */
  AT91C_BASE_AIC->AIC_ICCR = (1ul << RF_ID_IRQ);
  /* Enable the interrupt on the interrupt controller if pointer correct*/
  AT91C_BASE_AIC->AIC_IECR = (1ul << RF_ID_IRQ);
}

/******************************************************************************
  Returns current frequency code.
******************************************************************************/
HalSysFreq_t HAL_GetRfFreq(void)
{
  return HAL_FREQ_1MHZ; //ORIGIN 1Mhz
}

/******************************************************************************
 initialization rf spi
******************************************************************************/
void HAL_InitRfSpiMode(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  HAL_RfSpiMode_t mode;

  mode.csmode = HAL_SPI_SOFTWARE_CS;
  mode.dmamode = HAL_SPI_BYTE;
  //mode.dmamode = HAL_SPI_DMA;
  HAL_InitRfSpi(&mode);
}

/**************************************************************************//**
  \brief Enables RX TX indicator for radio if that is supported.
******************************************************************************/
void HAL_EnableRxTxSwitcher(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  #ifdef _HAL_RF_RX_TX_INDICATOR_
    phyRxTxSwitcherOn();
  #endif //_HAL_RF_RX_TX_INDICATOR_
}

/**************************************************************************//**
  \brief Enables Antenna diversity for radio if that is supported.
******************************************************************************/
void HAL_InitAntennaDiversity(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  #ifdef _HAL_ANT_DIVERSITY_
    phyAntennaDiversityInit();
  #endif //_HAL_ANT_DIVERSITY_
}

/**************************************************************************//**
  \brief Enables Antenna diversity in RX mode for radio if that is supported.
******************************************************************************/
void HAL_EnableRxAntennaDiversity(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  #ifdef _HAL_ANT_DIVERSITY_
    phyRxAntennaDiversity();
  #endif //_HAL_ANT_DIVERSITY_
}

/**************************************************************************//**
  \brief Enables Antenna diversity in TX mode for radio if that is supported.
******************************************************************************/
void HAL_EnableTxAntennaDiversity(void)
{
	//dbgu_print_ascii(__FUNCTION__);//dbgu_print_ascii("\n");
  #ifdef _HAL_ANT_DIVERSITY_
    phyTxAntennaDiversity();
  #endif //_HAL_ANT_DIVERSITY_
}

//eof halRfCtrl.c
