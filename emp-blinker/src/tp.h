/****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: timepoint.h
*
* PROJECT....: emp-blinker
*
* DESCRIPTION: A TIMEPOINT object, used to track time.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* ---------------------------------------------------------------------------
* 190220  MA    Module created.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdlib.h>

#include "emp_type.h"
#include "driver.h"

/*****************************    Defines    *******************************/

typedef struct  TIMEPOINT TIMEPOINT;
typedef enum    TP_UNIT TP_UNIT;
typedef enum    TP_TYPE TP_TYPE;

// done instead of having to define e.g. void func(struct TIMEPOINT tp, )..

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

#define TIME_ARRAY_SIZE 4

/*************************  Function Interfaces ****************************/

/*************************    Class Functions    ***************************/

extern const struct TIMEPOINT_CLASS
{
	// Constructor & Destructor
	TIMEPOINT*	(*new)(TP_TYPE type);
	void		(*del)(TIMEPOINT* this);

	// Methods
	void		(*tick)(TIMEPOINT* this);

	void		(*set_callback)(TIMEPOINT* this, void(*callback)());
	void		(*set_systick)(TIMEPOINT* this, INT64U duration, TP_UNIT unit);

	void		(*set_value)(TIMEPOINT* this, INT64U time_array[TIME_ARRAY_SIZE]);
	INT64U		(*get_value)(TIMEPOINT* this, TP_UNIT unit);

	void		(*copy)(TIMEPOINT* des, TIMEPOINT* src);
	INT64U		(*delta)(TIMEPOINT* tp1, TIMEPOINT* tp2, TP_UNIT unit);
} tp;

/*****************************    Constructs   *****************************/

enum TP_TYPE
{
	NORMAL,
	SYSTEM
};

enum TP_UNIT
{
	ns,     // 0 = nanoseconds
	us,     // 1 = microseconds
	ms,     // 2 = milliseconds
	s       // 3 = seconds
};

struct TIMEPOINT
{
	INT64U  volatile time_array[TIME_ARRAY_SIZE]; // 0: ns, 1: us, 2: ms, 3: s
	INT64U  systick_dur_ns;

	TP_TYPE type : 1;
	FUNPTR  callback; //void(*callback)();
};

/****************************** End Of Module ******************************/
