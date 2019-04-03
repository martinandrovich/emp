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

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>

#include "../tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

typedef struct      CIRC_BUFFER CIRC_BUFFER;
typedef struct      MESSAGE MESSAGE;
typedef enum        ID ID;
typedef enum        Q_ERROR Q_ERROR;
typedef enum        MESSAGE_TYPE MESSAGE_TYPE;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

extern const struct CIRC_BUFFER_CLASS
{

	CIRC_BUFFER*    (*new)();
	void     	    (*del)(CIRC_BUFFER * cb);
    Q_ERROR         (*read)(CIRC_BUFFER * cb, MESSAGE data);
    Q_ERROR         (*write)(CIRC_BUFFER * cb, MESSAGE data);
    uint32_t        (*lendata)(CIRC_BUFFER * cb);
    bool            (*empty)(CIRC_BUFFER * cb);

} circ_buffer;

enum Q_ERROR{
    FAIL,
    DONE,
    BUFFERFULL,
    BUFFEREMPTY
};

enum MESSAGE_TYPE {
    IMPORTANT,
    DATA
};

enum ID {
    INT,
    CHAR
};

typedef struct {
    uint32_t        size;
    uint32_t        ID;
    MESSAGE_TYPE    type;
} MESSAGE;

typedef struct {
    message *       buffer;
    uint32_t        head;
    uint32_t        tail;
    uint32_t        size;
} CIRC_BUFFER;
/*****************************    Constructs   *****************************/

/****************************** End Of Module ******************************/
