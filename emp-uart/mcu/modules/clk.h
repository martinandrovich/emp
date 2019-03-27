/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	clk.h
* MODULENAME.:	CLOCK
* API........:	n/a
* VERSION....:	1.0.0
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
	TIMEPOINT*	tp_counter;

	void 		(*callback)(const uint8_t*);

	void 		(* const init)(void (*callback)(const uint8_t*));

	void 		(* const toggle_mode)(void);
	void 		(* const toggle_dir)(void);

	void 		(* const operate)(void);
	void 		(* const adjust)(void);
	void 		(* const set_time)(uint8_t hh, uint8_t mm);
	uint8_t*	(* const get_timestr)(void);
} clk;

/****************************** End Of Module ******************************/
