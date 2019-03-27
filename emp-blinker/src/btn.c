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

/***************************** Include files *******************************/

#include <math.h>

#include "driver.h"
#include "btn.h"

/*****************************    Defines    *******************************/

#define DEBOUNCE_DUR_MS		10
#define COOLDOWN_DUR_MS		10
#define DOUBLEPRESS_DUR_MS	300
#define LONGPRESS_DUR_MS	2000

#define BTN_BIT this->button

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*************************  Function declaration ***************************/

static BUTTON*  BUTTON_new(BTN_NAME SW);
static void		BUTTON_del(BUTTON* this);

static void		BUTTON_controller(BUTTON* this);
static void		BUTTON_set_callback(BUTTON* this, void(*callback)(INT32S duration_ms));

static void		_BUTTON_is_key_down(BUTTON* this);
static void		_BUTTON_debounce_button(BUTTON* this);
static void		_BUTTON_key_press(BUTTON* this);
static void 	_BUTTON_key_cooldown(BUTTON* this);
static void		_BUTTON_init_hardware(BUTTON* this);

/****************************   Class Struct   *****************************/

struct BUTTON_CLASS btn =
{
	.new                            = &BUTTON_new,
	.del                            = &BUTTON_del,

	.controller                     = &BUTTON_controller,
	.set_callback                   = &BUTTON_set_callback
};

/***********************   Constructive Functions   ************************/

static BUTTON* BUTTON_new(BTN_NAME SW)
/****************************************************************************
*   Output   : Object
*   Function : Constructor for Button.
****************************************************************************/
{
	BUTTON* this				= malloc(sizeof(BUTTON));

	this->button				= SW;
	this->state			      	= KEY_UP;

	this->tp_pressed			= NULL;
	this->tp_released		 	= NULL;
	this->tp_db					= NULL;
	this->tp_pending			= NULL;

	this->duration_ms			= 0;
	this->db_delta_ms			= 0;

	this->pending_callback	 	= FALSE;

	this->callback				= NULL;

	_BUTTON_init_hardware(this);

	return this;
}

static void BUTTON_del(BUTTON * this)
/****************************************************************************
*   Input    : Pointer to Button object
*   Function : Destructor for object
****************************************************************************/
{
	tp.del(this->tp_pressed);
	tp.del(this->tp_released);
	tp.del(this->tp_db);
	tp.del(this->tp_pending);

	free(this);
}

/*****************************   Functions   *******************************/

static void BUTTON_controller(BUTTON* this)
/****************************************************************************
*   Input    : Object this pointer, this is a method
*   Function : Finite State Machine determines, which state for button to be in
****************************************************************************/
{
	// state machine
	switch (this->state)
	{
		case KEY_UP:
			_BUTTON_is_key_down(this);
			break;

		case DEBOUNCING:
			_BUTTON_debounce_button(this);
			break;

		case KEY_DOWN:
			_BUTTON_key_press(this);
			break;

		case COOLDOWN:
			_BUTTON_key_cooldown(this);
			break;

		default:
			this->state = KEY_UP;
			break;
	}

	// check pending callback
	if (this->pending_callback)
	{
		// get duration since pending callback
		__disable_irq();
		INT32U delta_pending = tp.delta(this->tp_pending, tp_global, ms);
		__enable_irq();

		// check if callback is to be called
		if (delta_pending >= abs(DOUBLEPRESS_DUR_MS - this->duration_ms))
		{
			this->pending_callback = FALSE;
			this->callback(this->duration_ms);
		}
	}

}

static void BUTTON_set_callback(BUTTON* this, void(*callback)(INT32S duration_ms))
/****************************************************************************
*   Output   : Object is input
*   Function : Method for m_handler_button, pick mode
****************************************************************************/
{
	this->callback = callback;
}

