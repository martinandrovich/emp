/****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: timepoint.c
*
* PROJECT....: emp-blinker
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 190220  MA    Module created.
*
****************************************************************************/

/***************************** Include files *******************************/

#include <math.h>
#include <assert.h>

#include "tp.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function declarations ***************************/

static TIMEPOINT*	TIMEPOINT_new(TP_TYPE type);
static void 		TIMEPOINT_del(TIMEPOINT* this);

static void 		_TIMEPOINT_increment(TIMEPOINT* this, INT64U value, TP_UNIT unit);
static void			_TIMEPOINT_assert_irq(TIMEPOINT* this);

static void 		TIMEPOINT_tick(TIMEPOINT* this);
static void 		TIMEPOINT_set_callback(TIMEPOINT* this, void(*callback)());
static void 		TIMEPOINT_set_systick(TIMEPOINT* this, INT64U duration, TP_UNIT unit);
static void 		TIMEPOINT_set_value(TIMEPOINT* this, INT64U time_array[TIME_ARRAY_SIZE]);
static INT64U 		TIMEPOINT_get_value(TIMEPOINT* this, TP_UNIT unit);
static void 		TIMEPOINT_copy(TIMEPOINT* des, TIMEPOINT* src);
static INT64U 		TIMEPOINT_delta(TIMEPOINT* tp1, TIMEPOINT* tp2, TP_UNIT unit);

/****************************   Class Struct   *****************************/

const struct TIMEPOINT_CLASS tp =
{
	.new			= &TIMEPOINT_new,
	.del			= &TIMEPOINT_del,

	.tick			= &TIMEPOINT_tick,

	.set_callback	= &TIMEPOINT_set_callback,
	.set_systick	= &TIMEPOINT_set_systick,

	.set_value		= &TIMEPOINT_set_value,
	.get_value		= &TIMEPOINT_get_value,

	.copy			= &TIMEPOINT_copy,
	.delta			= &TIMEPOINT_delta
};

/***********************   Constructive Functions   ************************/

static TIMEPOINT* TIMEPOINT_new(TP_TYPE type)
{
	// allocate memory
	TIMEPOINT* this = malloc(sizeof(TIMEPOINT));

	// initialize time_array
	//for (int i = 0; i < sizeof(tp->time_array)/sizeof(INT64U); i++)
	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		this->time_array[i] = 0;
	}

	// initialize callback to nullptr
	this->callback = NULL;

	// set systick duration according to TIMEPOINT type  (default = 100ns)
	this->systick_dur_ns = (type == SYSTEM) ? 100 : 0;

	// set timepoint type
	this->type = type;

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

static void _TIMEPOINT_increment(TIMEPOINT* this, INT64U value, TP_UNIT unit)
/****************************************************************************
*   Input    : this = pointer to TIMEPOINT instance.
			   value = ammount to increment (unit defined by index).
			   unit = index of time_array (0 = ns, 1 = us, 2 = ms ...).
*   Function : Recurisve increment of TIMEPOINT, until no overflow.
****************************************************************************/
{
	this->time_array[unit] += value;

	INT64U remainder = this->time_array[unit] % 1000;
	INT64U quotient = this->time_array[unit] / 1000;

	if (remainder != this->time_array[unit]) // OR quotient > 1 ????
	{
		this->time_array[unit] = remainder;
		_TIMEPOINT_increment(this, quotient, unit + 1);
	}
}

static void _TIMEPOINT_assert_irq(TIMEPOINT* this)
/****************************************************************************
*   Input    : this = pointer to the TIMEPOINT to assert
*   Function : Assert whether TIMEPOINT is NORMAL or interrupts disabled
****************************************************************************/
{
	assert(this->type == NORMAL || get_irq() == 0);
}

static void TIMEPOINT_tick(TIMEPOINT* this)
/****************************************************************************
*   Input    : Pointer to TIMEPOINT instance.
*   Function : Increment TIMEPOINT with a systick unit.
****************************************************************************/
{
	// increment time_array
	_TIMEPOINT_increment(this, this->systick_dur_ns, ns);

	// call callback if defined
	if (this->callback != NULL && this->type == SYSTEM)
	{
		this->callback();
	}
}

static void TIMEPOINT_set_callback(TIMEPOINT* this, void(*callback)())
/****************************************************************************
*   Input    : this: Pointer to TIMEPOINT instance.
			   callback: Pointer to void function.
*   Function : Set the callback of a TIMEPOINT instance.
****************************************************************************/
{
	this->callback = callback;
}

static void TIMEPOINT_set_systick(TIMEPOINT* this, INT64U duration, TP_UNIT unit)
/****************************************************************************
*   Input    : this: Pointer to TIMEPOINT instance.
			   systick_dur_ns: Duration of systick tick in ns.
*   Function : Set the systick_dur_ns of a TIMEPOINT instance.
****************************************************************************/
{
	this->systick_dur_ns = duration * pow(10, 3 * unit);
}

static void TIMEPOINT_set_value(TIMEPOINT* this, INT64U time_array[TIME_ARRAY_SIZE])
/****************************************************************************
*   Input    : this: Pointer to TIMEPOINT instance.
			   time_array[]: Array with new values.
*   Function : Set the time_array values of a TIMEPOINT instance.
****************************************************************************/
{
	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		this->time_array[i] = time_array[i];
	}
}

static INT64U TIMEPOINT_get_value(TIMEPOINT* this, TP_UNIT unit)
/****************************************************************************
*   Input    : this = Pointer to TIMEPOINT instance.
			   unit = desired TP_UNIT.
*   Output   : Unsigned long long integer.
*   Function : Return the value of a TIMEPOINT instance given in
			   unit defined by TP_UNIT.
****************************************************************************/
{
	// assert mutual exclusion
	_TIMEPOINT_assert_irq(this);

	INT64U sum_ns = this->time_array[0] +
		this->time_array[1] * pow(10, 3) +
		this->time_array[2] * pow(10, 6) +
		this->time_array[3] * pow(10, 9);

	return (sum_ns * pow(10, -3 * unit));
}

/***************************   Class Functions   ***************************/

static void TIMEPOINT_copy(TIMEPOINT* des, TIMEPOINT * src)
/****************************************************************************
*   Input    : des, src = Pointers to TIMEPOINT instances.
*   Function : Copy time_array from 'src' TIMEPOINT to 'des' TIMEPOINT.
****************************************************************************/
{
	// assert mutual exclusion
	_TIMEPOINT_assert_irq(des);
	_TIMEPOINT_assert_irq(src);

	// copy values of src time_array to des time_array
	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		des->time_array[i] = src->time_array[i];
	}
}

static INT64U TIMEPOINT_delta(TIMEPOINT* tp1, TIMEPOINT* tp2, TP_UNIT unit)
/****************************************************************************
*   Input    : tp1, tp2 = Pointers to TIMEPOINT instances.
               unit = TP_UNIT to be used.
*   Output   : Unsigned long long integer.
*   Function : Calculate absolute delta duration between two TIMEPOINTs
               given in unit defined by TP_UNIT.
****************************************************************************/
{
	// assert mutual exclusion
	_TIMEPOINT_assert_irq(tp1);
	_TIMEPOINT_assert_irq(tp2);

	INT64U tp1_ns = tp.get_value(tp1, ns);
	INT64U tp2_ns = tp.get_value(tp2, ns);

	INT64U tp_diff = (tp1_ns >= tp2_ns) ? tp1_ns - tp2_ns : tp2_ns - tp1_ns;

	return (tp_diff * pow(10, -3 * unit));
}

/****************************** End Of Module ******************************/
