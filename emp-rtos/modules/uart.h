/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	uart.h
* MODULENAME.:	UART
* API........:	https://goo.gl/pBQsgZ
* VERSION....:	1.1.0
*
* DESCRIPTION:	UART serial communication module.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "../tm4c123gh6pm.h"

#include "chksum.h"
#include "tp.h"

/*****************************    Defines    *******************************/

typedef struct  UART UART;
typedef struct	UART_FRAME UART_FRAME;
typedef enum    UART_FRAME_TYPE UART_FRAME_TYPE;
typedef enum    UART_BAUDRATE UART_BAUDRATE;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/

extern const struct UART_CLASS
{
	UART*		(*new)(UART_BAUDRATE baudrate);
	void		(*del)(UART* this);

	UART_FRAME*	(*newframe)();
	void 		(*delframe)(UART_FRAME* frame);

	void 		(*send)(UART* this, UART_FRAME_TYPE type, uint8_t* payload, uint8_t payload_size);
	bool		(*read)(UART* this, UART_FRAME* frame, bool send_ack);
} uart;

/*****************************    Constructs   *****************************/

enum UART_BAUDRATE
{
	BAUD_1200		= 0,
	BAUD_9600		= 1,
	BAUD_14400		= 2,
	BAUD_115200		= 3
};

enum UART_FRAME_TYPE
{
	UART_CONNECT	= 0x00,
	UART_RAW		= 0x01,
	UART_GET		= 0x02,
	UART_SET		= 0x03,
	UART_ACK		= 0x04,
	UART_RESPONSE	= 0x05,
	UART_STREAM		= 0x06,
	UART_MSG		= 0x07
};

struct UART_FRAME
{
	UART_FRAME_TYPE	type : 3;
	uint8_t 		payload_size : 5;
	uint8_t* 		payload;
	uint8_t 		chksum;
};

struct UART
{
	UART_BAUDRATE	baudrate;
	TIMEPOINT*		tp_timeout;
};

/****************************** End Of Module ******************************/
