/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	exm.h
* MODULENAME.:	EXAMPLE
* API........:	github.com/rb-pro4-f19/MCU/blob/master/README.md
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
#include <malloc.h>

#include "cbuf.h"

/*****************************    Defines    *******************************/

typedef struct TCB TCB;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

#define MAX_NUM_TASKS 	20
#define STACK_SIZE		40

/*****************************    Constructs   *****************************/

struct TCB
{
	uint8_t 	id;
	uin32_t*	sp;
	uint8_t 	state;
	uint8_t 	timer;

	C_BUFFER* 	event_queue;
	uint32_t	stack[STACK_SIZE];

	void (*callback)(void);
};

/*************************    Class Functions    ***************************/

extern struct OS_CLASS
{
	TCB 		tasks[MAX_NUM_TASKS];
	uint8_t 	next_id;

	void (*operate)(void);

	void (*create_task)(void (*callback)(void));
	void (*sleep)(uint8_t duration_ms);
} os;

/****************************** End Of Module ******************************/
