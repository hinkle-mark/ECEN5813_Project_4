
#include "TMP102.h"
#include "i2c.h"
#include "logger.h"

/* PORTx_PCRn is the register to set for gpio interrupts from alert pin */

void TMP102Init()
{
	int16_t lowTempWrite	= (TMP102_LOW_TEMP_B0 << 8) + TMP102_LOW_TEMP_B1;
	int16_t highTempWrite 	= (TMP102_HI_TEMP_B0 << 8)  + TMP102_HI_TEMP_B1;

	i2c0WriteBytes(TMP102_SLAVE_ADDR, TMP102_TLOW_REG, lowTempWrite);
	i2c0WriteBytes(TMP102_SLAVE_ADDR, TMP102_THI_REG, highTempWrite);
}

/* Read data that was written in initialization and verify accuracy */
int TMP102POST()
{
	uint16_t lowTempRead, hiTempRead;
	lowTempRead = i2c0ReadBytes(TMP102_SLAVE_ADDR, TMP102_TLOW_REG);
	hiTempRead = i2c0ReadBytes(TMP102_SLAVE_ADDR, TMP102_THI_REG);

	if((lowTempRead == (TMP102_LOW_TEMP >> 4)) && (hiTempRead == (TMP102_HI_TEMP >> 4)))
	{
		logString(LL_Debug, FN_TMP102POST, "Power Up Self Test: Success");
		return 0;
	}
	else
	{
		logString(LL_Debug, FN_TMP102POST, "Power Up Self Test: Failed");
		return -1;
	}
}


void TMP102RegWrite(uint8_t reg, uint8_t data)
{

}

read_status_t TMP102TempRead(volatile int16_t * rawData)
{
	*rawData = i2c0ReadBytes(TMP102_SLAVE_ADDR, TMP102_TEMP_REG);
	logString(LL_Debug, FN_TMP102TempRead, "Temperature Raw Data Read Successfully");
	return success;
}

float TMP102ConvertT(volatile int16_t data)
{
	return 0.0625 * data;
}
