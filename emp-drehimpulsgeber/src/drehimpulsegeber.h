/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: adc.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Analog input from EMP-board pot-meter.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150412  MoH   Module created.
*
*****************************************************************************/

#pragma once

/***************************** Include files *******************************/
#include <stdint.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include "tm4c123gh6pm.h"

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

typedef struct DREHIMPULSEGEBER_MSG DREHIMPULSEGEBER_MSG;

/*************************    Class Functions    ***************************/

extern struct DREHIMPULSEGEBER_CLASS
{
	void        (* const init)();
	void        (* const task)(void *pm);
} drehimpulsegeber;


struct DREHIMPULSEGEBER_MSG
{

	int32_t pos         : 10;
	uint32_t revol      : 19;
	int32_t dir         : 2;
	uint32_t rst        : 1;

};


/****************************** End Of Module *******************************/
