/****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: LED_controller.h
*
* PROJECT....: EMP

* DESCRIPTION: Interface to a LED struct.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*******************************************************************************/

#pragma once

/*****************************    Includes    *********************************/

#include <stdlib.h>

#include "../tm4c123gh6pm.h"
#include "emp_type.h"

#include "driver.h"
#include "tp.h"
#include "led.h"

/*****************************    Defines    **********************************/

typedef struct	LED_CONTROLLER			LED_CONTROLLER;
typedef struct	LED_CONTROLLER_CLASS	LED_CONTROLLER_CLASS;

typedef enum	LEDCTRL_MODE 			LEDCTRL_MODE;
typedef enum	LEDCTRL_DIR 			LEDCTRL_DIR;

/*************************** MAIN Constructs   ********************************/
extern const struct LED_CONTROLLER_CLASS
{
	LED_CONTROLLER*	(*new)(void);
	void 			(*del)(LED_CONTROLLER* this);

	void 			(*controller)(LED_CONTROLLER* this, LED* led_obj);
	void 			(*callback)(LED_CONTROLLER* this, INT32S duration_ms);
	void 			(*set_mode)(LED_CONTROLLER* this, LEDCTRL_MODE mode);
	LEDCTRL_MODE	(*get_mode)(LED_CONTROLLER* this);
	void 			(*set_reference_ms)(LED_CONTROLLER* this, INT32S reference_ms);
	INT32S			(*get_reference_ms)(LED_CONTROLLER* this);

} ledctrl;

/*************************    CONSTRUCTS         ******************************/

enum LEDCTRL_MODE
{
	MANUAL   =  0,
	AUTO     =  1
};

enum LEDCTRL_DIR
{
	UP      =   0,
	DOWN    =   1
};

struct LED_CONTROLLER
{
	LEDCTRL_MODE	mode;
	LEDCTRL_DIR		direction;

	INT32S			reference_ms;
	INT8U			led_color; // state of led {001} {010}... etc RGB

	TIMEPOINT*		tp_pressed;
};
/***************************** END MODULE    **********************************/
