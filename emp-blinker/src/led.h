/****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: led.h
*
* PROJECT....: emp-blinker

* DESCRIPTION: Interface to a LED struct.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 270220  PN    Module created.
*
****************************************************************************/

#pragma once

#include "../tm4c123gh6pm.h"
#include "emp_type.h"

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

typedef struct  LED LED;
typedef struct  RGB RGB;
typedef enum    LED_TYPE LED_TYPE;

#define LEDRED      1
#define LEDBLUE     2
#define LEDGREEN    3

/********************** External declaration of Variables ******************/

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

/*************************    Class Functions    ***************************/

extern const struct LED_CLASS
{
	LED*		(*new)(void);
	void   		(*del)(LED*);

	void   		(*set_callback)(LED*, void (*callback)(void));
	void   		(*set_state)(LED* this, BOOLEAN state);
	BOOLEAN 	(*get_state)(LED* this);
	void   		(*set_color)(LED* this, RGB);
	RGB    		(*get_color)(LED* this);

	void 		(*toggle)(LED* this);
	void 		(*invert_colors)(LED* this);
} led;

/*****************************    Constructs   *****************************/

struct RGB
{
	INT8U R : 1;
	INT8U G : 1;
	INT8U B : 1;
};

struct LED
{
	BOOLEAN		state;
	RGB 		color;
	void(*callback)(void);
};

/****************************** End Of Module ******************************/
