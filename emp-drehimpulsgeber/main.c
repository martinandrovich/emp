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

#define DEFAULT_STACK 		512

/***************************** Include files *******************************/

#include <FreeRTOS.h>
#include <task.h>
#include <message_buffer.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <systick.h>

#include "ctrl.h"
#include "lcd.h"

/*****************************   Handles   *********************************/

TaskHandle_t			htsk_ctrl;
TaskHandle_t			htsk_lcd;
TaskHandle_t			htsk_drehimpulsegeber;

MessageBufferHandle_t	hmbf_lcd;

/*******************************   Main   **********************************/

int main(void)
{

    // that compiler bug tho..
    //volatile int i = 20;

    disable_global_int();

    // init hardware
    init_systick();
    lcd.init();
    drehimpulsegeber.init();


	// create message buffers
	hmbf_lcd = xMessageBufferCreate(LCD_DATA_ARRAY_SIZE*2);


	// create controller task
	xTaskCreate
	(
		ctrl.task,				/* Function that implements the task. */
		"ctrl_task",			/* Text name for the task. */
		DEFAULT_STACK, 			/* Stack size in words, not bytes. */
		NULL, 					/* Parameter passed into the task. */
		1,						/* Priority at which the task is created. */
		&htsk_ctrl				/* Used to pass out the created task's handle. */
	);

	// create LCD task
	xTaskCreate
	(
		lcd.task,				/* Function that implements the task. */
		"lcd_task",				/* Text name for the task. */
		DEFAULT_STACK, 			/* Stack size in words, not bytes. */
		NULL, 					/* Parameter passed into the task. */
		2,						/* Priority at which the task is created. */
		&htsk_lcd				/* Used to pass out the created task's handle. */
	);

	// create drehimpulsegeber task
	xTaskCreate
	(
		drehimpulsegeber.task,	/* Function that implements the task. */
		"dig_task",				/* Text name for the task. */
		DEFAULT_STACK, 			/* Stack size in words, not bytes. */
		NULL, 					/* Parameter passed into the task. */
		1,						/* Priority at which the task is created. */
		&htsk_drehimpulsegeber	/* Used to pass out the created task's handle. */
	);


    enable_global_int();


	// start scheduler
	vTaskStartScheduler();

	// terminate program
	return 0;
}

/****************************** End Of Module ******************************/
