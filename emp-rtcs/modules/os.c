/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	exm.c
* MODULENAME.:	EXAMPLE
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "os.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static void 		OS_operate();
static void 		OS_create_task(void (*callback)(void));
static void 		OS_sleep(uint8_t duration_ms);

inline static void 	_OS_run_task(uint8_t id);

/****************************   Class Struct   *****************************/

struct OS_CLASS os =
{
	.next_id		= 0,

	.create_task	= &OS_create_task,
	.operate		= &OS_operate,
	.sleep			= &OS_sleep
};

/*****************************   Functions   *******************************/

static void OS_operate()
{
	;
}

static void OS_create_task(void (*callback)(void))
{

	// check if enough space for more tasks
	;

	// assign ID and callback to TCB
	os.tasks[os.next_id].id = os.next_id;
	os.tasks[os.next_id].callback = callback;

	// allocate event queue (CBUFFER)
	;

	// increment counter for next
	os.next_id++;
}

static void OS_sleep(uint8_t duration_ms)
{
	;
}

inline static void 	_OS_run_task(uint8_t id)
{
	os.tasks[id].callback();
}

/****************************** End Of Module ******************************/
