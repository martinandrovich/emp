/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	tp.c
* MODULENAME.:	TIMEPOINT
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <math.h>
#include <assert.h>

#include "tp.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

static TIMEPOINT* 	tp_sys;
static uint32_t		systick_dur_us;

/************************  Function declarations ***************************/

static TIMEPOINT*	TIMEPOINT_new(void);
static void 		TIMEPOINT_del(TIMEPOINT* this);

static void 		TIMEPOINT_systick(void);
static void 		TIMEPOINT_init_systick(uint32_t duration_us, TP_UNIT unit);

static void 		TIMEPOINT_reset(TIMEPOINT* this);
static inline void 	TIMEPOINT_increment(TIMEPOINT* this, uint32_t value, TP_UNIT unit);
static uint32_t		TIMEPOINT_convert_us(uint64_t value_us, TP_UNIT unit);
static void 		TIMEPOINT_set(TIMEPOINT* this, uint16_t time_array[TIME_ARRAY_SIZE]);
static uint64_t		TIMEPOINT_get(TIMEPOINT* this, TP_UNIT unit);
static uint16_t*	TIMEPOINT_get_array(TIMEPOINT* this);
static uint16_t*	TIMEPOINT_now(void);
static void 		TIMEPOINT_copy(TIMEPOINT* des, TIMEPOINT* src);
static uint64_t 	TIMEPOINT_delta(TIMEPOINT* tp1, TIMEPOINT* tp2, TP_UNIT unit);
static uint64_t 	TIMEPOINT_delta_now(TIMEPOINT* tp1, TP_UNIT unit);

/****************************   Class Struct   *****************************/

const struct TIMEPOINT_CLASS tp =
{
	.new			= &TIMEPOINT_new,
	.del			= &TIMEPOINT_del,

	.systick		= &TIMEPOINT_systick,
	.init_systick	= &TIMEPOINT_init_systick,

	.reset			= &TIMEPOINT_reset,
	.inc			= &TIMEPOINT_increment,
	.convert_us		= &TIMEPOINT_convert_us,
	.set			= &TIMEPOINT_set,
	.get			= &TIMEPOINT_get,
	.get_array		= &TIMEPOINT_get_array,
	.now			= &TIMEPOINT_now,
	.copy			= &TIMEPOINT_copy,
	.delta			= &TIMEPOINT_delta,
	.delta_now		= &TIMEPOINT_delta_now
};

/***********************   Constructive Functions   ************************/

static TIMEPOINT* TIMEPOINT_new()
{
	// allocate memory
	TIMEPOINT* this = malloc(sizeof(TIMEPOINT));

	// initialize time_array
	//for (int i = 0; i < sizeof(tp->time_array)/sizeof(uint64_t); i++)
	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		this->time_array[i] = 0;
	}

	// memset everything to zero
	tp.reset(this);

	// return pointer to instance
	return this;
}

static void TIMEPOINT_reset(TIMEPOINT* this)
{
	memset(this, 0, sizeof(TIMEPOINT));
}

static void TIMEPOINT_del(TIMEPOINT* this)
/****************************************************************************
*   Input    : Pointer to a TIMEPOINT instance.
*   Function : Destructor of a TIMEPOINT instance.
****************************************************************************/
{
	free(this);
}

/*****************************   Functions   *******************************/

static void TIMEPOINT_systick(void)
/****************************************************************************
*   Input    : Pointer to TIMEPOINT instance.
*   Function : Increment TIMEPOINT with a systick unit.
****************************************************************************/
{
	// increment system timepoint by duration of a SYSTICK
	assert(tp_sys != NULL);
	tp.inc(tp_sys, systick_dur_us, us);
}

static void TIMEPOINT_init_systick(uint32_t duration_us, TP_UNIT unit)
/****************************************************************************
*   Input    : Pointer to TIMEPOINT instance.
*   Function : Increment TIMEPOINT with a systick unit.
****************************************************************************/
{
	tp_sys 			= tp.new();
	systick_dur_us	= duration_us * pow(10, 3 * unit);
}

