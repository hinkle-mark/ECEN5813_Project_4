/*****************************************************************************
 *                                                                           *
 *  uCUnit - A unit testing framework for microcontrollers                   *
 *                                                                           *
 *  (C) 2007 - 2008 Sven Stefan Krauss                                       *
 *                  https://www.ucunit.org                                   *
 *                                                                           *
 *  File        : Testsuite.h                                                *
 *  Description : Sample testsuite                                           *
 *  Author      : Sven Stefan Krauss                                         *
 *  Contact     : www.ucunit.org                                             *
 *                                                                           *
 *****************************************************************************/

/*
 * This file is part of ucUnit.
 *
 * You can redistribute and/or modify it under the terms of the
 * Common Public License as published by IBM Corporation; either
 * version 1.0 of the License, or (at your option) any later version.
 *
 * uCUnit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with uCUnit.
 *
 * It may also be available at the following URL:
 *       http://www.opensource.org/licenses/cpl1.0.txt
 *
 * If you cannot obtain a copy of the License, please contact the
 * author.
 */

#include "System.h"
#include "uCUnit.h"
#include "Testsuite.h"
#include "TMP102.h"
#include "i2c.h"

static void Test_BasicChecksDemo(void)
{
    char * s = NULL;
    int a = 0;
    int b = 0;
    int c = 0;

    UCUNIT_TestcaseBegin("DEMO:Simple Checks");
    UCUNIT_CheckIsEqual( 0, a ); /* Pass*/
    UCUNIT_CheckIsEqual( 0, b ); /* Pass */
    UCUNIT_CheckIsEqual( 0, c ); /* Pass */

    UCUNIT_CheckIsEqual( 0, (1-1) ); /* Pass */
    UCUNIT_CheckIsEqual( 0, (-1+1) ); /* Pass */
    UCUNIT_CheckIsEqual( -1, (-2+1) ); /* Pass */

    UCUNIT_CheckIsNull(s); /* Pass */
    UCUNIT_TestcaseEnd();
}

static void Test_PointersDemo(void)
{
    char * s = NULL;

    UCUNIT_TestcaseBegin("DEMO:Checking pointers");

    UCUNIT_CheckIsNull(s); /* Pass */

    /* Empty string has a 0 byte/word as end of string */
    s = "";

    UCUNIT_CheckIsNotNull(s); /* Pass */
    UCUNIT_Check( (*s)=='\00' , "No end-of-string found","s" ); /* Pass */

    /* Check if first character of string "Hello World!\n" is a 'H' */
    s = "Hello World!\n";
    UCUNIT_CheckIsNotNull(s); /* Pass */
    UCUNIT_Check( (*s)=='H' , "String starts not with 'H'","s" ); /* Pass */

    UCUNIT_TestcaseEnd(); /* Pass */
}

static void Test_ChecklistDemo(void)
{
    int a = 0;
    int b = 0;
    int c = 0;

    UCUNIT_TestcaseBegin("DEMO:Checklists");

    /* Check if calculation works */
    a = 10;
    b = -20;
    c = a + b;

    UCUNIT_ChecklistBegin(UCUNIT_ACTION_WARNING);
    UCUNIT_CheckIsEqual( -10, c ); /* Pass */
    UCUNIT_CheckIsInRange(a,0,10); /* Pass */
    UCUNIT_CheckIsInRange(a,11,20); /* Fail */
    UCUNIT_CheckIsInRange(a,0,9); /* Fail */
    UCUNIT_ChecklistEnd();

    UCUNIT_TestcaseEnd(); /* Fail */
}

static void Test_BitChecksDemo(void)
{
    UCUNIT_TestcaseBegin("DEMO:Checking Bits");

    UCUNIT_CheckIsBitSet(0x0001, 0); /* Pass */
    UCUNIT_CheckIsBitSet(0x0000, 0); /* Fail */
    UCUNIT_CheckIsBitSet(0x0002, 1); /* Pass */
    UCUNIT_CheckIsBitSet(0x0000, 6); /* Fail */
    UCUNIT_CheckIsBitSet(0xFFFF, 6); /* Pass */

    UCUNIT_CheckIsBitClear(0x0001, 0); /* Fail */
    UCUNIT_CheckIsBitClear(0x0001, 1); /* Pass */

    UCUNIT_TestcaseEnd(); /* Fail */
}

