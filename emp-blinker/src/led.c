/****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: led.c
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
* 270220  PN    Module created.
*
****************************************************************************/

/***************************** Include files *******************************/

#include <malloc.h>
#include "led.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*************************  Function declaration ***************************/

static LED* 	LED_new();
static void 	LED_del(LED* this);

static void 	_LED_init(void);

static BOOLEAN 	LED_get_state(LED* this);
static void 	LED_set_state(LED* this, BOOLEAN state);
static RGB 		LED_get_color(LED* this);
static void 	LED_set_color(LED* this, RGB Value);
static void 	LED_set_callback(LED* this, void(*callback)(void));
static void 	LED_toggle_state(LED* this);
static void 	LED_invert_colors(LED* this);

/****************************   Class Struct   *****************************/

const struct LED_CLASS led =
{
	.new 			= &LED_new,
	.del 			= &LED_del,

	.set_callback 	= &LED_set_callback,
	.set_state 		= &LED_set_state,
	.get_state 		= &LED_get_state,
	.set_color 		= &LED_set_color,
	.get_color 		= &LED_get_color,

	.toggle 		= &LED_toggle_state,
	.invert_colors 	= &LED_invert_colors
};

/***********************   Constructive Functions   ************************/

static LED* LED_new(void)
/****************************************************************************
*   Output   : returns a pointer to an instance on the heap
*   Function : new
****************************************************************************/
{
	LED* this = malloc(sizeof(LED));

	this->state 	= 0;
	this->color 	= (RGB){ 0, 0, 0 };
	this->callback 	= NULL;

	_LED_init();

	return this;
}

static void LED_del(LED * this)
/****************************************************************************
*   Input    : instance
*   Output   : void
*   Function : Free the memory of the instance.
****************************************************************************/
{
	free(this);
}

/*****************************   Functions   *******************************/

static void _LED_init(void)
{
	// Enable GPIO register F
	SYSCTL_RCGCGPIO_R 	|= SYSCTL_RCGC2_GPIOF;

	// Stall
	asm volatile
	(
		"nop;"
		"nop;"
		"nop;"
	);

	// PORTF Direction (Switches input & LED output)
	GPIO_PORTF_DIR_R 	|= (1 << LEDRED) | (1 << LEDGREEN) | (1 << LEDBLUE);

	// Enable Ports
	GPIO_PORTF_DEN_R 	|= (1 << LEDRED) | (1 << LEDGREEN) | (1 << LEDBLUE);
}

static void LED_set_callback(LED* this, void(*callback)(void))
/****************************************************************************
*   Input    : Instance of LED, the desired function that should be called
			   whenever, an event occurs.
*   Function : set_callback
****************************************************************************/
{
	this->callback = callback;
}

static void LED_set_state(LED* this, BOOLEAN state)
/****************************************************************************
*   Input    :  set the state for the given instance of LED
*   Output   :  void
*   Function : set_state
****************************************************************************/
{
	this->state = state;
	GPIO_PORTF_DATA_R &= (0 << LEDRED) | (0 << LEDBLUE) | (0 << LEDGREEN);

	if(this->state == 1)
	{
		GPIO_PORTF_DATA_R 	|= 	(this->color.R 	<< LEDRED) 	 |
								(this->color.G 	<< LEDGREEN) |
								(this->color.B) << LEDBLUE;
	}
}

static BOOLEAN LED_get_state(LED* this)
/****************************************************************************
*   Input    :  an instance
*   Output   :  returns the state of the LED ( on/off)
*   Function : get_state
****************************************************************************/
{
	return this->state;
}

static void LED_set_color(LED* this, RGB value)
/****************************************************************************
*   Input    : A 3 bit value.
*   Output   : updates the color;
*   Function : set_color
****************************************************************************/
{
	this->color = value;
	LED_set_state(this, this->state);
}

static RGB LED_get_color(LED* this)
/****************************************************************************
*   Input    : instance of LED
*   Output   : returns the state of the given LED
*   Function : get_color
****************************************************************************/
{
	return this->color;
}

static void LED_toggle_state(LED* this)
{
	LED_set_state(this, !this->state);
}

static void LED_invert_colors(LED* this)
{
	// cast struct to char pointer, then de-ref and xor to invert
	*(char*)(&this->color) ^= 0xFF;
	LED_set_state(this, this->state);
}

/****************************** End Of Module ******************************/
