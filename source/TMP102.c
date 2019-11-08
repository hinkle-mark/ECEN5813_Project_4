
#include "TMP102.h"
#include "i2c.h"
#include "logger.h"


/************************************
 * TMP102Init
 * @brief	Initialize TMP102 with Hi and Low alarm values
 ************************************/

void TMP102Init()
{
	int16_t lowTempWrite	= (TMP102_LOW_TEMP_B0 << 8) | TMP102_LOW_TEMP_B1;
	int16_t highTempWrite 	= (TMP102_HI_TEMP_B0 << 8)  | TMP102_HI_TEMP_B1;

	i2c0WriteBytes(TMP102_SLAVE_ADDR, TMP102_TLOW_REG, lowTempWrite);
	i2c0WriteBytes(TMP102_SLAVE_ADDR, TMP102_THI_REG, highTempWrite);
}

/*
 * TMP102POST
 * @brief	Verify data written to TMP102 on initialization
 */
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

/*
 * TMP102TempRead
 * @brief	Reads the 'value' of the temperature register
 * @param[out] int16_t* rawData		Value of the data read
 *
 * @returns Status of read operation
 */
read_status_t TMP102TempRead(volatile int16_t * rawData)
{
	*rawData = i2c0ReadBytes(TMP102_SLAVE_ADDR, TMP102_TEMP_REG);
	logString(LL_Debug, FN_TMP102TempRead, "Temperature Raw Data Read Successfully");
	return success;
}


/*
 * TMP102ConvertT
 *
 * @brief Converts raw temperature data to Celcius
 *
 * @param[in] Data to convert
 *
 * @returns		Floating point value of temp in Celcius
 */

float TMP102ConvertT(volatile int16_t data)
{
	float temp_value;
	/* if the leading bit is a 1, the number is negative */
	if(data & 0x800)
	{
		data = ((~data) + 1) & 0xFFF;
		temp_value = (float) data * 0.0625 * -1;
	}
	else
	{
		temp_value = (float) data * 0.0625;
	}
	return temp_value;
}
