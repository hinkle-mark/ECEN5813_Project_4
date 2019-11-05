/*
 * File: logger.h
 * Description: This file defines macros and function prototypes for logger
 * Authors: Mike Fruge and Mark Hinkle
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* Create Logging Structures */
/* Created in preparation for later assignments */

#define BUF_SIZE		50
#define NUM_LEVELS      3
#define NUM_FUNCTIONS 	4

#ifdef BOARD
#include "fsl_debug_console.h"
#define printf	PRINTF
#endif

/*** Logger Level Header Info ***/
typedef enum
{
	LL_Test 	= 0,
	LL_Debug 	= 1,
	LL_Normal 	= 2
} LoggerLevel;

/*** Function Name Header Info ***/
typedef enum
{
	FN_main					= 0,
	FN_redLEDOn             = 1,
	FN_blueLEDOn			= 2,
	FN_greenLEDOn			= 3
} FunctionName;

/***********************************************
 *
 * @brief	Initialize the global logger to its default state (disabled, WARNING, default buffer)
 *
 ***********************************************/
void logInit();

/***********************************************
 *
 * @brief	Enable Logger for printouts
 *
 ***********************************************/
void logEnable();

/***********************************************
 * @brief	Disable logger activity
 ***********************************************/
void logDisable();

/***********************************************
 * @brief	Check status of logger
 *
 * @return true if logger is enabled, false if not
 *
 ***********************************************/
bool logStatus();

/***********************************************
 * @brief	Prints out the address and values read from memory as well as logger info
  *
 * @param[in] lvl	Specified level of logging info
 *
 * @param[in] fn	Name of function that called logger
 *
 * @param[in] loc	Pointer to base address to read from memory
 *
 * @param[in] len	Number of words to read from memory
 ***********************************************/
void logData(LoggerLevel lvl, FunctionName fn, uint32_t* loc, size_t len);

/***********************************************
 * @brief	Print out input message as well as logger info
 *
 * @param[in] lvl		Specified level of logging info
 *
 * @param[in] fn		Name of function that called logger
 *
 * @param[in] message	Information to be logged
 ***********************************************/
void logString(LoggerLevel lvl, FunctionName fn, char* message);

/***********************************************
 * @brief	Print out input value as well as logger info
 *
 * @param[in] lvl		Specified level of logging info
 *
 * @param[in] fn		Name of function that called logger
 *
 * @param[in] value		Value to be logged
 ***********************************************/
void logInteger(LoggerLevel lvl, FunctionName fn, uint32_t value);