static void _BUTTON_is_key_down(BUTTON* this)
/****************************************************************************
*   Output   : Object
*   Function : Method for m_handler_button, calculate if btn pressed
****************************************************************************/
{
	// check if button is pressed
	if(!(GPIO_PORTF_DATA_R & (1 << BTN_BIT)))
	{
		this->state = DEBOUNCING;

		// construct objects
		if (this->tp_pressed == NULL)
		{
			this->tp_pressed = tp.new(NORMAL);
			this->tp_db = tp.new(NORMAL);
			this->tp_pending = tp.new(NORMAL);
		}

		__disable_irq();
			tp.copy(this->tp_pressed, tp_global);
		__enable_irq();
	}
}

static void _BUTTON_debounce_button(BUTTON* this)
/****************************************************************************
*   Output   : Object
*   Function : Method for m_handler_button, calculate debounce_state
****************************************************************************/
{
	// check if button is still pressed
	if(!(GPIO_PORTF_DATA_R & (1 << BTN_BIT)))
	{
		__disable_irq();

		// check if debounce duration has been passed
		if(tp.delta(this->tp_pressed, tp_global, ms) >= DEBOUNCE_DUR_MS)
		{
			this->db_delta_ms = (INT32S)tp.delta(this->tp_db, tp_global, ms);
			tp.copy(this->tp_db, tp_global);
			this->state = KEY_DOWN;
		}

		__enable_irq();
	}
	else
	{
		this->state = KEY_UP;
	};
}


static void _BUTTON_key_press(BUTTON* this )
/****************************************************************************
*   Output   : Object
*   Function : Method for m_handler_button, pick mode
****************************************************************************/
{
	// update duration since key got pressed down
	__disable_irq();
	INT32S temp_duration_ms  = (INT32S)tp.delta(this->tp_pressed, tp_global, ms);
	__enable_irq();

	// check if button has been released (pressed)
	if(GPIO_PORTF_DATA_R & (1 << BTN_BIT))
	{
		// update duration
		__disable_irq();
		this->duration_ms  = temp_duration_ms;
		tp.copy(this->tp_released, tp_global);
		__enable_irq();

		// calculate time since last debounce
		INT32U diff_tp = this->db_delta_ms + this->duration_ms;

		// check whether button has been doublepressed
		if (diff_tp < DOUBLEPRESS_DUR_MS)
		{
			this->duration_ms = -1;
			this->pending_callback = FALSE;
			this->callback(this->duration_ms);
		}
		// otherwise init pending callback for single press
		else
		{
			__disable_irq();
			tp.copy(this->tp_pending, tp_global);
			this->pending_callback = TRUE;
			__enable_irq();
		}

		// change state to cooldown
		this->state = COOLDOWN;
	}
	else if (temp_duration_ms >= LONGPRESS_DUR_MS)
	{
		this->duration_ms  = temp_duration_ms;
		this->callback(this->duration_ms);

		// change state to cooldown
		this->state = COOLDOWN;
	}
}

static void _BUTTON_key_cooldown(BUTTON* this)
{
	__disable_irq();

	// check whether cooldown duration has passed & button has been released
	if((tp.delta(this->tp_released, tp_global, ms) >= DEBOUNCE_DUR_MS) && (GPIO_PORTF_DATA_R & (1 << BTN_BIT)))
	{
		this->state = KEY_UP;
	}
	__enable_irq();
}

static void _BUTTON_init_hardware(BUTTON* this)
/****************************************************************************
*   Input    : input this Button and Parameter
*   Function : Setup Hardware
****************************************************************************/
{
	// Enable GPIO F Register
	SYSCTL_RCGCGPIO_R	|= SYSCTL_RCGC2_GPIOF;

	asm volatile
	(
		"nop;"
		"nop;"
		"nop;"
	);

	// PORTF Direction for Button
	GPIO_PORTF_DIR_R	&= ~(1 << BTN_BIT);

	// PORF Pull UP - Active Low
	GPIO_PORTF_PUR_R	|= (1 << BTN_BIT);

	// PortF Digital enable
	GPIO_PORTF_DEN_R	|= (1 << BTN_BIT);
}

/****************************** End Of Module *******************************/
