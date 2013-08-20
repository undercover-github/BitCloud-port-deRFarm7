/***************************************************************************//**
\file  leds.c

\brief The module to access to the leds.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    29/05/07 E. Ivanov - Created
*******************************************************************************/
#if APP_DISABLE_BSP != 1

/******************************************************************************
                   Includes section
******************************************************************************/
#include <bspLeds.h>

/******************************************************************************
                   Implementations section
******************************************************************************/
/****************************************************************
Init LED control module.
****************************************************************/
static void initLeds(void)
{
  halInitLed0();
  halInitLed1();
  halInitLed2();
  halInitLed3();
  halInitLed4();
  halInitLed5();
  halInitLed6();
  halInitLed7();
}

/****************************************************************
Opens leds module to use.
****************************************************************/
result_t BSP_OpenLeds(void)
{
  initLeds();
  halOffLed0();
  halOffLed1();
  halOffLed2();
  halOffLed3();
  halOffLed4();
  halOffLed5();
  halOffLed6();
  halOffLed7();
  return BC_SUCCESS;
}

/****************************************************************
Closes leds module.
****************************************************************/
result_t BSP_CloseLeds(void)
{
  halUnInitLed0();
  halUnInitLed1();
  halUnInitLed2();
  halUnInitLed3();
  halUnInitLed4();
  halUnInitLed5();
  halUnInitLed6();
  halUnInitLed7();
  return BC_SUCCESS;
}

/****************************************************************
Turn on LED.
Parameters:
    id - number of led
****************************************************************/
void BSP_OnLed(uint8_t id)
{
  switch(id)
  {
    case LED_FIRST:
          halOnLed0();
         break;
    case LED_SECOND:
          halOnLed1();
         break;
    case LED_THIRD:
          halOnLed2();
         break;
    case LED_FOURTH:
          halOnLed3();
         break;
    case LED_FIFTH:
          halOnLed4();
         break;
    case LED_SIXTH:
          halOnLed5();
         break;
    case LED_SEVENTH:
          halOnLed6();
         break;
    case LED_EIGHTH:
          halOnLed7();
         break;
  }
}

/****************************************************************
Turn off LED.
Parameters:
      id - number of led
****************************************************************/
void BSP_OffLed(uint8_t id)
{
  switch(id)
  {
    case LED_FIRST:
          halOffLed0();
         break;
    case LED_SECOND:
          halOffLed1();
         break;
    case LED_THIRD:
          halOffLed2();
         break;
    case LED_FOURTH:
          halOffLed3();
         break;
    case LED_FIFTH:
		 halOffLed4();
		 break;
	case LED_SIXTH:
		  halOffLed5();
		 break;
	case LED_SEVENTH:
		  halOffLed6();
		 break;
	case LED_EIGHTH:
		  halOffLed7();
		 break;
  }
}

/****************************************************************
Change LED state to opposite.
    Parameters:
      id - number of led
****************************************************************/
void BSP_ToggleLed(uint8_t id)
{
  switch(id)
  {
    case LED_FIRST:
          halToggleLed0();
         break;
    case LED_SECOND:
          halToggleLed1();
         break;
    case LED_THIRD:
          halToggleLed2();
         break;
    case LED_FOURTH:
          halToggleLed3();
         break;
    case LED_FIFTH:
		 halToggleLed4();
		 break;
	case LED_SIXTH:
		  halToggleLed5();
		 break;
	case LED_SEVENTH:
		  halToggleLed6();
		 break;
	case LED_EIGHTH:
		  halToggleLed7();
		 break;
  }
}

#endif // APP_DISABLE_BSP != 1

// eof leds.c
