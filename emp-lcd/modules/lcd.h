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

#include "../tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

typedef enum	NIBBLE NIBBLE;
typedef enum 	LCD_TYPE LCD_TYPE;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

#define	EXM_ARRAY_SIZE 8

/*************************    Class Functions    ***************************/

extern const struct LCD_CLASS
{

	void 		(*operate)();
	void     	(*init)();
	void        (*write_char)(uint8_t data, uint8_t row, uint8_t column);
	void   		(*write_string)(const uint8_t * data, uint8_t row, uint8_t column, bool wrap, bool middle);
	void        (*clear)();

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
