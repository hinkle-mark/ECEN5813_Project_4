#ifndef _I2C_H
#define _I2C_H

/* Includes */
#include "MKL25Z4.h"
#include <stdbool.h>
#include <stdint.h>

/* Defines */
#define I2C_M_START						I2C0->C1 |= I2C_C1_MST_MASK;
#define I2C_M_STOP						I2C0->C1 &= ~(I2C_C1_MST_MASK);
#define I2C_M_RESTART					I2C0->C1 |= I2C_C1_RSTA_MASK;
#define I2C_TRAN						I2C0->C1 |= I2C_C1_TX_MASK;
#define I2C_REC							I2C0->C1 &= ~(I2C_C1_TX_MASK);
#define NACK							I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK								I2C0->C1 &= ~(I2C_C1_TXAK_MASK)
#define I2C0_EN()						I2C0->C1 |= I2C_C1_IICEN_MASK

#define I2C_READY						while((I2C0->S & I2C_S_BUSY_MASK));
#define I2C_ACK_WAIT					while((I2C0->S & I2C_S_RXAK_MASK) == 1);
#define I2C_IF_WAIT						while((I2C0->S & I2C_S_IICIF_MASK) == 0) {} \
										I2C0->S |= I2C_S_IICIF_MASK;
#define I2C_IF_CLEAR					I2C0->

#define I2C_ENABLE_MASK					0x80
#define I2C_INT_ENABLE_MASK				0x40
#define I2C_MASTER_MODE_SELECT_MASK		0x20
#define BUS_PRESCALAR					0x12
#define I2C0_PORT						GPIOB
#define I2C0_SCL_PIN					0U
#define I2C0_SDA_PIN					1U
#define I2C_WRITE_CMD					0U
#define I2C_READ_CMD					1U
#define I2C_TC_FLAG						0x80
#define I2C_RXAK_FLAG					0x01

/* Function Prototypes */
void i2c0Init();
void i2c0WriteBytes(uint8_t dev, uint8_t reg, uint16_t data);
int16_t i2c0ReadBytes(uint8_t dev, uint8_t reg);

#endif /* _I2C_H */
