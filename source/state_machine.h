#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define NUM_STATES		4
#define DELAY_15_SEC    (3600000UL * 15)

typedef struct
{
	int16_t 	raw;
	float 		cur;
	float 		avg;

	/* Values to calculate average */
	float 		total;
	uint16_t 	samples;
} temperature_data_t;

typedef enum
{
	SM_A,
	SM_B,
	END
} sm_t;

typedef enum
{
	ST_TempReading 	= 0,
	ST_AvgWait		= 1,
	ST_Alert		= 2,
	ST_Disconnect	= 3
} state_t;

/* Function Pointer Typedef */
typedef state_t (* fnptr_t)(void);

typedef struct
{
	state_t st;
	fnptr_t handler;
} st_element_t;

extern volatile sm_t state_machine;

void stateMachineA(state_t * state);
void stateMachineB(state_t * state);

state_t tempReading(void);
state_t tempAlert(void);
state_t avgWait(void);
state_t disconnect(void);

void calculateAverage(float temp);
void initSMParameters();

/*
 * Brief: delay - delays the processor
 * Input: delay_time (number of cycles to delay)
 */
static inline void delay(uint32_t delay_time)
{
	for(volatile uint32_t i = 0; i < delay_time; i++)
	{
		__asm volatile("nop");
	}
}

/* * * * *
 * Brief: convertTemp - translates raw data into float
 * Input: raw_data
 * Returns: float value
 * * * * */
static inline float convertTemp(int16_t raw_data)
{
	float temp = (0.0625) * raw_data;
	return temp;
}

/*
 * Brief: stateToIndex - translates the state enum to uint for indexing state table
 * Input: state (current state)
 * Returns: Index of state table
 */
static inline uint8_t stateToIndex(state_t state)
{
	switch(state)
	{
		case ST_TempReading:
			return 0;
			break;

		case ST_AvgWait:
			return 1;
			break;

		case ST_Alert:
			return 2;
			break;

		case ST_Disconnect:
			return 3;
			break;

		default:
			return 4; //invalid
			break;
	}
}

#endif /* _STATE_MACHINE_H */