static inline void TIMEPOINT_increment(TIMEPOINT* this, uint32_t value, TP_UNIT unit)
/****************************************************************************
*   Input    : this = pointer to TIMEPOINT instance.
			   value = ammount to increment (unit defined by index).
			   unit = index of time_array (0 = ns, 1 = us, 2 = ms ...).
*   Function : Recurisve increment of TIMEPOINT, until no overflow.
****************************************************************************/
{
	uint16_t divisor;

	// add value
	this->time_array[unit] += value;

	// select arithemtic divisor value
	switch (unit)
	{
		case us:
		case ms:

			divisor = 1000;
			break;

		case s:
		case m:

			divisor = 60;
			break;

		case h:

			divisor = 24;
			break;

		default:

			// waaaay too far
			tp.reset(this);
			return;
	}


	// calculate remainder and quotient
	uint16_t remainder	= this->time_array[unit] % divisor;
	uint16_t quotient	= this->time_array[unit] / divisor;

	// check if remainder has overflown
	if (remainder != this->time_array[unit])
	{
		this->time_array[unit] = remainder;
		TIMEPOINT_increment(this, quotient, unit + 1);
	}
}

static uint32_t TIMEPOINT_convert_us(uint64_t value_us, TP_UNIT unit)
{
	switch (unit)
	{
		case us:
		case ms:
		case s:

			return (value_us * pow(10, -3 * unit));

		case m:

			return (value_us * pow(10, -3 * unit) / 60);

		case h:

			return (value_us * pow(10, -3 * unit) / 3600);;

		default:

			return 0;
	}
}

static void TIMEPOINT_set(TIMEPOINT* this, uint16_t time_array[TIME_ARRAY_SIZE])
/****************************************************************************
*   Input    : this: Pointer to TIMEPOINT instance.
			   time_array[]: Array with new values.
*   Function : Set the time_array values of a TIMEPOINT instance, in
			   the order ns, us, ms & s.
****************************************************************************/
{
	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		this->time_array[i] = time_array[i];
	}
}

static uint64_t TIMEPOINT_get(TIMEPOINT* this, TP_UNIT unit)
/****************************************************************************
*   Input    : this = Pointer to TIMEPOINT instance.
			   unit = desired TP_UNIT.
*   Output   : Unsigned long long integer.
*   Function : Return the value of a TIMEPOINT instance given in
			   unit defined by TP_UNIT.
****************************************************************************/
{
	// sum everything into microseconds
	uint64_t sum_us =
		this->time_array[0] +						// us
		this->time_array[1] * pow(10,  3) +			// ms
		this->time_array[2] * pow(10,  6) +			// s
		this->time_array[3] * pow(10,  6) * 60 +	// m
		this->time_array[4] * pow(10, 12) * 3600;	// h

	// return value according to unit
	return tp.convert_us(sum_us, unit);
}

static uint16_t* TIMEPOINT_get_array(TIMEPOINT* this)
/****************************************************************************
*   Input    : this = Pointer to TIMEPOINT instance.
			   unit = desired TP_UNIT.
*   Output   : Pointer to a time_array[]
*   Function : Return the time_array of a TIMEPOINT instance.
****************************************************************************/
{
	static uint16_t time_array_copy[TIME_ARRAY_SIZE];

	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		time_array_copy[i] = this->time_array[i];
	}

	return time_array_copy;
}

static uint16_t* TIMEPOINT_now(void)
/****************************************************************************
*   Output   : Pointer to a time_array[]
*   Function : Return the time_array of the SYSTICK TIMEPOINT instance.
****************************************************************************/
{
	// disable interrupts if disabled
	bool disabled_irq = __disable_irq();

    // make copy of system time array
    static uint16_t* sys_time_array_copy;
    sys_time_array_copy = tp.get_array(tp_sys);

	// enable interrupts if they got disabled
	if (disabled_irq) { __enable_irq(); }

	// return static copy
	return sys_time_array_copy;
}