static void Test_CheckTracepointsDemo(void)
{

    int found = FALSE;
    int i = 0;
    unsigned short a = 0;

    UCUNIT_TestcaseBegin("DEMO:Tracepoints");

    a = 0x1234;
    UCUNIT_ResetTracepointCoverage();
    UCUNIT_Tracepoint(0); /* Pass */
    UCUNIT_CheckIs8Bit(a); /* Fail */
    UCUNIT_CheckIs16Bit(a); /* Pass */
    UCUNIT_CheckIs32Bit(a); /* Pass */
    UCUNIT_Tracepoint(1); /* Pass */

    UCUNIT_CheckTracepointCoverage(0); /* Pass */
    UCUNIT_CheckTracepointCoverage(1); /* Pass */

    UCUNIT_ResetTracepointCoverage();
    for (i = 0; (i < 50) && (!found); i++)
    {
        if (i == 25)
        {
            UCUNIT_Tracepoint(0); /* Never executed */
            break;
        }

        if (i == 10)
        {
            UCUNIT_Tracepoint(1); /* Executed */
            found = TRUE;
        }

        if (i == 15)
        {
            UCUNIT_Tracepoint(2); /* Never executed */
            found = TRUE;
        }
    }

    UCUNIT_CheckTracepointCoverage(0); /* Fail  */
    UCUNIT_CheckTracepointCoverage(1); /* Pass */
    UCUNIT_CheckTracepointCoverage(2); /* Fail */

    UCUNIT_TestcaseEnd(); /* Fail */
}

static void Test_CheckTempRange(void)
{
	 UCUNIT_TestcaseBegin("Temperature Range Check");
	uint16_t sensorData;
	sensorData = TMP102_TempRead();

	UCUNIT_CheckIsInRange(sensorData, (TMP102_LOW_TEMP >> 4), (TMP102_HI_TEMP >> 4));

	UCUNIT_TestcaseEnd();
}

static void Test_TempIsNegative(void)
{
	 UCUNIT_TestcaseBegin("Negative Temperature Test");
	uint16_t sensorData;
	sensorData = TMP102_TempRead();

	UCUNIT_CheckIsBitSet(sensorData, 11);
	UCUNIT_TestcaseEnd();
}

static void Test_TempReturnSize(void)
{
	 UCUNIT_TestcaseBegin("Size of Temperature Data Returned Test");
	uint16_t sensorData;
	sensorData = TMP102_TempRead();

	UCUNIT_CheckIs8Bit(sensorData); /* fail*/

	UCUNIT_CheckIs16Bit(sensorData); /* Pass */
	UCUNIT_CheckIs32Bit(sensorData); /* Pass */
	UCUNIT_TestcaseEnd();
}

static void Test_WholeNumberTemp()
{
	UCUNIT_TestcaseBegin("Temperature = Integer Test");
	uint16_t sensorData;
	sensorData = TMP102_TempRead();

	UCUNIT_CheckIsBitClear(sensorData, 0);
	UCUNIT_CheckIsBitClear(sensorData, 1);
	UCUNIT_CheckIsBitClear(sensorData, 2);
	UCUNIT_CheckIsBitClear(sensorData, 3);
	UCUNIT_TestcaseEnd();
}

static void Test_VerifyPOST(void)
{
	UCUNIT_TestcaseBegin("POST verification test");
	int8_t POSTres;
	TMP102_Init();
	POSTres = TMP102_POST();
	UCUNIT_CheckIsEqual(POSTres, 0);
	UCUNIT_TestcaseEnd();
}

static void Test_I2CEnabled()
{
	UCUNIT_TestcaseBegin("I2C Enable Test");

	I2C0->C1 &= ~(I2C_ENABLE_MASK);
	UCUINT_CheckIsBitClear(I2C0->C1, 7);
	i2c0_Init();
	UCUINT_CheckIsBitSet(I2C0->C1, 7);
	UCUNIT_TestcaseEnd();
}

static void Test_I2CEnabled()
{
	UCUNIT_TestcaseBegin("I2C Interrupt Disabled Test");

	i2c0_Init();
	UCUINT_CheckIsBitClear(I2C0->C1, 6);
	UCUNIT_TestcaseEnd();
}

static void Test_I2CDataEmpty()
{
	UCUNIT_TestcaseBegin("I2C Data Buffer Clear Test");
	i2c0_Init();

	UCUNIT_CheckIsEqual(I2C0->D, 0);
}


void Testsuite_RunTests(void)
{
    Test_CheckTempRange();
    Test_TempIsNegative();
    Test_TempReturnSize();
    Test_WholeNumberTemp();
    Test_VerifyPOST();
    Test_I2CEnabled();
    Test_I2CIntEnabled();
    Test_I2CDataEmpty();

    UCUNIT_WriteSummary();
}


int test_main(void)
{
    UCUNIT_Init();
    UCUNIT_WriteString("\n**************************************");
    UCUNIT_WriteString("\nName:     ");
    UCUNIT_WriteString("uCUnit demo application");
    UCUNIT_WriteString("\nCompiled: ");
    UCUNIT_WriteString(__DATE__);
    UCUNIT_WriteString("\nTime:     ");
    UCUNIT_WriteString(__TIME__);
    UCUNIT_WriteString("\nVersion:  ");
    UCUNIT_WriteString(UCUNIT_VERSION);
    UCUNIT_WriteString("\n**************************************");
    Testsuite_RunTests();
    UCUNIT_Shutdown();

    return 0;
}
