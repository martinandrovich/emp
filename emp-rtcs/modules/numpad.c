/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	exm.c
* MODULENAME.:	LCD
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include "numpad.h"

/*****************************    Defines    *******************************/

#define LCD_RS 2
#define LCD_E  3
#define PC7    7
#define PC6	   6
#define PC5	   5
#define PC4	   4
#define PD3    3
#define PD2    2
#define DELAY_NS(X) ( X / (62.5) ) // one clk cycle is 62.5 ns (bad approx method tho)

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

void 		NUMPAD_operate();
void     	NUMPAD_init();
void        NUMPAD_del();

/****************************   Class Struct   *****************************/

const struct NUMPAD numpad =
{

	.init			= &NUMPAD_init,
	.write_char		= &NUMPAD_operate,
	.write_string   = &NUMPAD_del

};
