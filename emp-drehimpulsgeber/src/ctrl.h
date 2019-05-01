/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	ctrl
* MODULENAME.:	CONTROLLER
* API........:	n/a
* VERSION....:	1.0.0
*
* DESCRIPTION:	An example module. This might have a lengthy description, in
*				which case, we simply add some tabs.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>

#include <FreeRTOS.h>
#include <task.h>
#include <message_buffer.h>

#include "drehimpulsegeber.h"
#include <lcd.h>

/*****************************    Defines    *******************************/

/***********************     External Variables     ************************/

extern TaskHandle_t	htsk_ctrl;
extern TaskHandle_t	htsk_lcd;
extern MessageBufferHandle_t hmbf_lcd;

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

extern struct CTRL_CLASS
{
	uint32_t	notification;
	void 		(*task)(void* param);
} ctrl;

/*****************************    Constructs   *****************************/

/****************************** End Of Module ******************************/
