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

#include "../tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

typedef struct		C_BUFFER C_BUFFER;
typedef struct		MESSAGE MESSAGE;
typedef enum		ID ID;
typedef enum		Q_ERROR Q_ERROR;
typedef enum		MESSAGE_TYPE MESSAGE_TYPE;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

extern const struct C_BUFFER_CLASS
{
	C_BUFFER*		(*new)();
	void     	    (*del)(C_BUFFER* this);
	Q_ERROR			(*read)(C_BUFFER* this, MESSAGE data);
	Q_ERROR			(*write)(C_BUFFER* this, MESSAGE data);
	uint32_t		(*length)(C_BUFFER* this);
	bool			(*empty)(C_BUFFER* this);
} cbuf;

/*****************************    Constructs   *****************************/

enum Q_ERROR
{
	FAIL,
	DONE,
	BUFFERFULL,
	BUFFEREMPTY
};

enum MESSAGE_TYPE
{
	IMPORTANT,
	DATA
};

enum ID
{
	INT,
	CHAR
};

struct MESSAGE
{
	uint32_t		size;
	uint32_t		id;
	MESSAGE_TYPE	type;
};

struct C_BUFFER
{
	message*		buffer;
	uint32_t		head;
	uint32_t		tail;
	uint32_t		size;
};

/****************************** End Of Module ******************************/
