/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	clk.h
* MODULENAME.:	CLOCK
* API........:	n/a
* VERSION....:	1.0.0
*
* DESCRIPTION:	An example module. This might have a lengthy description, in
*				which case, we simply add some tabs.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

#include "tp.h"

/*****************************    Defines    *******************************/

typedef enum CLK_MODE		CLK_MODE;
typedef enum CLK_ADJ_DIR	CLK_ADJ_DIR;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*****************************    Constructs   *****************************/

enum CLK_MODE
{
	CLK_MODE_RUN	= 0,
	CLK_MODE_ADJ	= 1
};

enum CLK_ADJ_DIR
{
	CLK_ADJ_INC 	=  1,
	CLK_ADJ_DEC		= -1
};

/*************************    Class Functions    ***************************/

extern struct CLOCK_CLASS
{
	CLK_MODE	mode;
	CLK_ADJ_DIR adjust_dir;
	TIMEPOINT*	tp_time;

	void 		(*callback);

	void 		(* const init)(void);
	void 		(* const operate)(void);
	void 		(* const adjust)(CLK_ADJ_DIR dir);
	void 		(* const set_time)(TP_UNIT unit, uint8_t value);
} clk;

/****************************** End Of Module ******************************/
