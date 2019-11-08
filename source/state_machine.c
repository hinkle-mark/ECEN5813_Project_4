#include "state_machine.h"
#include "i2c.h"
#include "logger.h"
#include "gpio.h"
#include "TMP102.h"

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
	state_t next_state = *state; //init to original state in case of invalid input
	switch(*state)
	{
		case ST_TempReading:
			logString(LL_Normal, FN_stateMachineA, "Entering Temperature Read State");
			next_state = tempReading();
			break;

		case ST_AvgWait:
			logString(LL_Normal, FN_stateMachineA, "Entering Average/Wait State");
			next_state = avgWait();
			break;

		case ST_Alert:
			logString(LL_Normal, FN_stateMachineA, "Entering Temperature Alert State");
			next_state = tempAlert();
			break;

		case ST_Disconnect:
			logString(LL_Normal, FN_stateMachineA, "Entering Disconnect State");
			next_state = disconnect();
			break;

		default:
			/* Returns if input invalid - state remains unchanged */
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
	logString(LL_Normal, FN_stateMachineB, "Entering Next State");

	uint8_t idx = stateToIndex(*state);
	state_t next_state = STATE_TABLE[idx].handler();
	*state = next_state;
}

/* * * * *
 * Brief: tempReading - initiates an I2C read of TMP102 sensor and converts data
 * Returns: Next state to transition to
 * * * * */
state_t tempReading(void)
{
	/* Set Led */
	gpioGreenLEDOn();

	/* Read Sensor */
	logString(LL_Debug, FN_tempReading, "Initiating I2C Read from TMP102");
	read_status_t ret = TMP102TempRead(&data.raw);
	if(ret == error)
	{
		logString(LL_Debug, FN_tempReading, "Temperature Read Failed");
		return ST_Disconnect;
	}
	else
	{
		logString(LL_Test, FN_tempReading, "Temperature Read Successful");
	}

	/* Convert Temperature */
	logString(LL_Debug, FN_tempReading, "Converting Temperature Value");
	data.cur = TMP102ConvertT(data.raw);

	logTemperature(LL_Normal, FN_tempReading, data.cur);

	/* Check Value */
	if(data.cur >= TMP102_LOW_TEMP)
	{
		logString(LL_Test, FN_tempReading, "Temperature Above T_Low");
		return ST_AvgWait;
	}
	else
	{
		logString(LL_Test, FN_tempReading, "Temperature Below T_Low");
		return ST_Alert;
	}
}

/* * * * *
 * Brief: avgWait - averages the temperature data and waits 15 seconds
 * Returns: Next state to transition to
 * * * * */
state_t avgWait(void)
{
	/* Set Led */
	gpioGreenLEDOn();

	/* Calculate new avg value */
	logString(LL_Debug, FN_avgWait, "Calculating New Average Temperature");
	calculateAverage(data.cur);

	/* Wait 15 Seconds */
	logString(LL_Debug, FN_avgWait, "Beginning 15 Second Delay");
	delay(DELAY_15_SEC);
	num_timeouts++;

	logString(LL_Test, FN_avgWait, "Number of Timeouts:");
	logInteger(LL_Test, FN_avgWait, (uint32_t)num_timeouts);

	if(num_timeouts >= 4)
	{
		logString(LL_Debug, FN_avgWait, "Max Timeouts Completed: Changing State Machines");
		num_timeouts = 0;

		if(state_machine == SM_A)
			state_machine = SM_B;
		else if(state_machine == SM_B)
			state_machine = SM_A;
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
	/* Set Led */
	gpioBlueLEDOn();

	/* Read Sensor */
	logString(LL_Debug, FN_tempAlert, "Initiating I2C Read from TMP102");
	read_status_t ret = TMP102TempRead(&data.raw);
	if(ret == error)
	{
		logString(LL_Debug, FN_tempAlert, "Temperature Read Failed");
		return ST_Disconnect;
	}
	else
	{
		logString(LL_Test, FN_tempAlert, "Temperature Read Successful");
	}

	/* Convert Temperature */
	logString(LL_Debug, FN_tempAlert, "Converting Temperature Value");
	data.cur = TMP102ConvertT(data.raw);

	logTemperature(LL_Normal, FN_tempAlert, data.cur);

	return ST_AvgWait;
}

/* * * * *
 * Brief: disconnect - signals main to stop program
 * Returns: Next state to transition to
 * * * * */
state_t disconnect(void)
{
	/* Set Led */
	gpioRedLEDOn();

	logString(LL_Debug, FN_disconnect, "Ending Program...");

	state_machine = END;
	return ST_Disconnect;
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
	logString(LL_Debug, FN_initSMParameters, "Initializing Temperature Data and State Machine Parameters");

	data.raw        = 0;
	data.cur        = 0;
	data.avg 		= 0;
	data.total 		= 0;
	data.samples 	= 0;
	num_timeouts 	= 0;
}

