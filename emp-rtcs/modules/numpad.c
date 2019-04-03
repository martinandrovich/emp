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
#include "tp.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

void NUMPAD_operate( void (*callback)(void));
void NUMPAD_init();
void NUMPAD_del();

/****************************   Class Struct   *****************************/

const struct NUMPAD numpad =
{
	.init			= &NUMPAD_init(),
	.write_char		= &NUMPAD_operate,
	.write_string   = &NUMPAD_del
};

void NUMPAD_init()
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


void NUMPAD_operate( void (*callback)(void) )
{
	static TIMEPOINT* timer =  tp.new();
	uint32_t  column = GPIO_PORTA_DATA_R;
	uint32_t  row = GPIO_PORTE_DATA_R;
	static uint32_t col_row[3][4];

		for(int row_ = 0; row_ < 4; row_++)
		{
			for(int col_ = 0; col_ < 3; col_++)
			{
				col_row[col_][row_] |= ( ((column >> 2 + col_) & 1 )  &( (row >> row_)) & 1));
				col_row[col_][row_] <<= 1;

				if(!col_row[col_][row_])
				{
					if(200 >tp.delta_now(timer,ms))
					{
						tp.now(timer);
						callback();
						col_row[col_][row_] = 0xffffffff;
					}
				}
			}
		}
}



void NUMPAD_del()
{


}
