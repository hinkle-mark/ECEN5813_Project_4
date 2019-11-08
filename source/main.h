#ifndef _MAIN_H
#define _MAIN_H

#include <stdbool.h>
#include "fsl_gpio.h"

#define ALERT_PORT 		GPIOD
#define ALERT_PIN 		4

#define LOW_TEMP 		1
#define LOGGING_LEVEL 	LL_Normal



static inline bool pollAlertPin(void)
{
	return 	GPIO_ReadPinInput(ALERT_PORT, ALERT_PIN);
}

#endif /* _MAIN_H */
