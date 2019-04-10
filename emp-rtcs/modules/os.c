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

#include "exm.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static void 		OS_operate();
static void 		OS_create_task(void (*func_ptr)(void));
static void 		OS_sleep(uint8_t duration_ms);

inline static void 	_OS_run_task(uint8_t id);

/****************************   Class Struct   *****************************/

static struct OS_CLASS os =
{

	.tasks			= NULL,
	.next_id		= 0,

	.create_task	= NULL,
	.operate		= NULL,
	.sleep			= NULL

};

/*****************************   Functions   *******************************/

static void OS_operate()
{
}

static void OS_create_task(void (*func_ptr)(void))
{
	os.tasks[os.next_id].task_callback = function;
	os.tasks[os.next_id].id = os.next_id;
	os.next_id++;
}

static void OS_sleep(uint8_t duration_ms)
{
}

inline static void 	_OS_run_task(uint8_t id)
{
	os.tasks[id].task_callback();
}

/****************************** End Of Module ******************************/
