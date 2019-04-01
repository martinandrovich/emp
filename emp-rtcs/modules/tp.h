/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	tp.h
* MODULENAME.:	TIMEPOINT
* API........:	https://goo.gl/Z2FV18
* VERSION....:	1.2.0
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

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

#define TIME_ARRAY_SIZE 5

/*************************  Function Interfaces ****************************/

/*************************    Class Functions    ***************************/

extern const struct TIMEPOINT_CLASS
{
	// constructor & destructor
	TIMEPOINT*	(*new)(void);
	void		(*del)(TIMEPOINT* this);

	// methods
	void		(*systick)(void);
	void 		(*init_systick)(uint32_t duration_us, TP_UNIT unit);

	void		(*reset)(TIMEPOINT* this);
	void 		(*inc)(TIMEPOINT* this, uint32_t value, TP_UNIT unit);
	uint32_t	(*convert_us)(uint64_t value_us, TP_UNIT unit);
	void 		(*set)(TIMEPOINT* this, uint16_t time_array[TIME_ARRAY_SIZE]);
	uint64_t	(*get)(TIMEPOINT* this, TP_UNIT unit);
	uint16_t*	(*get_array)(TIMEPOINT* this);
	uint16_t*	(*now)(void);
	void		(*copy)(TIMEPOINT* des, TIMEPOINT* src);
	uint64_t	(*delta)(TIMEPOINT* tp1, TIMEPOINT* tp2, TP_UNIT unit);
	uint64_t	(*delta_now)(TIMEPOINT* tp1, TP_UNIT unit);
} tp;

/*****************************    Constructs   *****************************/

enum TP_UNIT
{
	us		= 0,     // 0 = microseconds
	ms		= 1,     // 1 = milliseconds
	 s		= 2,     // 2 = seconds
	 m		= 3,     // 3 = minutes
	 h		= 4,     // 4 = hours
};

struct TIMEPOINT
{
	uint16_t	time_array[TIME_ARRAY_SIZE]; // 0: us | 1: ms | 2: s | 3: m | 4: h

	//uint16_t	us;		// microseconds
	//uint16_t 	ms;		// milliseconds
	//uint8_t 	s;		// seconds
	//uint8_t 	m;		// minutes
	//uint8_t 	h;		// hours
};

/****************************** End Of Module ******************************/
