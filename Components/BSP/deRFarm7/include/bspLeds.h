/***************************************************************************//**
\file  leds.h

\brief Declaration of leds interface.

\author
    Atmel Corporation: http://www.atmel.com \n
    Support email: avr@atmel.com

  Copyright (c) 2008-2012, Atmel Corporation. All rights reserved.
  Licensed under Atmel's Limited License Agreement (BitCloudTM).

\internal
  History:
    29/05/07 E. Ivanov - Created
*******************************************************************************/

#ifndef _BSPLEDS_H
#define _BSPLEDS_H

/******************************************************************************
                   Includes section
******************************************************************************/
#include <gpio.h>
#include <leds.h>

/******************************************************************************
                   Define(s) section
******************************************************************************/
#define halInitLed0()     GPIO_LED_0_make_out()
#define halUnInitLed0()   GPIO_LED_0_make_in()
#define halOnLed0()       GPIO_LED_0_set()
#define halOffLed0()      GPIO_LED_0_clr()
#define halReadLed0()     GPIO_LED_0_read()
#define halToggleLed0()   GPIO_LED_0_toggle()

#define halInitLed1()    GPIO_LED_1_make_out()
#define halUnInitLed1()  GPIO_LED_1_make_in()
#define halOnLed1()      GPIO_LED_1_set()
#define halOffLed1()     GPIO_LED_1_clr()
#define halReadLed1()    GPIO_LED_1_read()
#define halToggleLed1()  GPIO_LED_1_toggle()

#define halInitLed2()     GPIO_LED_2_make_out()
#define halUnInitLed2()   GPIO_LED_2_make_in()
#define halOnLed2()       GPIO_LED_2_set()
#define halOffLed2()      GPIO_LED_2_clr()
#define halReadLed2()     GPIO_LED_2_read()
#define halToggleLed2()   GPIO_LED_2_toggle()

#define halInitLed3()    GPIO_LED_3_make_out()
#define halUnInitLed3()  GPIO_LED_3_make_in()
#define halOnLed3()      GPIO_LED_3_set()
#define halOffLed3()     GPIO_LED_3_clr()
#define halReadLed3()    GPIO_LED_3_read()
#define halToggleLed3()  GPIO_LED_3_toggle()

#define halInitLed4()    GPIO_LED_4_make_out()
#define halUnInitLed4()  GPIO_LED_4_make_in()
#define halOnLed4()      GPIO_LED_4_set()
#define halOffLed4()     GPIO_LED_4_clr()
#define halReadLed4()    GPIO_LED_4_read()
#define halToggleLed4()  GPIO_LED_4_toggle()

#define halInitLed5()    GPIO_LED_5_make_out()
#define halUnInitLed5()  GPIO_LED_5_make_in()
#define halOnLed5()      GPIO_LED_5_set()
#define halOffLed5()     GPIO_LED_5_clr()
#define halReadLed5()    GPIO_LED_5_read()
#define halToggleLed5()  GPIO_LED_5_toggle()

#define halInitLed6()    GPIO_LED_6_make_out()
#define halUnInitLed6()  GPIO_LED_6_make_in()
#define halOnLed6()      GPIO_LED_6_set()
#define halOffLed6()     GPIO_LED_6_clr()
#define halReadLed6()    GPIO_LED_6_read()
#define halToggleLed6()  GPIO_LED_6_toggle()

#define halInitLed7()    GPIO_LED_7_make_out()
#define halUnInitLed7()  GPIO_LED_7_make_in()
#define halOnLed7()      GPIO_LED_7_set()
#define halOffLed7()     GPIO_LED_7_clr()
#define halReadLed7()    GPIO_LED_7_read()
#define halToggleLed7()  GPIO_LED_7_toggle()

#endif /*_BSPLEDS_H*/
// eof leds.h
