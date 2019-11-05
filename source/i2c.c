#include "i2c.h"

void i2cInit(void)
{
	//add stuff
	return;
}

I2CStatus_t i2cSetTLow(uint32_t t_low)
{
	return success;
}

I2CStatus_t I2CReadTemperature(volatile uint32_t * raw_temp)
{
	*raw_temp = 111111;
	return success;
}
