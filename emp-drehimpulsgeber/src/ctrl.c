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
#include "drehimpulsegeber.h"

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
	static DREHIMPULSEGEBER_MSG* msg;

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

		// perform actions
		;
	}
}

/****************************** End Of Module ******************************/
