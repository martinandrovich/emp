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

#include "lcd.h"

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

static void LCD_init();
static void _LCD_delay(uint32_t);
static void _LCD_write(uint8_t, LCD_TYPE, NIBBLE);
static void LCD_write_char(uint8_t, uint8_t, uint8_t);
static void LCD_write_string(const uint8_t *, uint8_t, uint8_t, bool, bool); //    \0 escape char
static void LCD_clear();


/****************************   Class Struct   *****************************/

const struct LCD_CLASS lcd =
{
	.init			= &LCD_init,
	.write_char		= &LCD_write_char,
	.write_string   = &LCD_write_string,
	.clear          = &LCD_clear

};

/***********************   Constructive Functions   ************************/

/*****************************   Functions   *******************************/
static void LCD_write_string(const uint8_t * data, uint8_t row, uint8_t column, bool wrap, bool middle)
/****************************************************************************
*   Input    : data, row, column
*   Function : Send Lower and Upper Nibble
****************************************************************************/
{

	volatile uint8_t len_of_data = 0;
	volatile uint8_t len_data_possible = 0;
	volatile uint8_t wrapable = 0;
	volatile uint8_t i = 0;
	volatile uint8_t index = 0;

	while( data[ len_of_data ] != '\0' ) { len_of_data++; };

	if ( column >= 0 && column <= 15 && (row == 1 || row == 0) && len_of_data <= 32 && len_of_data != 0 )
	{

		len_of_data 		= len_of_data - 1;
		len_data_possible 	= ( ( row == 0 ) ? column : ( column + 16 ) );
		wrapable 			= ( ( 31 - len_data_possible ) >= len_of_data ) ? 1 : 0;

		if ( !wrap || wrapable )
		{
			switch ( row )
			{
				case 0:

					_LCD_write(column + 0x80, CMD , BOTH );

					for(i = column; i <= 15; i++)
					{
						if ( index > len_of_data )
						{
							i = 16;
						}
						else
						{
		                    _LCD_write( data[ index++ ] , DATA , BOTH );
						}
					}

					column = 16;

				case 1:

					_LCD_write( ( ( row == 0 ) ? 0 : column ) + 0x80 + 0x40, CMD , BOTH );

					for(i = column; i <= 31; i++)
					{
						if ( index > len_of_data )
						{
							i = 32;
						}
						else
						{
		                    _LCD_write( data[ index++ ] , DATA , BOTH );
						}
					}

					break;
			}
		}
	}
}


static void LCD_write_char(uint8_t data, uint8_t row, uint8_t column)
/****************************************************************************
*   Input    : data, row, column
*   Function : Send Lower and Upper Nibble
****************************************************************************/
{
	switch (row) {

		case 0:

			if ( column >= 0 && column <= 15 )
			{
                // DB6 to DB0 is Adress, so therefor offset with 0x80
				_LCD_write( column + 0x80, CMD , BOTH );
				_LCD_write( data , DATA , BOTH );
			}

		break;

		case 1:

			if ( column >= 0 && column <= 15 )
			{
			    // DB6 to DB0 is Adress, so therefor offset with 0x80
				_LCD_write( column + 0x80 + 0x40, CMD , BOTH );
				_LCD_write( data , DATA , BOTH );
			}

		break;

		default:

			LCD_write_string( (uint8_t *) "FAIL", 0, 6, 0, 0);

		    break;

			// node this is an error, current nothing happends - ASSERT here
	}
}

static void _LCD_write(uint8_t cmd, LCD_TYPE type, NIBBLE nibble)
/****************************************************************************
*   Input    : this = pointer to a LCD instance, uint8_t and type
*   Function : Send Lower and Upper Nibble
****************************************************************************/
{

	switch (nibble)
	{

		case BOTH:

		case UPPER:

			//0xF0 is a MASK
			GPIO_PORTC_DATA_R = ( cmd & 0xF0 ) | ( GPIO_PORTC_DATA_R & ~0xF0 );

			//Prepare to Send Data
			GPIO_PORTD_DATA_R = ( ( ( ( ( type == DATA ? 1 : 0 ) << LCD_RS) | ( 1 << LCD_E ) ) & 0x0C ) | ( GPIO_PORTD_DATA_R & ~0x0C ) );

			_LCD_delay( DELAY_NS( 10000 ) );

			//Set Enable Low - Send!
			GPIO_PORTD_DATA_R = ( GPIO_PORTD_DATA_R & ~0x08 );

			_LCD_delay( DELAY_NS( 10000 ) );

			if (nibble != BOTH) { break; }

		case LOWER:

			//0xF0 is MASK
			GPIO_PORTC_DATA_R = ( ( cmd << 4 ) & 0xF0 ) | ( GPIO_PORTC_DATA_R & ~0xF0 );

			//Prepare to Send Data
			GPIO_PORTD_DATA_R = ( ( ( ( ( type == DATA ? 1 : 0 ) << LCD_RS) | ( 1 << LCD_E ) ) & 0x0C ) | ( GPIO_PORTD_DATA_R & ~0x0C ) );

			_LCD_delay( DELAY_NS( 10000 ) );

			//Set Enable Low - Send!
			GPIO_PORTD_DATA_R = ( GPIO_PORTD_DATA_R & ~0x08 );

			_LCD_delay( DELAY_NS( 10000 ) );

			break;

		default:

			break;

	}

}

static void LCD_clear()
/****************************************************************************
*   Function : cleans up the display
****************************************************************************/
{
	_LCD_write(0x01, CMD, BOTH);
};


static void LCD_init()
/****************************************************************************
*   Function : init function for LCD
****************************************************************************/
{

	// Enable GPIO C and GPIO D Register
	SYSCTL_RCGCGPIO_R	|= SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD;

	asm volatile
	(
		"nop;"
		"nop;"
		"nop;"
	);

	// PORTC and PortD Direction
	GPIO_PORTC_DIR_R	|= ( ( 1 << PC4 ) | ( 1 << PC5 ) | ( 1 << PC6 ) | ( 1 << PC7) );
	GPIO_PORTD_DIR_R	|= ( ( 1 << PD2 ) | ( 1 << PD3 ) );

	// PortC and PortD Digital
	GPIO_PORTC_DEN_R	|= ( ( 1 << PC4 ) | ( 1 << PC5 ) | ( 1 << PC6 ) | ( 1 << PC7) );
	GPIO_PORTD_DEN_R	|= ( ( 1 << PD2 ) | ( 1 << PD3 ) );

	// atleast from startup
	_LCD_delay(DELAY_NS(15000000));
    _LCD_write(0x03, CMD, LOWER);

	_LCD_delay(DELAY_NS(4100000));
    _LCD_write(0x03, CMD, LOWER);

	_LCD_delay(DELAY_NS(100000));
	_LCD_write(0x03, CMD, LOWER);

	_LCD_write(0x02, CMD, LOWER);

	 // 4 bit mode 1/16 duty 5x8 font
	_LCD_write(0x28, CMD, BOTH);

	// Display Off
	_LCD_write(0x08, CMD, BOTH);

	// Display on - Blink Curson on;
	_LCD_write(0x0C, CMD, BOTH);

	// Entry mode
	_LCD_write(0x06, CMD, BOTH);

	// Home
	_LCD_write(0x01, CMD, BOTH);

}

static void _LCD_delay(uint32_t delay)
/****************************************************************************
*   Function : LCD -- delay function very unprecise
****************************************************************************/
{

    volatile uint32_t delay_t = delay;
	while (delay_t != 0) { delay_t--;};

}

/****************************** End Of Module ******************************/
