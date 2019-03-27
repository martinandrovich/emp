/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	exm.c
* MODULENAME.:	EXAMPLE
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "exm.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

static int32_t		_global_var = 0;

/************************  Function Declarations ***************************/

static EXAMPLE*		EXAMPLE_new(EXM_TYPE type, int32_t init_val);
static void 		EXAMPLE_del(EXAMPLE* this);

static void 		EXAMPLE_some_func(EXAMPLE* this);

static int32_t 		_EXAMPLE_private_func(void);

/****************************   Class Struct   *****************************/

const struct EXAMPLE_CLASS exm =
{
	.new			= &EXAMPLE_new,
	.del			= &EXAMPLE_del,

	.some_func		= &EXAMPLE_some_func
};

/***********************   Constructive Functions   ************************/

static EXAMPLE* EXAMPLE_new(EXM_TYPE type, int32_t init_val)
/****************************************************************************
*   Input    : type = desired EXM_TYPE for the instance.
			 : init_val = desired value for `var`.
*   Output   : Pointer to a new EXAMPLE instance.
*   Function : Constructor of a EXAMPLE instance.
****************************************************************************/
{
	// allocate memory
	EXAMPLE* this = malloc(sizeof(EXAMPLE));

	// initialize variables
	this->var 		= init_val;
	this->_secret	= 0;
	this->is_set 	= false;

	// initialize array
	for (int i = 0; i < EXM_ARRAY_SIZE; i++)
	{
		this->array[i] = 0;
	}

	// return pointer to instance
	return this;
}

static void EXAMPLE_del(EXAMPLE* this)
/****************************************************************************
*   Input    : this = pointer to a EXAMPLE instance.
*   Function : Destructor of a EXAMPLE instance.
****************************************************************************/
{
	free(this);
}

/*****************************   Functions   *******************************/

static void EXAMPLE_some_func(EXAMPLE* this)
/****************************************************************************
*   Input    : this = pointer to a EXAMPLE instance.
*   Function : Sets `is_set` of an EXAMPLE instance to false, such that
			   something interesting may happen. Maybe.
****************************************************************************/
{
	this->is_set = false;
}

static int32_t _EXAMPLE_private_func(void)
/****************************************************************************
*   Function : Some arithmetic expression.
****************************************************************************/
{
	int32_t temp = 20 * ((7 + 4) / (-5));
	return temp;
}

/****************************** End Of Module ******************************/
