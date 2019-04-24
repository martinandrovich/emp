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
	static ENCODER_MSG* enc_msg;

	// task loop
	while(true)
	{
		// check notification
		xTaskNotifyWait
		(
			0x00,				/* Don't clear any notification bits on entry. */
			UINT8_MAX,			/* Reset the notification value to 0 on exit. */
			&ctrl.notification,	/* Where to store notified value. */
			portMAX_DELAY		/* Block indefinitely. */
		);

		// parse notification
		enc_msg = (ENCODER_MSG*)&ctrl.notification;

		// perform actions
		switch(enc_msg->id)
		{
			case ENC_A:
			{
				break;
			}

			case ENC_B:
			{
				break;
			}

		}

	}
}

/****************************** End Of Module ******************************/
