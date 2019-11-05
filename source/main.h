#ifndef _MAIN_H
#define _MAIN_H

#include <stdbool.h>
#include "fsl_gpio.h"

#define ALERT_PORT 		GPIOA
#define ALERT_PIN 		1

#define LOW_TEMP 		1
#define LOGGING_LEVEL 	LL_Normal



static inline bool pollAlertPin(void)
{
//	return 	GPIO_ReadPinInput(ALERT_PORT, ALERT_PIN);
	return 	false;
}

#endif /* _MAIN_H */
