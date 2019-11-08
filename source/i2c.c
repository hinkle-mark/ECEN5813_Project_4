#include "i2c.h"
#include <stdbool.h>
#include "TMP102.h"
#include "logger.h"


/*
 * i2c0Init
 * @brief	Initialize the I2C module
 */
void i2c0Init(void)
{
	/* Route clock to I2C0 */
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;

	/* I2C ports are C8 (SCL) and C9 (SDA) */
	SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	/* Disable to allow configuration */
	I2C0->C1 &= ~(I2C_ENABLE_MASK);

	/* Configure Pullup resistors for SDA and SCL */
//	PORTB->PCR[0] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);
//	PORTB->PCR[1] |= (PORT_PCR_PE_MASK | PORT_PCR_PS_MASK);

	/* Configure peripheral for use */
//	I2C0->SLTH = 0;
//	I2C0->SLTL = 0x40;

	/* Write to I2C_F register */
	I2C0->F |= I2C_F_ICR(0x13) | I2C_F_MULT(0); // Mul = 0, SCL divider = 60	I2C baud rate = bus speed (Hz)/(mul × SCL divider)

	/* Enable values in Control Register */
	I2C0_EN();

	I2C0->C2 |= (I2C_C2_HDRS_MASK);
}


/*
 * i2c0WriteBytes
 * @brief Writes 2 bytes to the specified register
 *
 * @param[in] dev		Address of slave device
 *
 * @param[in] reg		Register to write value to
 *
 * @param[in] data		2 bytes of data to write
 *
 */
void i2c0WriteBytes(uint8_t dev, uint8_t reg, uint16_t data)
{
	uint8_t MSB, LSB;

	MSB = ((data & 0xFF00) >> 8);
	LSB = (data & 0x00FF);

	/* Put master in transmit mode, send start bit, send slave address and write */
	I2C_TRAN;
	I2C_READY;
	I2C_M_START;
	I2C0->D = ((dev << 1) | I2C_WRITE_CMD);
	I2C_IF_WAIT;

	I2C0->D = reg;
	I2C_IF_WAIT;

	I2C0->D = MSB;
	I2C_IF_WAIT;

	I2C0->D = LSB;
	I2C_IF_WAIT;
	I2C_M_STOP;

	logString(LL_Debug, FN_i2cWriteBytes, "Wrote the following data to TMP102");
	logInteger(LL_Debug, FN_i2cWriteBytes, (uint32_t)data);
}


/*
 * i2c0WriteBytes
 * @brief Writes 2 bytes to the specified register
 *
 * @param[in] dev		Address of slave device
 *
 * @param[in] reg		Register to read value from
 *
 * @returns				raw temp data
 *
 */
int16_t i2c0ReadBytes(uint8_t dev, uint8_t reg)
{
	int16_t data = 0;
	int8_t bytein;

	I2C_TRAN;
	I2C_READY;
	I2C_M_START;
	I2C0->D = ((dev << 1) | I2C_WRITE_CMD);
	I2C_IF_WAIT;
//	I2C_ACK_WAIT;

	I2C0->D = reg;
	I2C_IF_WAIT;
//	I2C_ACK_WAIT;

	I2C_M_RESTART;
	I2C0->D = ((dev << 1) | I2C_READ_CMD);
	I2C_IF_WAIT;
//	I2C_ACK_WAIT;

	I2C_REC;
	ACK;

	bytein = I2C0->D;		// Dummy Read
	I2C_IF_WAIT;

	ACK;
	bytein = I2C0->D;		// MSB READ

	I2C_IF_WAIT;
	data |= (int16_t) bytein << 8;

	NACK;
	bytein = I2C0->D;		//LSB

	data |= (int16_t) bytein;

	data = data >> 4;
	data = data & 0xFFF;
	I2C_IF_WAIT;
	I2C_M_STOP;

	logString(LL_Debug, FN_i2cReadBytes, "Read the following data from TMP102");
	logInteger(LL_Debug, FN_i2cReadBytes, (uint32_t)data);

	return data;
}
