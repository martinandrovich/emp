/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	main.c
* MODULENAME.:	MAIN
* VERSION....:	1.0.0
*
*****************************************************************************

// all std includes are defined in "FreeRTOSConfig.h", such as stdint.h etc.
// more information about the system can be found in "README.MD".

****************************************************************************/

#define DEFAULT_STACK 		128

/***************************** Include files *******************************/

#include <FreeRTOS.h>
#include <task.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "ctrl.h"

/*****************************   Handles   *********************************/

TaskHandle_t	htsk_ctrl;
TaskHandle_t	htsk_disp;
TaskHandle_t	htsk_enc;

/*******************************   Main   **********************************/

int main(void)
{

	// create controller task
	xTaskCreate
	(
		ctrl.task,			/* Function that implements the task. */
		"ctrl_task",		/* Text name for the task. */
		DEFAULT_STACK, 		/* Stack size in words, not bytes. */
		NULL, 				/* Parameter passed into the task. */
		tskIDLE_PRIORITY,	/* Priority at which the task is created. */
		&htsk_ctrl			/* Used to pass out the created task's handle. */
	);

	// create encoder task
	;

	// create display task
	;

	// start scheduler
	;

	// terminate program
	return 0;
}

/****************************** End Of Module ******************************/
