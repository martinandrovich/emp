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

/*****************************    Defines    *******************************/

typedef struct C_BUFFER C_BUFFER;
typedef struct TCB TCB;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

extern struct OS_CLASS {

    TCB tasks[20];
    uint8_t next_id;

	void (*create_task)(void (*func_ptr)(void));
	void (*operate)(void);
	void (*sleep)(uint8_t duration_ms);

} os;

/*****************************    Constructs   *****************************/

struct TCB {

    uint8_t state;
    uint8_t id;
    C_BUFFER * event_queue;
    uint8_t timer;
    void (*task_callback)(void);

};


/****************************** End Of Module ******************************/
