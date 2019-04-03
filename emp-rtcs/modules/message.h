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

typedef struct		MESSAGE MESSAGE;
typedef enum		MESSAGE_TYPE MESSAGE_TYPE;
typedef enum        MESSAGE_ID MESSAGE_ID;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

/*****************************    Constructs   *****************************/

enum MESSAGE_TYPE
{
	MESSAGE_IMPORTANT,
	MESSAGE_DATA
};

enum MESSAGE_ID
{
	MESSAGE_ID_UINT8,
    MESSAGE_ID_UINT16,
    MESSAGE_ID_UINT32,
    MESSAGE_ID_INT8,
    MESSAGE_ID_INT16,
    MESSAGE_ID_INT32,
	MESSAGE_ID_CHAR
};

struct MESSAGE
{
	uint32_t		data;
	uint32_t		id;
	MESSAGE_TYPE	type;
};

/****************************** End Of Module ******************************/
