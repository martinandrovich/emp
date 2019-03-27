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

#define NORWEGIAN_YELLOW_DUR_MS		1000

#define NORMAL_RED_DUR_MS			2000
#define NORMAL_YELLOW_DUR_MS		1000
#define NORMAL_GREEN_DUR_MS 		3000
#define NORMAL_RED_YELLOW_DUR_MS	1000

/************************  Function declarations ***************************/

static LED_CONTROLLER*	LED_CONTROLLER_new(void);
static void 			LED_CONTROLLER_del(LED_CONTROLLER* this);

static void 			LED_CONTROLLER_operate(LED_CONTROLLER* this, LED* led_obj);
static void 			LED_CONTROLLER_set_mode(LED_CONTROLLER* this, LEDCTRL_MODE mode);

static volatile void	_LED_CONTROLLER_mode_normal(LED_CONTROLLER* this, LED* led_obj);
static volatile void 	_LED_CONTROLLER_mode_norwegian(LED_CONTROLLER* this, LED* led_obj);
static volatile void 	_LED_CONTROLLER_mode_emergency(LED_CONTROLLER* this, LED* led_obj);

/****************************   Class Struct   *****************************/

const struct LED_CONTROLLER_CLASS ledctrl =
{
	.new                =  &LED_CONTROLLER_new,
	.del                =  &LED_CONTROLLER_del,

	.operate 	        =  &LED_CONTROLLER_operate,
	.set_mode           =  &LED_CONTROLLER_set_mode
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

	this->mode			= NORMAL;
	this->tp_timer		= tp.new();

	return this;
}

static void LED_CONTROLLER_del(LED_CONTROLLER* this)
/****************************************************************************
*   Input    : this object
*   Function : deletes object
****************************************************************************/
{
	tp.del(this->tp_timer);
	free(this);
}

/*****************************   Functions   *******************************/

static void LED_CONTROLLER_operate(LED_CONTROLLER* this, LED* led_obj)
/****************************************************************************
*   Input    : Duration
*   Function : LED controller, chooses state
****************************************************************************/
{
	switch (this->mode)
	{
		case NORMAL:

			_LED_CONTROLLER_mode_normal(this, led_obj);
			break;

		case NORWEGIAN:

			_LED_CONTROLLER_mode_norwegian(this, led_obj);
			break;

		case EMERGENCY:

			_LED_CONTROLLER_mode_emergency(this, led_obj);
			break;

		default:

			this->mode = NORMAL;
			break;
	}
}


static void _LED_CONTROLLER_mode_normal(LED_CONTROLLER* this, LED* led_obj)
{
	static NORMAL_STATES state = RED_ON;

	switch (state)
	{
		case RED_ON:

			if(tp.delta_now(this->tp_timer,ms) < NORMAL_RED_DUR_MS)
			{
				led.set_color(led_obj, (RGB){1, 0, 0});
			}
			else
			{
				state = RED_YELLOW_ON;
				tp.set(this->tp_timer, tp.now());
			}

			break;

		case RED_YELLOW_ON:

			if(tp.delta_now(this->tp_timer,ms) < NORMAL_RED_YELLOW_DUR_MS)
			{
				led.set_color(led_obj, (RGB){1, 0, 1});
			}
			else
			{
				state = GREEN_ON;
				tp.set(this->tp_timer, tp.now());
			}

			break;

		case GREEN_ON:

			if(tp.delta_now(this->tp_timer,ms) < NORMAL_GREEN_DUR_MS)
			{
				led.set_color(led_obj, (RGB){0, 1, 0});
			}
			else
			{
				state = YELLOW_ON;
				tp.set(this->tp_timer, tp.now());
			}

			break;

		case YELLOW_ON:

			if(tp.delta_now(this->tp_timer,ms) < NORMAL_YELLOW_DUR_MS)
			{
				led.set_color(led_obj, (RGB){0, 0, 1});
			}
			else
			{
				state = RED_ON;
				tp.set(this->tp_timer, tp.now());
			}

			break;
	}
}

static void _LED_CONTROLLER_mode_norwegian(LED_CONTROLLER* this, LED* led_obj)
{
	// reversed RBG
	led.set_color(led_obj, (RGB){0, 0, 1});

	if (tp.delta_now(this->tp_timer, ms) >= NORWEGIAN_YELLOW_DUR_MS)
	{
		led.toggle(led_obj);
		tp.set(this->tp_timer, tp.now());
	}
}

static void _LED_CONTROLLER_mode_emergency(LED_CONTROLLER* this, LED* led_obj)
{
 	led.set_color(led_obj, (RGB){1, 0, 0});
}

static void LED_CONTROLLER_set_mode(LED_CONTROLLER* this, LEDCTRL_MODE mode)
/****************************************************************************
*   Input    : this and the mode u want to set
*   Function : set mode in this object
****************************************************************************/
{
	this->mode = mode;
	tp.set(this->tp_timer, tp.now());
}

/****************************** End Of Module ******************************/
