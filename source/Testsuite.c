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


static void Test_CheckTempRange(void)
{
	UCUNIT_TestcaseBegin("Temperature Range Check");
	int16_t sensorData;
	TMP102TempRead(&sensorData);

	UCUNIT_CheckIsInRange(sensorData, (TMP102_LOW_TEMP >> 4), (TMP102_HI_TEMP >> 4));

	UCUNIT_TestcaseEnd();
}

static void Test_TempIsNegative(void)
{
	 UCUNIT_TestcaseBegin("Negative Temperature Test");
	int16_t sensorData;
	TMP102TempRead(&sensorData);

	UCUNIT_CheckIsBitSet(sensorData, 11);
	UCUNIT_TestcaseEnd();
}

static void Test_TempReturnSize(void)
{
	 UCUNIT_TestcaseBegin("Size of Temperature Data Returned Test");
	int16_t sensorData;
	TMP102TempRead(&sensorData);

	UCUNIT_CheckIs8Bit(sensorData); /* fail*/

	UCUNIT_CheckIs16Bit(sensorData); /* Pass */
	UCUNIT_CheckIs32Bit(sensorData); /* Pass */
	UCUNIT_TestcaseEnd();
}

static void Test_WholeNumberTemp()
{
	UCUNIT_TestcaseBegin("Temperature = Integer Test");
	int16_t sensorData;
	TMP102TempRead(&sensorData);

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
	TMP102Init();
	POSTres = TMP102POST();
	UCUNIT_CheckIsEqual(POSTres, 0);
	UCUNIT_TestcaseEnd();
}

static void Test_I2CEnabled()
{
	UCUNIT_TestcaseBegin("I2C Enable Test");

	I2C0->C1 &= ~(I2C_ENABLE_MASK);
	UCUNIT_CheckIsBitClear(I2C0->C1, 7);
	i2c0Init();
	UCUNIT_CheckIsBitSet(I2C0->C1, 7);
	UCUNIT_TestcaseEnd();
}

static void Test_I2CIntEnabled()
{
	UCUNIT_TestcaseBegin("I2C Interrupt Disabled Test");

	i2c0Init();
	UCUNIT_CheckIsBitClear(I2C0->C1, 6);
	UCUNIT_TestcaseEnd();
}

static void Test_I2CDataEmpty()
{
	UCUNIT_TestcaseBegin("I2C Data Buffer Clear Test");
	i2c0Init();

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
