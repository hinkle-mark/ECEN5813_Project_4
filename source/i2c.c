#include "i2c.h"

void i2cInit(void)
{
	//add stuff
	return;
}

I2CStatus_t i2cSetTLow(int16_t t_low)
{
	return success;
}

I2CStatus_t I2CReadTemperature(volatile int16_t * raw_temp)
{
	*raw_temp = 0x190;
	return success;
}
