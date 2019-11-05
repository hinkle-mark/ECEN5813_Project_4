#include "state_machine.h"
#include "i2c.h"

/* Global Vars */
volatile sm_t state_machine;

/* Local Global Vars */
static volatile temperature_data_t data;
static volatile uint8_t  num_timeouts;

/* State Table */
static const st_element_t STATE_TABLE[NUM_STATES] = {
		{.st = ST_TempReading, 	.handler = &tempReading},
		{.st = ST_AvgWait, 		.handler = &avgWait},
		{.st = ST_Alert, 		.handler = &tempAlert},
		{.st = ST_Disconnect, 	.handler = &disconnect},
};


/* * * * *
 * Brief: stateMachineA - state oriented handler of program operation
 * Input: state (current state of system)
 * * * * */
void stateMachineA(state_t * state)
{
	state_t next_state;

	switch(*state)
	{
		case ST_TempReading:
			next_state = tempReading();
			break;

		case ST_AvgWait:
			next_state = avgWait();
			break;

		case ST_Alert:
			next_state = tempAlert();
			break;

		case ST_Disconnect:
			next_state = disconnect();
			break;

		default:
			break;
	}
	*state = next_state;
}

/* * * * *
 * Brief: stateMachineB - table oriented handler of program operation
 * Input: state (current state of system)
 * * * * */
void stateMachineB(state_t * state)
{
	state_t next_state = STATE_TABLE[(uint8_t)state].handler();
	*state = next_state;
}

/* * * * *
 * Brief: tempReading - initiates an I2C read of TMP102 sensor and converts data
 * Returns: Next state to transition to
 * * * * */
state_t tempReading(void)
{
	/* Read Sensor */
	I2CStatus_t ret = I2CReadTemperature(&data.raw);
	if(ret == error)
		return ST_Disconnect;

	/* Convert Temperature */
	data.cur = convertTemp(data.raw);

	/* Check Value */
	if(data.cur >= 0)
		return ST_AvgWait;
	else
		return ST_Alert;
}

/* * * * *
 * Brief: avgWait - averages the temperature data and waits 15 seconds
 * Returns: Next state to transition to
 * * * * */
state_t avgWait(void)
{
	/* Calculate new avg value */
	calculateAverage(data.cur);

	/* Wait 15 Seconds */
	delay(DELAY_15_SEC);
	num_timeouts++;
	if(num_timeouts >= 4)
	{
		num_timeouts = 0;
		state_machine = SM_B;
	}

	/*
	 * if cannot communicate
	 * return ST_Disconnect;
	 */

	return ST_TempReading;
}

/* * * * *
 * Brief: tempAlert - reacts to temperature below threshold and measures/converts
 * Returns: Next state to transition to
 * * * * */
state_t tempAlert(void)
{
	/* Read Sensor */
	I2CStatus_t ret = I2CReadTemperature(&data.raw);
	if(ret == error)
		return ST_Disconnect;

	/* Convert Temperature */
	data.cur = convertTemp(data.raw);

	return ST_AvgWait;
}

/* * * * *
 * Brief: disconnect - signals main to stop program
 * Returns: Next state to transition to
 * * * * */
state_t disconnect(void)
{
	state_machine = END;
	return ST_Disconnect;
}

/* * * * *
 * Brief: convertTemp - translates raw data into float
 * Input: raw_data
 * Returns: float value
 * * * * */
float convertTemp(uint32_t raw_data)
{
	/* Needs Work */
	float temp = -1.5 * raw_data;
	return temp;
}

/* * * * *
 * Brief: calculateAverage - calculates average with new temp reading
 * Input: temperature
 * * * * */
void calculateAverage(float temp)
{
	data.samples++;
	data.total += temp;
	data.avg = (data.total / data.samples);
}

/* * * * *
 * Brief: initSMParameters - initializes data used in state machine
 * * * * */
void initSMParameters()
{
	data.raw        = 0;
	data.cur        = 0;
	data.avg 		= 0;
	data.total 		= 0;
	data.samples 	= 0;
	num_timeouts 	= 0;
}

/*
 * Brief: delay - delays the processor
 * Input: delay_time (number of cycles to delay)
 */
void delay(uint32_t delay_time)
{
	for(volatile uint32_t i = 0; i < delay_time; i++)
	{
		__asm volatile("nop");
	}
}

