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
static void _CTRL_split_int(int32_t split, uint8_t * data);
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
	static uint8_t msg_to_lcd [32] = {' '};

	// task loop
	while(true)
	{
		// wait for task notification
		xTaskNotifyWait
		(
			0x00,				/* Don't clear any notification bits on entry. */
			UINT32_MAX,			/* Reset the notification value to 0 on exit. */
			&ctrl.notification,	/* Where to store notified value. */
			portMAX_DELAY		/* Block indefinitely. */
		);

		// parse notification data
		msg = (DREHIMPULSEGEBER_MSG*)&ctrl.notification;

		msg_to_lcd[1] = 'p';
		msg_to_lcd[2] = 'o';
		msg_to_lcd[3] = 's';
		msg_to_lcd[4] = ':';

		msg_to_lcd[24] = 'd';
		msg_to_lcd[25] = 'i';
		msg_to_lcd[26] = 'r';
		msg_to_lcd[27] = ':';

		_CTRL_split_int(msg->pos, (msg_to_lcd+6));

		// perform actions
		switch(msg->rst)
		{
			case true:
			{
				msg_to_lcd[17] = 'r';
				msg_to_lcd[18] = 'e';
				msg_to_lcd[19] = 's';
				msg_to_lcd[20] = 'e';
				msg_to_lcd[21] = 't';
				break;
			}

			case false:
			{
				msg_to_lcd[17] = ' ';
				msg_to_lcd[18] = ' ';
				msg_to_lcd[19] = ' ';
				msg_to_lcd[20] = ' ';
				msg_to_lcd[21] = ' ';
				break;
			}

		}

		switch (msg->dir)
		{
			case -1:
				msg_to_lcd[28] = '-';
				msg_to_lcd[29] = '>';

			case 0:
				msg_to_lcd[28] = '-';
				msg_to_lcd[29] = '-';

			case 1:
				msg_to_lcd[28] = '<';
				msg_to_lcd[29] = '-';
		}




	}

}

static void _CTRL_split_int(int32_t split, uint8_t * data)
{

	if( split < 0 ) { *(data) = (uint8_t)'-' ;}

	split *= ( split >= 0 ) ? 1 : -1;

	*(data+1) = (uint8_t)( ( split / 100 ) % 10 ) + '0';
	*(data+2) = (uint8_t)( ( split / 10 ) % 10 ) + '0';
	*(data+3) = (uint8_t)( ( split / 1 ) % 10 ) + '0';

}

/****************************** End Of Module ******************************/
