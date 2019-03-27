/****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: LED_controller.c
*
* PROJECT....: emp-blinker

* DESCRIPTION: Interface to a LED struct.
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  DT    Module created.
*
****************************************************************************/

/***************************** Include files *******************************/

#include "ledctrl.h"

/*****************************    Defines    *******************************/

#define MAXLED      7
#define MINLED      0

/********************** External declaration of Variables ******************/

extern TIMEPOINT* tp_global;

/*****************************   Constants   *******************************/

#define BLINK_DUR_MS 200

/************************  Function declarations ***************************/

static LED_CONTROLLER*	LED_CONTROLLER_new(void);
static void 			LED_CONTROLLER_del(LED_CONTROLLER* this);

static void 			LED_CONTROLLER_callback(LED_CONTROLLER* this, INT32S duration_ms);
static void 			LED_CONTROLLER_controller(LED_CONTROLLER* this, LED* led_obj);
static void 			LED_CONTROLLER_set_mode(LED_CONTROLLER* this, LEDCTRL_MODE mode);
static LEDCTRL_MODE 	LED_CONTROLLER_get_mode(LED_CONTROLLER* this);
static void 			LED_CONTROLLER_set_reference_ms(LED_CONTROLLER* this, INT32S reference_ms);
static INT32S 			LED_CONTROLLER_get_reference_ms(LED_CONTROLLER* this);

static void 			_LED_CONTROLLER_circulation(LED_CONTROLLER* this);

/****************************   Class Struct   *****************************/

const struct LED_CONTROLLER_CLASS ledctrl =
{
	.new                =  &LED_CONTROLLER_new,
	.del                =  &LED_CONTROLLER_del,

	.controller         =  &LED_CONTROLLER_controller,
	.callback           =  &LED_CONTROLLER_callback,
	.set_mode           =  &LED_CONTROLLER_set_mode,
	.get_mode           =  &LED_CONTROLLER_get_mode,
	.set_reference_ms   =  &LED_CONTROLLER_set_reference_ms,
	.get_reference_ms   =  &LED_CONTROLLER_get_reference_ms
};

/***********************   Constructive Functions   ************************/

static LED_CONTROLLER* LED_CONTROLLER_new(void)
/****************************************************************************
*   Input    : no input, constructor
*   Output   : object pointer
*   Function : creates object
****************************************************************************/
{
	// allocate memory
	LED_CONTROLLER* this = malloc(sizeof(LED_CONTROLLER));

	this->mode			= MANUAL;
	this->direction		= UP;

	this->reference_ms	= 2000;
	this->led_color		= 0;

	this->tp_pressed	= tp.new(NORMAL);

	return this;
}

static void LED_CONTROLLER_del(LED_CONTROLLER* this)
/****************************************************************************
*   Input    : this object
*   Function : deletes object
****************************************************************************/
{
	free(this);
}

/*****************************   Functions   *******************************/

static void LED_CONTROLLER_controller(LED_CONTROLLER* this, LED* led_obj)
/****************************************************************************
*   Input    : Duration
*   Function : LED controller, chooses state
****************************************************************************/
{
	if(this->mode == AUTO)
	{
		__disable_irq();

			if(tp.delta(this->tp_pressed, tp_global, ms) >= BLINK_DUR_MS)
			{
				_LED_CONTROLLER_circulation(this);
				tp.copy(this->tp_pressed, tp_global);
			};

		__enable_irq();
	}

	// update LED color & refresh state
	RGB new_rgb = (RGB)
	{
		(((this->led_color) >> 2) & 1),
		(((this->led_color) >> 0) & 1),
		(((this->led_color) >> 1) & 1)
	};

	led.set_color(led_obj, new_rgb);
}

static void LED_CONTROLLER_callback(LED_CONTROLLER* this, INT32S duration_ms)
/****************************************************************************
*   Input    : Duration
*   Function : LED controller, chooses state
****************************************************************************/
{
	switch (duration_ms)
	{
		case -1:
			this->direction = (this->direction == UP) ? DOWN : UP;
			break;
		// ^-- change mode if double click -- toggle -- does not change anything
		default:
			this->mode = (this->reference_ms > duration_ms) ? MANUAL : AUTO;
			// if normal mode change
			if(this->mode == MANUAL)
			{
				_LED_CONTROLLER_circulation(this);
			}
			else
			{
				__disable_irq();
				tp.copy(this->tp_pressed, tp_global);
				__enable_irq();
			}
			break;
		// -- change mode if AUTO otherwise change to NORMAL and calculate new led.
	};
}

static void LED_CONTROLLER_set_mode(LED_CONTROLLER* this, LEDCTRL_MODE mode)
/****************************************************************************
*   Input    : this and the mode u want to set
*   Function : set mode in this object
****************************************************************************/
{
	this->mode = mode;
}

static LEDCTRL_MODE LED_CONTROLLER_get_mode(LED_CONTROLLER* this)
/****************************************************************************
*   Input    : this object
*   Function : get this object mode
****************************************************************************/
{
	return this->mode;
}

static void LED_CONTROLLER_set_reference_ms(LED_CONTROLLER* this, INT32S reference_ms)
/****************************************************************************
*   Input    : set reference_ms
*   Function : set reference in this object
****************************************************************************/
{
	this->reference_ms = reference_ms;
}

static INT32S LED_CONTROLLER_get_reference_ms(LED_CONTROLLER* this)
/****************************************************************************
*   Input    : get reference_ms
*   Function : set reference in this object
****************************************************************************/
{
	return this->reference_ms;
}

static void _LED_CONTROLLER_circulation(LED_CONTROLLER* this)
/****************************************************************************
*   Input    : this object
*   Function : update led_color
****************************************************************************/
{
	INT8U alias_state = this->led_color;

	if(this->direction == UP)
	{
		alias_state += 1;
		this->led_color = (this->led_color == MAXLED) ? MINLED : alias_state;
	}
	else
	{
		alias_state -= 1;
		this->led_color = (this->led_color == MINLED) ? MAXLED : alias_state;
	}
}

/****************************** End Of Module ******************************/
