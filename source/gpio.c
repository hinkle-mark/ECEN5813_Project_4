/*
 * File: flash.c
 * Description: This file handles the LED functionality on the KL25Z
 * Authors: Mike Fruge and Mark Hinkle
 */


#include <gpio.h>
#include "logger.h"

void gpioInit()
{
	/* Wanna see if this works */
	/* Initialize LEDS to on */
	LED_RED_INIT(LED_ON);
	LED_BLUE_INIT(LED_ON);
	LED_GREEN_INIT(LED_ON);

	/* Turn LEDs off */

	LED_RED_OFF();
	LED_BLUE_OFF();
	LED_GREEN_OFF();

	/* Init I2C Pins */
}


void gpioRedLEDOn()
{
	LED_RED_ON();
	LED_GREEN_OFF();
	LED_BLUE_OFF();
}

void gpioBlueLEDOn()
{
	LED_RED_OFF();
	LED_GREEN_OFF();
	LED_BLUE_ON();
}

void gpioGreenLEDOn()
{
	LED_RED_OFF();
	LED_GREEN_ON();
	LED_BLUE_OFF();
}


