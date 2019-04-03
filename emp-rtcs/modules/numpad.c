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

static void 	NUMPAD_operate();
static void 	NUMPAD_init(void (*callback)(uint8_t byte));

static uint8_t 	_NUMPAD_getchar(uint8_t row, uint8_t col);

/****************************   Class Struct   *****************************/

struct NUMPAD numpad =
{
	.callback 		= NULL,

	.init			= &NUMPAD_init,
	.operate 		= &NUMPAD_operate
};

void NUMPAD_init(void (*callback)(uint8_t byte))
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

	numpad.callback = callback;
}

void NUMPAD_operate()
{
	static TIMEPOINT* tp_timer = NULL;
	if (tp_timer == NULL) { tp.new(); }

	uint32_t  column = GPIO_PORTA_DATA_R;
	uint32_t  row = GPIO_PORTE_DATA_R;

	static uint32_t col_row[3][4];

	// for(int row_ = 0; row_ < 4; row_++)
	// {
	// 	for(int col_ = 0; col_ < 3; col_++)
	// 	{
	// 		//col_row[col_][row_] |= ( ((column >> 2 + col_) & 1 )  &( (row >> row_)) & 1));
	// 		col_row[col_][row_] <<= 1;
	// 		// shift AFTER masking???
	//
	// 		if(!col_row[col_][row_])
	// 		{
	// 			if(200 >tp.delta_now(timer,ms))
	// 			{
	// 				tp.set(tp_timer, tp.now());
	// 				numpad.callback('1');
	// 				col_row[col_][row_] = 0xffffffff;
	// 			}
	// 		}
	// 	}
	// }
}



void NUMPAD_del()
{


}
