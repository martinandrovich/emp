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

static TIMEPOINT* 		tp_sys;
static uint32_t			systick_dur_ns;

/************************  Function declarations ***************************/

static TIMEPOINT*	TIMEPOINT_new(void);
static void 		TIMEPOINT_del(TIMEPOINT* this);

static void 		TIMEPOINT_systick(void);
static void 		TIMEPOINT_init_systick(uint32_t duration, TP_UNIT unit);

static void 		TIMEPOINT_increment(TIMEPOINT* this, uint64_t value, TP_UNIT unit);
static void 		TIMEPOINT_set(TIMEPOINT* this, uint64_t time_array[TIME_ARRAY_SIZE]);
static uint64_t		TIMEPOINT_get(TIMEPOINT* this, TP_UNIT unit);
static uint64_t*	TIMEPOINT_get_array(TIMEPOINT* this);
static uint64_t*	TIMEPOINT_now(void);
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

	.inc			= &TIMEPOINT_increment,
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

	// return pointer to instance
	return this;
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
	tp.inc(tp_sys, systick_dur_ns, ns);
}

static void TIMEPOINT_init_systick(uint32_t duration, TP_UNIT unit)
/****************************************************************************
*   Input    : Pointer to TIMEPOINT instance.
*   Function : Increment TIMEPOINT with a systick unit.
****************************************************************************/
{
	tp_sys 			= tp.new();
	systick_dur_ns	= duration * pow(10, 3 * unit);
}

static void TIMEPOINT_increment(TIMEPOINT* this, uint64_t value, TP_UNIT unit)
/****************************************************************************
*   Input    : this = pointer to TIMEPOINT instance.
			   value = ammount to increment (unit defined by index).
			   unit = index of time_array (0 = ns, 1 = us, 2 = ms ...).
*   Function : Recurisve increment of TIMEPOINT, until no overflow.
****************************************************************************/
{
	this->time_array[unit] += value;

	uint64_t remainder = this->time_array[unit] % 1000;
	uint64_t quotient = this->time_array[unit] / 1000;

	if (remainder != this->time_array[unit]) // OR quotient > 1 ????
	{
		this->time_array[unit] = remainder;
		TIMEPOINT_increment(this, quotient, unit + 1);
	}
}

static void TIMEPOINT_set(TIMEPOINT* this, uint64_t time_array[TIME_ARRAY_SIZE])
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
	uint64_t sum_ns = this->time_array[0] +
		this->time_array[1] * pow(10, 3) +
		this->time_array[2] * pow(10, 6) +
		this->time_array[3] * pow(10, 9);

	return (sum_ns * pow(10, -3 * unit));
}

static uint64_t* TIMEPOINT_get_array(TIMEPOINT* this)
/****************************************************************************
*   Input    : this = Pointer to TIMEPOINT instance.
			   unit = desired TP_UNIT.
*   Output   : Pointer to a time_array[]
*   Function : Return the time_array of a TIMEPOINT instance.
****************************************************************************/
{
	static uint64_t time_array_copy[TIME_ARRAY_SIZE];

	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		time_array_copy[i] = this->time_array[i];
	}

	return time_array_copy;
}

static uint64_t* TIMEPOINT_now(void)
/****************************************************************************
*   Output   : Pointer to a time_array[]
*   Function : Return the time_array of the SYSTICK TIMEPOINT instance.
****************************************************************************/
{
	// disable interrupts if disabled
	bool disabled_irq = __disable_irq();

    // make copy of system time array
    static uint64_t* sys_time_array_copy;
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
	uint64_t tp1_ns = tp.get(tp1, ns);
	uint64_t tp2_ns = tp.get(tp2, ns);

	uint64_t tp_diff = (tp1_ns >= tp2_ns) ? tp1_ns - tp2_ns : tp2_ns - tp1_ns;

	return (tp_diff * pow(10, -3 * unit));
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
