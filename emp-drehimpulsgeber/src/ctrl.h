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

#include <FreeRTOS.h>
#include <task.h>

#include <stdint.h>
#include <stdbool.h>

/*****************************    Defines    *******************************/

typedef struct	ENCODER_MSG ENCODER_MSG;
typedef enum	ENC_MSG_ID ENC_MSG_ID;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

extern struct CTRL_CLASS
{
	uint32_t	notification;
	void 		(*task)(void* pvParameters);
} ctrl;

/*****************************    Constructs   *****************************/

enum ENC_MSG_ID
{
	ENC_A,
	ENC_B,
	ENC_C
};

typedef struct ENCODER_MSG
{
	ENC_MSG_ID 	id  : 2;
	uint32_t 	dat : 30;
};



/****************************** End Of Module ******************************/
