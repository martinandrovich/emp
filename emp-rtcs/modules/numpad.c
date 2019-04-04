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

struct NUMPAD_CLASS numpad =
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

	// PORTA (col) and PORTE (row) Direction
	GPIO_PORTA_DIR_R	&= ~( ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ) );
	GPIO_PORTE_DIR_R	|= ( ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) );

    //
    //GPIO_PORTA_PUR_R    |= ( ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ) );
	GPIO_PORTA_PDR_R	|= ( ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ) );
    //GPIO_PORTE_PUR_R    |= ( ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) );

	// PortC and PortD Digital
	GPIO_PORTA_DEN_R	|= ( ( 1 << 2 ) | ( 1 << 3 ) | ( 1 << 4 ) );
	GPIO_PORTE_DEN_R	|= ( ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) );

	numpad.callback = callback;
}

void NUMPAD_operate()
{
	static TIMEPOINT* tp_timer = NULL;
	if (tp_timer == NULL) { tp_timer = tp.new(); }

	volatile uint32_t  data_col = GPIO_PORTA_DATA_R;
	volatile uint32_t*  data_row = &GPIO_PORTE_DATA_R;

	static uint32_t shift_reg[12];

	 for(int row_idx = 0; row_idx < 4; row_idx++)
	 {

		 *data_row &= ~( ( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 2 ) | ( 1 << 3 ) );
		 *data_row |= (1 << row_idx);

		 data_col = GPIO_PORTA_DATA_R;

		 asm volatile
	 	(
	 		"nop;"
	 		"nop;"
	 		"nop;"
	 	);

	 	for(int col_idx = 0; col_idx < 3; col_idx++)
	 	{
			shift_reg[col_idx + row_idx * 3] <<= 1;

			uint8_t the_bit = ( data_col >> ( 2 + col_idx ) ) & 1;

			shift_reg[col_idx + row_idx * 3] |= ( the_bit & 1 );

	 		if( ((shift_reg[col_idx + row_idx * 3]) && tp.delta_now(tp_timer, ms) > 200) )
	 		{
 				tp.set(tp_timer, tp.now());
				char tmp = _NUMPAD_getchar(row_idx, col_idx);
 				numpad.callback(tmp);
				memset(&shift_reg, 0, 11);
				break;
	 		}
	 	}
	 }
}

static uint8_t 	_NUMPAD_getchar(uint8_t row, uint8_t col)
{
	switch(row*3 + col)
	{
		case 10:
			return '*';

		case 11:
			return '0';

		case 12:
			return '#';

		default:
			return '0'+ row*3 + col;
	}
}
