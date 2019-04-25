/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: adc.c
*
* PROJECT....: EMP
*
* DESCRIPTION: See module specification file (.h-file).
*
* Change Log:
*****************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 150215  MoH   Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/

#include "drehimpulsegeber.h"

/*****************************    Defines    *******************************/

#define CHECK_BIT(var,pos) ( (var) & (1 << (pos) ) )

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static void DREHIMPULSEGEBER_init();
static void DREHIMPULSEGEBER_task(void* param);

/*************************    Class Functions    ***************************/

struct DREHIMPULSEGEBER_CLASS drehimpulsegeber =
{
	.init       = &DREHIMPULSEGEBER_init,
	.task       = &DREHIMPULSEGEBER_task
};

/*****************************   Functions   *******************************/

static void DREHIMPULSEGEBER_init()
{

	SYSCTL_RCGCGPIO_R   |= SYSCTL_RCGC2_GPIOA;

	asm volatile
	(
		"nop;"
		"nop;"
		"nop;"
	);

	GPIO_PORTA_DIR_R    &= ~( ( 1 << 5 ) | ( 1 << 6 ) | ( 1 << 7 ) );

	GPIO_PORTA_DEN_R    |= ( ( 1 << 5 ) | ( 1 << 6 ) | ( 1 << 7) );

}


static void DREHIMPULSEGEBER_task(void* param)
{

	const TickType_t xDelay = pdMS_TO_TICKS(1);

	volatile int8_t prev_AB = (CHECK_BIT(GPIO_PORTA_DATA_R, 5) >> 4) + ( CHECK_BIT(GPIO_PORTA_DATA_R, 6 ) << 6);
	volatile int8_t AB[2] = { prev_AB, prev_AB };
	volatile uint8_t reset_button = 0;

	static int16_t 	dir = 0;
	static int16_t 	pos = 0;
	static bool 	send_msg = false;

	DREHIMPULSEGEBER_MSG msg = {0};

	while(true)
	{

		//read reset button
		reset_button = CHECK_BIT(GPIO_PORTA_DATA_R, 7) >> 7;

		if (!reset_button)
		{
			send_msg = true;
		}

		//read from digiswitch
		AB[0] = AB[1];

		AB[1] = ( CHECK_BIT(GPIO_PORTA_DATA_R, 5) >> 4) + ( CHECK_BIT(GPIO_PORTA_DATA_R, 6 ) >> 6 );

		if ( ( prev_AB == AB[1] ) || ( AB[1] != AB[0] ) )
		{
			dir = 0;
		}
		else
		{
			switch ( prev_AB )
			{
				case 0:
					dir = ( AB[1] == 1 ) ? -1 : 1;
					break;

				case 1:
					dir = ( AB[1] == 3 ) ? -1 : 1;
					break;

				case 2:
					dir = ( AB[1] == 0 ) ? -1 : 1;
					break;

				case 3:
					dir = ( AB[1] == 2 ) ? -1 : 1;
					break;
			};

			// save prev state
			prev_AB = AB[1];

			// increment position
			pos += dir;

			// send notification
			send_msg = true;

		};

		if (send_msg)
		{
			// direction parsed to msg
			msg.dir = dir;

			// revolution parsed to msg
			msg.revol = 0;

			// reset parsed to msg
			msg.rst = !reset_button;

			//
			if (!reset_button)
			{
				msg.pos = 0;
				pos = 0;
			}
			else
			{
				msg.pos = pos;
			}

			// convert message struct to uint32_t
			uint32_t* msg_val = (uint32_t*)&msg;

			// notify controller task
			xTaskNotify(htsk_ctrl, *msg_val, eSetValueWithOverwrite);

			// reset send boolean
			send_msg = false;
		}

		// sampling delay
		vTaskDelay(xDelay);

	};

};
