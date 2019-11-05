/*
 * File: logger.c
 * Description: This file handles the logger functionality for the program
 * Authors: Mike Fruge and Mark Hinkle
 */

#include "logger.h"

//TODO: Delete after development
#define TEST

/* Private Variable */
static bool LogEnable;

static const char * LoggerLevelStrings[NUM_LEVELS] = {
		"Test: ",
		"Debug: ",
		"Status: "
};

static const char * FunctionNameStrings[NUM_FUNCTIONS] = {
		"main: ",
		"RedLEDOn: ",
		"BlueLEDOn: ",
		"GreenLEDOn: "
};

void logInit()
{
	LogEnable = false;
}


void logEnable()
{
	if(LogEnable)
	{
		printf("Log already Enabled\n\r");
	}
	else
	{
		LogEnable = true;
		printf("Log Enabled\n\r");
	}

}


void logDisable()
{
	printf("Disabling Logger\n\r");
	if(LogEnable)
	{
		LogEnable = false;
	}
}


bool logStatus()
{
	return LogEnable;
}


void logData(LoggerLevel lvl, FunctionName fn, uint32_t* loc, size_t len)
{

	if(!LogEnable)
	{
		return;
	}

#ifdef NORMAL
	if(lvl == LL_Normal)
	{
		uint32_t* currLoc = loc;
		size_t i = 0;

		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("\n\r");

		for(i=0; i<len; i++)
		{
			printf("Address: 0x%p		Value: 0x%02X \n\r", currLoc, *currLoc);
			currLoc++;
		}
	}
#endif

#ifdef DEBUG
	if((lvl == LL_Debug) || (lvl == LL_Normal))
	{
		uint32_t* currLoc = loc;
		size_t i = 0;

		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("\n\r");

		for(i=0; i<len; i++)
		{
			printf("Address: 0x%p		Value: 0x%02X \n\r", currLoc, *currLoc);
			currLoc++;
		}
	}
#endif

#ifdef TEST
	uint32_t* currLoc = loc;
	size_t i = 0;

	printf(LoggerLevelStrings[lvl]);
	printf(FunctionNameStrings[fn]);
	printf("\n\r");

	for(i=0; i<len; i++)
	{
		printf("Address: 0x%p		Value: 0x%02X \n\r", currLoc, *currLoc);
		currLoc++;
	}
#endif
}


void logString(LoggerLevel lvl, FunctionName fn, char* message)
{
	if(!LogEnable)
	{
		return;
	}

#ifdef NORMAL
	if(lvl == LL_Normal)
	{
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("%s\n\r", message);
	}
#endif

#ifdef DEBUG
	if((lvl == LL_Debug) || (lvl == LL_Normal))
	{
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("%s\n\r", message);
	}
#endif

#ifdef TEST
	printf(LoggerLevelStrings[lvl]);
	printf(FunctionNameStrings[fn]);
	printf("%s\n\r", message);
#endif
}


void logInteger(LoggerLevel lvl, FunctionName fn, uint32_t value)
{
	/* Ensure Logger Enabled */
	if(!LogEnable)
	{
		return;
	}

#ifdef NORMAL
	if(lvl == LL_Normal)
	{
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("%u\n\r", value);
	}
#endif

#ifdef DEBUG
	if((lvl == LL_Debug) || (lvl == LL_Normal))
	{
		printf(LoggerLevelStrings[lvl]);
		printf(FunctionNameStrings[fn]);
		printf("%u\n\r", value);
	}
#endif

#ifdef TEST
	printf(LoggerLevelStrings[lvl]);
	printf(FunctionNameStrings[fn]);
	printf("%u\n\r", value);
#endif
}


