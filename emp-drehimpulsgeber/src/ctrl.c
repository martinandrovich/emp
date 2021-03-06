/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	ctrl.c
* MODULENAME.:	CONTROLLER
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include "ctrl.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/
static inline void _CTRL_split_int12(uint8_t * data, int32_t split);
static inline void _CTRL_clear_array(uint8_t * data);
static inline void _CTRL_reset_is_set(uint8_t * data);
static inline void _CTRL_set_dir(uint8_t * data, int32_t dir);
static inline void _CTRL_construct_message(uint8_t * data, DREHIMPULSEGEBER_MSG* msg);
static void CTRL_task(void* pvParameters);

/****************************   Class Struct   *****************************/

struct CTRL_CLASS ctrl =
{
	.notification	= 0,
	.task 			= &CTRL_task
};

/*****************************   Functions   *******************************/

static void CTRL_task(void* pvParameters)
{

	static DREHIMPULSEGEBER_MSG* msg;
	static uint8_t msg_to_lcd[LCD_DATA_ARRAY_SIZE];

	// task loop
	while (true)
	{

		// wait for task notification
		xTaskNotifyWait
		(
			0x00,					/* Don't clear any notification bits on entry. */
			UINT32_MAX,				/* Reset the notification value to 0 on exit. */
			&(ctrl.notification),	/* Where to store notified value. */
			portMAX_DELAY			/* Block indefinitely. */
		);

		// parse notification data
		msg = (DREHIMPULSEGEBER_MSG*)&(ctrl.notification);

		// construct message_buffer
		_CTRL_construct_message(msg_to_lcd, msg);

		// send message to buffer
		xMessageBufferSend
		(
			hmbf_lcd, 				/* LCD MessageBufferHandle */
			(void*)msg_to_lcd,		/* Pointer to the array. */
			sizeof(msg_to_lcd),		/* Size of the passed array. */
			0						/* The time to wait. */
		);

		// notify the LCD task
		xTaskNotify(htsk_lcd, 0, eSetValueWithOverwrite);
	}

}

static inline void _CTRL_construct_message(uint8_t * msg_to_lcd, DREHIMPULSEGEBER_MSG* msg)
{
    _CTRL_clear_array(msg_to_lcd);

    _CTRL_split_int12((msg_to_lcd+6), (msg->pos)*6);

    // set the direction
    _CTRL_set_dir(msg_to_lcd, msg->dir);

    // only if the reset button is pressed
    if (msg->rst == true) _CTRL_reset_is_set(msg_to_lcd);
};


static inline void _CTRL_clear_array(uint8_t * data)
{
	for (uint8_t i = 0; i < 32; i++)
	{
		switch (i)
		{
			case 1:

				*(data+i) = 'p';
				i++;
				*(data+i) = 'o';
				i++;
				*(data+i) = 's';
				i++;
				*(data+i) = ':';
				break;

			case 24:

				*(data+i) = 'd';
				i++;
				*(data+i) = 'i';
				i++;
				*(data+i) = 'r';
				i++;
				*(data+i) = ':';
				break;

			default:

				*(data+i) = ' ';

		}
	}

	*(data+31) = '\0';
}


static inline void _CTRL_split_int12(uint8_t * data, int32_t split)
{
	uint8_t i = 0;

	if( split < 0 ) { *(data + i) = (uint8_t)'-' ;}

	i++;

	split *= ( split >= 0 ) ? 1 : -1;

	split %= 360;

	*(data+i) = (uint8_t)( ( split / 100 ) % 10 ) + '0';
	i++;
	*(data+i) = (uint8_t)( ( split / 10 ) % 10 ) + '0';
	i++;
	*(data+i) = (uint8_t)( ( split / 1 ) % 10 ) + '0';

}

static inline void _CTRL_reset_is_set(uint8_t * data)
{
	uint8_t i = 17;
	*(data+i) = 'r';
	i++;
	*(data+i) = 'e';
	i++;
	*(data+i) = 's';
	i++;
	*(data+i) = 'e';
	i++;
	*(data+i) = 't';
}

static inline void _CTRL_set_dir(uint8_t * data, int32_t dir)
{
	switch (dir)
	{
		case -1:
			*(data+28) = '-';
			*(data+29) = '>';
			break;

		case 0:
			*(data+28) = '-';
			*(data+29) = '-';
			break;

		case 1:
			*(data+28) = '<';
			*(data+29) = '-';
			break;
	}
}

/****************************** End Of Module ******************************/
