/****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: button.c
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
* 190220  DT    Module created.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdlib.h>

#include "../tm4c123gh6pm.h"
#include "driver.h"

#include "emp_type.h"
#include "tp.h"

/*****************************    Defines    *******************************/

typedef struct BUTTON BUTTON;
typedef enum   BTN_NAME BTN_NAME;
typedef enum   BTN_STATE BTN_STATE;
typedef enum   BTN_CB_TYPE BTN_CB_TYPE;

/*****************************   Constants   *******************************/

/********************** External declaration of Variables ******************/

extern TIMEPOINT* tp_global;

/*****************************   Functions   *******************************/

/*************************    Class Functions    ***************************/

extern struct BUTTON_CLASS
{
	// Constructor & Destructor
	BUTTON*	(*const new)(BTN_NAME SW);
	void    (*const del)(BUTTON* this);

	// Methods
	void 	(*const controller)(BUTTON* this);
	void 	(*const set_callback)(BUTTON* this, BTN_CB_TYPE cb_type , void(*callback)(void));

} btn;

/*****************************    Constructs   *****************************/

enum BTN_STATE
{
   KEY_UP        =  0,
   DEBOUNCING    =  1,
   KEY_DOWN      =  2,
   COOLDOWN		 =  3
};

enum BTN_NAME
{
   SW2           =  0,
   SW1           =  4
};

enum BTN_CB_TYPE
{
   SINGLE_PRESS		=  0,
   DOUBLE_PRESS		=  1,
   LONG_PRESS		=  2
};

struct BUTTON
{
	BTN_NAME		button;
	BTN_STATE 		state;

	TIMEPOINT*		tp_pressed;
	TIMEPOINT*		tp_released;
	TIMEPOINT*		tp_db;
	TIMEPOINT*		tp_pending;

	INT32S			duration_ms;
	INT32S			db_delta_ms;

	BOOLEAN			pending_callback;

	//void 			(*callback)(INT32S duration_ms);

	void 			(*callback_single)(void); // single press
	void 			(*callback_double)(void); // double press
	void 			(*callback_long)(void); // long press
};

/****************************** End Of Module ******************************/
