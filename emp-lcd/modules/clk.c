/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	exm.c
* MODULENAME.:	EXAMPLE
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "clk.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static void 		CLK_init(void);
static void 		CLK_operate(void);
static void 		CLK_adjust(CLK_ADJ_DIR dir);
static void 		CLK_set_time(TP_UNIT unit, uint8_t value);

/****************************   Class Struct   *****************************/

struct CLOCK_CLASS clk =
{
	.mode 			= CLK_MODE_ADJ,
	.adjust_dir		= CLK_ADJ_INC,

	.init			= &CLK_init,
	.operate		= &CLK_operate,
	.adjust			= &CLK_adjust,
	.set_time		= &CLK_set_time
};

/*****************************   Functions   *******************************/

static void CLK_init(void)
{
	// init TIMEPOINT
	clk.tp_time		= tp.new();

	// set time to 12:00
	clk.set_time(h, 12);
	clk.set_time(m, 0);

	// set callback
	clk.callback = NULL;
}

static void CLK_operate(void)
{
	switch (clk.mode)
	{
		case CLK_MODE_RUN:
			break;

		case CLK_MODE_ADJ:
			break;
	}
}

static void CLK_adjust(CLK_ADJ_DIR dir)
{
	tp.inc(clk.tp_time, clk.adjust_dir, m);
}

/****************************** End Of Module ******************************/