static void TIMEPOINT_copy(TIMEPOINT* des, TIMEPOINT * src)
/****************************************************************************
*   Input    : des, src = Pointers to TIMEPOINT instances.
*   Function : Copy time_array from 'src' TIMEPOINT to 'des' TIMEPOINT.
****************************************************************************/
{
	// copy values of src time_array to des time_array
	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		des->time_array[i] = src->time_array[i];
	}
}

static uint64_t TIMEPOINT_delta(TIMEPOINT* tp1, TIMEPOINT* tp2, TP_UNIT unit)
/****************************************************************************
*   Input    : tp1, tp2 = Pointers to TIMEPOINT instances.
               unit = TP_UNIT to be used.
*   Output   : Unsigned long long integer.
*   Function : Calculate absolute delta duration between two TIMEPOINTs
               given in unit defined by TP_UNIT.
****************************************************************************/
{
	uint64_t tp1_us = tp.get(tp1, us);
	uint64_t tp2_us = tp.get(tp2, us);

	uint64_t tp_diff_us = (tp1_us >= tp2_us) ? tp1_us - tp2_us : tp2_us - tp1_us;

	return tp.convert_us(tp_diff_us, unit);
}

static uint64_t TIMEPOINT_delta_now(TIMEPOINT* tp1, TP_UNIT unit)
/****************************************************************************
*   Input    : tp1 = Pointers to TIMEPOINT instances.
               unit = TP_UNIT to be used.
*   Output   : Unsigned long long integer.
*   Function : Calculate absolute delta duration between two a TIMEPOINT
               and the GLOBAL time in unit defined by TP_UNIT.
****************************************************************************/
{
	// create temporary TIMEPOINT and set it to current time
	TIMEPOINT tp_temp;
	tp.set(&tp_temp, tp.now());

	return tp.delta(&tp_temp, tp1, unit);
}

/****************************** End Of Module ******************************/

// static void TIMEPOINT_increment2(TIMEPOINT* this, uint16_t value, TP_UNIT unit)
// {
//
// 	uint16_t inc_value = 1;
//
// 	switch (unit) {
//
// 		case us:
//
// 			// break if no overflow
// 			if (!((this->us += ((unit == us) ? value : inc_value)) >= 1000)) { break; }
//
// 			// overflow; set current value to remainder and increment with quotient
// 			inc_value 	= this->us / 1000;
// 			this->us 	= this->us % 1000;
//
// 		case ms:
//
// 			// break if no overflow
// 			if (!((this->ms += ((unit == ms) ? value : inc_value)) >= 1000)) { break; }
//
// 			// overflow; set current value to remainder and increment with quotient
// 			inc_value 	= this->ms / 1000;
// 			this->ms 	= this->ms % 1000;
//
// 		case s:
//
// 			// break if no overflow
// 			if (!((this->s += ((unit == s) ? value : inc_value)) >= 60)) { break; }
//
// 			// overflow; set current value to remainder and increment with quotient
// 			inc_value 	= this->s / 60;
// 			this->s 	= this->s % 60;
//
// 		case m:
//
// 			// break if no overflow
// 			if (!((this->m += ((unit == m) ? value : inc_value)) >= 60)) { break; }
//
// 			// overflow; set current value to remainder and increment with quotient
// 			inc_value 	= this->m / 60;
// 			this->m 	= this->m % 60;
//
// 		case h:
//
// 			// break if no overflow
// 			if (!((this->h += ((unit == h) ? value : inc_value)) >= 24)) { break; }
//
// 			// overflow; set current value to remainder and increment with quotient
// 			inc_value 	= this->h / 24;
// 			this->h 	= this->h % 24;
//
// 		default:
//
// 			// reset struct
// 			tp.reset(this);
// 	}
// }
