/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	cbuf.h
* MODULENAME.:	C_BUFFER
* API........:	n/a
* VERSION....:	1.0.0
*
* DESCRIPTION:	An example module. This might have a lengthy description, in
*				which case, we simply add some tabs.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "message.h"

#include "../tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

typedef struct		C_BUFFER C_BUFFER;
typedef enum		C_BUF_ERROR C_BUF_ERROR;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

extern const struct C_BUFFER_CLASS
{
	C_BUFFER*		(*new)();
	void     	    (*del)(C_BUFFER* this);
	C_BUF_ERROR		(*read)(C_BUFFER* this, MESSAGE* data);
	C_BUF_ERROR		(*write)(C_BUFFER* this, MESSAGE data);
	uint32_t		(*length)(C_BUFFER* this);
	bool			(*empty)(C_BUFFER* this);
} cbuf;

/*****************************    Constructs   *****************************/

enum C_BUF_ERROR
{
	C_BUF_FAIL,
	C_BUF_DONE,
	C_BUF_FULL,
	C_BUF_EMPTY
};

struct C_BUFFER
{
	MESSAGE*		buffer;
	uint32_t		head;
	uint32_t		tail;
	uint32_t		size;
};

/****************************** End Of Module ******************************/
