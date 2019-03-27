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
#include "timepoint.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function declarations ***************************/

void 	TIMEPOINT_tick(TIMEPOINT * this);
void 	TIMEPOINT_set_callback(TIMEPOINT * this, void(*callback)());
void 	TIMEPOINT_set_systick(TIMEPOINT * this, INT64U duration, TP_UNIT unit);
void 	TIMEPOINT_increment(TIMEPOINT * this, INT64U value, TP_UNIT unit);
void 	TIMEPOINT_set_value(TIMEPOINT * this, INT64U time_array[TIME_ARRAY_SIZE]);
INT64U 	TIMEPOINT_get_value(TIMEPOINT * this, TP_UNIT unit);

void 	TIMEPOINT_copy(TIMEPOINT * des, TIMEPOINT * src);
INT64U 	TIMEPOINT_delta(TIMEPOINT * tp1, TIMEPOINT * tp2, TP_UNIT unit);

TIMEPOINT * new_TIMEPOINT(TP_TYPE type);
void 		del_TIMEPOINT(TIMEPOINT * this);

/*****************************   Functions   *******************************/

void TIMEPOINT_increment(TIMEPOINT * this, INT64U value, TP_UNIT unit)
/****************************************************************************
*   Input    : this = pointer to TIMEPOINT instance.
               value = ammount to increment (unit defined by index).
               unit = index of time_array (0 = ns, 1 = us, 2 = ms ...).
*   Function : Recurisve increment of TIMEPOINT, until no overflow.
****************************************************************************/
{
    // CPP Version
    // https://gist.github.com/martinandrovich/17615f282af047c953e0d776b218e603

	this->time_array[unit] += value;

	INT64U remainder = this->time_array[unit] % 1000;
	INT64U quotient	= this->time_array[unit] / 1000;

	if (remainder != this->time_array[unit]) // OR quotient > 1 ????
	{
		this->time_array[unit] = remainder;
        TIMEPOINT_increment(this, quotient, unit + 1);
	}
}

void TIMEPOINT_tick(TIMEPOINT * this)
/****************************************************************************
*   Input    : Pointer to TIMEPOINT instance.
*   Function : Increment TIMEPOINT with a systick unit.
****************************************************************************/
{
	// increment time_array
	TIMEPOINT_increment(this, this->systick_dur_ns, ns);

	// call callback if defined
	if (this->callback != NULL)
	{
		this->callback();
	}
}

void TIMEPOINT_set_callback(TIMEPOINT * this, void(*callback)())
/****************************************************************************
*   Input    : this: Pointer to TIMEPOINT instance.
			   callback: Pointer to void function.
*   Function : Set the callback of a TIMEPOINT instance.
****************************************************************************/
{
	this->callback = callback;
}

void TIMEPOINT_set_systick(TIMEPOINT * this, INT64U duration, TP_UNIT unit)
/****************************************************************************
*   Input    : this: Pointer to TIMEPOINT instance.
			   systick_dur_ns: Duration of systick tick in ns.
*   Function : Set the systick_dur_ns of a TIMEPOINT instance.
****************************************************************************/
{
	this->systick_dur_ns = duration * pow(10, 3*unit);
}

void TIMEPOINT_set_value(TIMEPOINT * this, INT64U time_array[TIME_ARRAY_SIZE])
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

INT64U TIMEPOINT_get_value(TIMEPOINT * this, TP_UNIT unit)
/****************************************************************************
*   Input    : this = Pointer to TIMEPOINT instance.
			   unit = desired TP_UNIT.
*   Output   : Unsigned long long integer.
*   Function : Return the value of a TIMEPOINT instance given in
			   unit defined by TP_UNIT.
****************************************************************************/
{
	INT64U sum_ns = this->time_array[0] +
					this->time_array[1] * pow(10, 3) +
					this->time_array[2] * pow(10, 6) +
					this->time_array[3] * pow(10, 9);

	return (sum_ns * pow(10, -3 * unit));
}

void TIMEPOINT_copy(TIMEPOINT * des, TIMEPOINT * src)
/****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	for (int i = 0; i < TIME_ARRAY_SIZE; i++)
	{
		des->time_array[i] = src->time_array[i];
	}
}

INT64U TIMEPOINT_delta(TIMEPOINT * tp1, TIMEPOINT * tp2, TP_UNIT unit)
/****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
	INT64U tp1_ns = tp1->get_value(tp1, ns);
	INT64U tp2_ns = tp2->get_value(tp2, ns);
	INT64U tp_diff = (tp1_ns >= tp2_ns) ? tp1_ns - tp2_ns : tp2_ns - tp1_ns;
	return (tp_diff * pow(10, -3 * unit));
}

/***********************   Constructive Functions   ************************/

TIMEPOINT * new_TIMEPOINT(TP_TYPE type)
/****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    // allocate object
    TIMEPOINT * tp = malloc(sizeof(TIMEPOINT));

    // initialize time_array
    //for (int i = 0; i < sizeof(tp->time_array)/sizeof(INT64U); i++)
    for (int i = 0; i < TIME_ARRAY_SIZE; i++)
    {
        tp->time_array[i] = 0;
    }

	// initialize callback to nullptr
	tp->callback = NULL;

    // set systick duration according to TIMEPOINT type  (default = 100ns)
    tp->systick_dur_ns  = (type == SYSTEM) ? 100 : 0;

	// set timepoint type
	tp->type			= type;

    // link function pointers according to TIMEPOINT type
    tp->tick            = (type == SYSTEM) ? &TIMEPOINT_tick 			: NULL;
	tp->set_callback	= (type == SYSTEM) ? &TIMEPOINT_set_callback 	: NULL;
	tp->set_systick		= (type == SYSTEM) ? &TIMEPOINT_set_systick 	: NULL;
	tp->get_value		= &TIMEPOINT_get_value;
	tp->set_value		= &TIMEPOINT_set_value;

	// maybe better to set all funcptr to their according func, and perform
	// error handling in the function, according to TIMEPOINT type?

	// return pointer to instance
    return tp;
}

void del_TIMEPOINT(TIMEPOINT * this)
/****************************************************************************
*   Function : See module specification (.h-file).
*****************************************************************************/
{
    free(this);
}
