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

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

void NUMPAD_operate();
void NUMPAD_init( void (*callback)(void) );
void NUMPAD_del();

/****************************   Class Struct   *****************************/

const struct NUMPAD numpad =
{

	.init			= &NUMPAD_init,
	.write_char		= &NUMPAD_operate,
	.write_string   = &NUMPAD_del

};


void NUMPAD_init( void (*callback)(void) )
{
    // Enable GPIO C and GPIO D Register
	SYSCTL_RCGCGPIO_R	|= SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOE;

	asm volatile
	(
		"nop;"
		"nop;"
		"nop;"
	);

	// PORTA and PortC Direction
	GPIO_PORTA_DIR_R	&= ~( ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ) );
	GPIO_PORTE_DIR_R	&= ~( ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) );

    //
    GPIO_PORTA_PUR_R    |= ( ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ) );
    GPIO_PORTE_PUR_R    |= ( ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) );

	// PortC and PortD Digital
	GPIO_PORTA_DEN_R	|= ( ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ) );
	GPIO_PORTE_DEN_R	|= ( ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) );


}


void NUMPAD_operate()
{

}
