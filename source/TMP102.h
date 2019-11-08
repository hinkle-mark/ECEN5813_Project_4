/* When ADD0 is wired to GND pin, device address is 1001000 */
/* Can use a frequency of 400kHz */

#include <stdint.h>

#define TMP102_SLAVE_ADDR		0x48

#define TMP102_TEMP_REG			0x00
#define TMP102_CFG_REG			0x01
#define TMP102_TLOW_REG			0x02
#define TMP102_THI_REG			0x03

#define TMP102_LOW_TEMP_B0		0x00
#define TMP102_LOW_TEMP_B1		0x00
#define TMP102_LOW_TEMP			((TMP102_LOW_TEMP_B0 << 8) + TMP102_LOW_TEMP_B1)

#define TMP102_HI_TEMP_B0		0x33
#define TMP102_HI_TEMP_B1		0x00
#define TMP102_HI_TEMP			((TMP102_HI_TEMP_B0 << 8) + TMP102_HI_TEMP_B1)


#define TMP102_CFG_DEFAULT		0x60A0

#define TMP102_CFG_TWO_FAULTS	0x0800
#define TMP102_CFG_FOUR_FAULTS	0x1000
#define TMP102_CFG_SIX_FAULTS	0x1800

typedef enum
{
	success,
	error
} read_status_t;


void TMP102Init();
int TMP102POST();
void TMP102RegWrite(uint8_t reg, uint8_t data);
read_status_t TMP102TempRead(volatile int16_t * rawData);
float TMP102ConvertT(volatile int16_t data);
