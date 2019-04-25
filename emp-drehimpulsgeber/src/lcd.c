/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:  exm.c
* MODULENAME.:  LCD
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
#define PC6    6
#define PC5    5
#define PC4    4
#define PD3    3
#define PD2    2

#define DELAY_NS(X) ( X / (62.5) ) // one clk cycle is 62.5 ns (bad approx method tho)

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static void LCD_init();
static void LCD_task(void* param)
;
static void _LCD_write(uint8_t, LCD_TYPE, NIBBLE);
static void _LCD_delay(uint32_t);
static void LCD_write_char(uint8_t, uint8_t, uint8_t);
static void LCD_write_string(const uint8_t*, uint8_t, uint8_t, bool, bool); //    \0 escape char
static void LCD_write_string_s(const uint8_t* data);
static void LCD_clear();
static void _LCD_split_uint16_t(uint16_t split, uint8_t * data);

/****************************   Class Struct   *****************************/

struct LCD_CLASS lcd =
{
	.notification 	= 0,

	.init           = &LCD_init,
	.write_char     = &LCD_write_char,
	.write_string   = &LCD_write_string,
	.write_string_s = &LCD_write_string_s,
	.clear          = &LCD_clear,
	.task           = &LCD_task
};

void LCD_task(void* param)
{
	static uint8_t lcd_str[LCD_DATA_ARRAY_SIZE];
	const  TickType_t block_dur = pdMS_TO_TICKS(20);

	while(true)
	{
		// wait for task notification
		xTaskNotifyWait
		(
			0x00,				/* Don't clear any notification bits on entry. */
			UINT32_MAX,			/* Reset the notification value to 0 on exit. */
			&lcd.notification,	/* Where to store notified value. */
			portMAX_DELAY		/* Block indefinitely. */
		);

		// read message buffer
		xMessageBufferReceive
		(
			hmbf_lcd,			/* Handle to message buffer. */
			(void*)lcd_str,		/* Where to store the read value. */
			sizeof(lcd_str),	/* Size of the value to read. */
			block_dur			/* Block for some duration. */
		);

		// output data to display
		lcd.write_string_s(lcd_str);

	};

};

static void LCD_write_string_s(const uint8_t* data)
/****************************************************************************
*   Input    : implemntation, without row, column, etc
*   Function : Send Lower and Upper Nibble
****************************************************************************/
{
	LCD_clear();
	_LCD_delay(10);
	LCD_write_string(data, 0, 0, 1, 0);
};

/*****************************   Functions   *******************************/
static void LCD_write_string(const uint8_t* data, uint8_t row, uint8_t column, bool wrap, bool middle)
/****************************************************************************
*   Input    : data, row, column, wrap - middle not implemented
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

		len_of_data         = len_of_data - 1;
		len_data_possible   = ( ( row == 0 ) ? column : ( column + 16 ) );
		wrapable            = ( ( 31 - len_data_possible ) >= len_of_data ) ? 1 : 0;

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
	for(int i = 0; i < 10000; i++);
};


static void LCD_init()
/****************************************************************************
*   Function : init function for LCD
****************************************************************************/
{
	// Enable GPIO C and GPIO D Register
	SYSCTL_RCGCGPIO_R   |= SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD;

	asm volatile
	(
		"nop;"
		"nop;"
		"nop;"
	);

	// PORTC and PortD Direction
	GPIO_PORTC_DIR_R    |= ( ( 1 << PC4 ) | ( 1 << PC5 ) | ( 1 << PC6 ) | ( 1 << PC7) );
	GPIO_PORTD_DIR_R    |= ( ( 1 << PD2 ) | ( 1 << PD3 ) );

	// PortC and PortD Digital
	GPIO_PORTC_DEN_R    |= ( ( 1 << PC4 ) | ( 1 << PC5 ) | ( 1 << PC6 ) | ( 1 << PC7) );
	GPIO_PORTD_DEN_R    |= ( ( 1 << PD2 ) | ( 1 << PD3 ) );

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
	LCD_clear();

}

static void _LCD_delay(uint32_t delay)
/****************************************************************************
*   Function : LCD -- delay function very unprecise
****************************************************************************/
{

	volatile uint32_t delay_t = delay;
	while (delay_t != 0) { delay_t--;};

}

static void _LCD_split_uint16_t(uint16_t split, uint8_t * data)
{
	*(data) = ( split / 1000 ) % 10 + 48;
	*(data+1) = ( split / 100 ) % 10 + 48;
	*(data+2) = ( split / 10 ) % 10 + 48;
	*(data+3) = ( split / 1 ) % 10 + 48;
	*(data+4) = '\0';
}

/****************************** End Of Module ******************************/
