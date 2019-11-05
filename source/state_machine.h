#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define NUM_STATES		4
#define DELAY_15_SEC    (48000000UL * 15)

typedef struct
{
	uint32_t 	raw;
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

float convertTemp(uint32_t raw_data);
void calculateAverage(float temp);
void initSMParameters();
void delay(uint32_t delay_time);

#endif /* _STATE_MACHINE_H */
