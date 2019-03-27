/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	tp.h
* MODULENAME.:	TIMEPOINT
* API........:	https://goo.gl/Z2FV18
* VERSION....:	1.1.0
*
* DESCRIPTION:	Timepoint is an instance used for tracking time, including
				calculating delta, delta_systick etc.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>

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
	TIMEPOINT*	(*new)(void);
	void		(*del)(TIMEPOINT* this);

	// Methods
	void		(*systick)(void);
	void 		(*init_systick)(uint32_t duration, TP_UNIT unit);

	void 		(*inc)(TIMEPOINT* this, uint64_t value, TP_UNIT unit);
	void 		(*set)(TIMEPOINT* this, uint64_t time_array[TIME_ARRAY_SIZE]);
	uint64_t	(*get)(TIMEPOINT* this, TP_UNIT unit);
	uint64_t*	(*get_array)(TIMEPOINT* this);
	uint64_t*	(*now)(void);
	void		(*copy)(TIMEPOINT* des, TIMEPOINT* src);
	uint64_t	(*delta)(TIMEPOINT* tp1, TIMEPOINT* tp2, TP_UNIT unit);
	uint64_t	(*delta_now)(TIMEPOINT* tp1, TP_UNIT unit);
} tp;

/*****************************    Constructs   *****************************/

enum TP_UNIT
{
	ns,     // 0 = nanoseconds
	us,     // 1 = microseconds
	ms,     // 2 = milliseconds
	s       // 3 = seconds
};

struct TIMEPOINT
{
	uint64_t	time_array[TIME_ARRAY_SIZE]; // 0: ns, 1: us, 2: ms, 3: s
};

/****************************** End Of Module ******************************/
