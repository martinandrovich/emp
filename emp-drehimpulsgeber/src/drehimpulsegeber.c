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

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

extern QueueHandle_t queue_lcd;
extern SemaphoreHandle_t semph_lcd;
extern TaskHandle_t lcd_write_handle;

/************************  Function Declarations ***************************/

static void DREHIMPULSEGEBER_init();
static void DREHIMPULSEGEBER_task(void *pm);

/*************************    Class Functions    ***************************/

const struct DREHIMPULSEGEBER_CLASS drehimpulsegeber =
{

    .init       = &DREHIMPULSEGEBER_init,
    .task       = &DREHIMPULSEGEBER_task

};

/*****************************   Functions   *******************************/


static void DREHIMPULSEGEBER_init()
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
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


static void DREHIMPULSEGEBER_task(void *pm)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{

    volatile int8_t prev_AB = ( CHECK_BIT(GPIO_PORTA_DATA_R, 5) >> 4) + ( CHECK_BIT(GPIO_PORTA_DATA_R, 6 ) << 6 );

    volatile int8_t AB [2] = { prev_AB , prev_AB };

    volatile uint8_t reset_button = 0;

    static int8_t dir = 0;

    static int16_t pos = 0;

    static uint8_t send_msg = 0;

    DIGISWITCH_MSG msg = { 0 , 0 , 0 , 0 };

    const TickType_t xDelay = pdMS_TO_TICKS(1);

    //read initial state

    for (;;)
    {

        //read reset button
        reset_button = ( CHECK_BIT(GPIO_PORTA_DATA_R, 7) >> 7);

        if ( !reset_button )
        {
            send_msg = 1;
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

                    if ( AB[1] == 1 )
                    {
                        dir = -1;
                    }
                    else
                    {
                        dir = 1;
                    }

                    break;


                case 1:

                    if ( AB[1] == 3 )
                    {
                        dir = -1;
                    }
                    else
                    {
                        dir = 1;
                    }

                    break;

                case 2:

                    if ( AB[1] == 0 )
                    {
                        dir = -1;
                    }
                    else
                    {
                        dir = 1;
                    }

                    break;

                case 3:

                    if ( AB[1] == 2 )
                    {
                        dir = -1;
                    }
                    else
                    {
                        dir = 1;
                    }

                    break;

            };

            send_msg = 1;
            prev_AB = AB[1];
            pos += (int16_t) dir;

        };

        if ( send_msg == 1 )
        {

            msg.dir = dir;
            msg.revol = 0;
            msg.rst = ~reset_button;

            if (!reset_button)
            {
                msg.pos = 0;
                pos = 0;
            }
            else
            {
                msg.pos = pos;
            }

            send_msg = 0;

            uint32_t * i_msg = (uint32_t *)&msg;
            xTaskNotify(lcd_write_handle, (*i_msg) , eSetValueWithOverwrite);
        }

        vTaskDelay( xDelay );

    };

};
