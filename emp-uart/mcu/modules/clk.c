/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	clk.h
* MODULENAME.:	CLOCK
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>
#include <assert.h>

#include "clk.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static void 		CLK_init(void (*callback)(const uint8_t*));
static void 		CLK_toggle_mode(void);
static void 		CLK_toggle_dir(void);
static void 		CLK_operate(void);
static void 		CLK_adjust(void);
static void 		CLK_set_time(uint8_t hh, uint8_t mm);
static uint8_t*		CLK_get_timestr(void);

static void 		_CLK_print_time();

/****************************   Class Struct   *****************************/

struct CLOCK_CLASS clk =
{
	.mode 			= CLK_MODE_ADJ,
	.adjust_dir		= CLK_ADJ_INC,
	.tp_time		= NULL,
	.tp_counter     = NULL,
	.callback		= NULL,

	.init			= &CLK_init,

	.toggle_mode 	= &CLK_toggle_mode,
	.toggle_dir 	= &CLK_toggle_dir,

	.operate		= &CLK_operate,
	.adjust			= &CLK_adjust,
	.set_time		= &CLK_set_time,
	.get_timestr	= &CLK_get_timestr
};

/*****************************   Functions   *******************************/

static void CLK_init(void (*callback)(const uint8_t*))
{
	// init TIMEPOINT
	clk.tp_time		= tp.new();
	clk.tp_counter	= tp.new();

	// set callback
	clk.callback = callback;

	// set counter timepoint
	tp.set(clk.tp_counter, tp.now());

	// print time
	_CLK_print_time();
}

static void CLK_toggle_mode(void)
{
	clk.mode = (clk.mode == CLK_MODE_RUN) ? CLK_MODE_ADJ : CLK_MODE_RUN;
}

static void CLK_toggle_dir(void)
{
	clk.adjust_dir = (clk.adjust_dir == CLK_ADJ_INC) ? CLK_ADJ_DEC : CLK_ADJ_INC;
}

static void CLK_operate(void)
{
	// check if clock is set to run
	if (clk.mode != CLK_MODE_RUN) { return; }

	// check if second has passed
	if (tp.delta_now(clk.tp_counter, s) < 1) { return; }

	// reset counter timepoint
	tp.set(clk.tp_counter, tp.now());

	// increment time timepoint
	tp.inc(clk.tp_time, 1, s);

	// print time
	_CLK_print_time();
}

static void CLK_adjust(void)
{
	// check if clock is set to adjust mode
	if (clk.mode != CLK_MODE_ADJ) { return; }

	// read from time_array
	int8_t hh = clk.tp_time->time_array[h];
	int8_t mm = clk.tp_time->time_array[m];

	// calculate time (rollover arithmetic)
	switch (clk.adjust_dir)
	{
		case CLK_ADJ_INC:

			mm == 59 ? hh = (hh + 1) % 24, mm = 0 : mm++;
			break;

		case CLK_ADJ_DEC:

			mm ==  0 ? hh--, mm = 59 : mm--;
			hh == -1 ? hh = 23 : hh;
			break;
	}

	// reset and write to time_array
	tp.reset(clk.tp_time);
	clk.tp_time->time_array[h] = hh;
	clk.tp_time->time_array[m] = mm;

	// print time
	_CLK_print_time();
}

static void CLK_set_time(uint8_t hh, uint8_t mm)
{
	// reset and write to time_array
	tp.reset(clk.tp_time);
	clk.tp_time->time_array[h] = hh % 24;
	clk.tp_time->time_array[m] = mm % 60;

	// print time
	_CLK_print_time();
}

static uint8_t* CLK_get_timestr(void)
{
	static char time_str[6] = "00:00";

	// write string to buffer from timepoint
	// flashing ':' every odd second
	sprintf(time_str, "%02u%c%02u",
		clk.tp_time->time_array[h],
		clk.tp_time->time_array[s] % 2 ? ':' : ' ',
		clk.tp_time->time_array[m]
	);

	// return string
	return (uint8_t*)time_str;
}

static void	_CLK_print_time()
{
	// check callback
	assert(clk.callback != NULL);

	// get time string
	const uint8_t* time_str = clk.get_timestr();

	// output to display (callback)
	clk.callback(time_str);
}

/****************************** End Of Module ******************************/
