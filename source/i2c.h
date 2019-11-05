#ifndef _I2C_H
#define _I2C_H

#include <stdint.h>

typedef enum
{
	success,
	error
}I2CStatus_t;

void i2cInit(void);
I2CStatus_t i2cSetTLow(uint32_t t_low);
I2CStatus_t I2CReadTemperature(volatile uint32_t * raw_temp);

#endif /* _I2C_H */
