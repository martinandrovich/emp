/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:  exm.h
* MODULENAME.:  EXAMPLE
* API........:  github.com/rb-pro4-f19/MCU/blob/master/README.md
* VERSION....:  1.0.0
*
* DESCRIPTION:  An example module. This might have a lengthy description, in
*               which case, we simply add some tabs.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <message_buffer.h>

#include "digiswitch.h"

/*****************************    Defines    *******************************/

typedef enum    NIBBLE NIBBLE;
typedef enum    LCD_TYPE LCD_TYPE;
typedef struct  ADC_SET ADC_SET;

/***********************     External Variables     ************************/

extern MessageBufferHandle_t hmbf_lcd;

/*****************************   Constants   *******************************/

#define EXM_ARRAY_SIZE 8

/*************************    Class Functions    ***************************/

extern struct LCD_CLASS
{
	uint32_t	notification;

	void        (* const operate)();
	void        (* const init)();
	void        (* const write_char)(uint8_t data, uint8_t row, uint8_t column);
	void        (* const write_string)(const uint8_t * data, uint8_t row, uint8_t column, bool wrap, bool middle);
	void        (* const write_string_s)(const uint8_t * data);
	void        (* const clear)();
	void        (* const task)(void *pm);

} lcd;

/*****************************    Constructs   *****************************/

enum NIBBLE
{
	UPPER,
	LOWER,
	BOTH
};

enum LCD_TYPE
{
	CMD,
	DATA
};

/****************************** End Of Module ******************************/
