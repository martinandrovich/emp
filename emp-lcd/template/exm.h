/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	exm.h
* MODULENAME.:	EXAMPLE
* API........:	github.com/rb-pro4-f19/MCU/blob/master/README.md
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

/*****************************    Defines    *******************************/

typedef struct  EXAMPLE EXAMPLE;
typedef enum    EXM_TYPE EXM_TYPE;

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

#define	EXM_ARRAY_SIZE 8

/*************************    Class Functions    ***************************/

extern const struct EXAMPLE_CLASS
{
	EXAMPLE*	(*new)(EXM_TYPE type, int32_t init_val);
	void		(*del)(EXAMPLE* this);

	void 		(*some_func)(EXAMPLE* this);
} exm;

/*****************************    Constructs   *****************************/

enum EXM_TYPE
{
	NORMAL,
	SYSTEM
};

struct EXAMPLE
{
	// public
	uint64_t	var;
	uint64_t	array[EXM_ARRAY_SIZE];
	bool		is_set;
	EXM_TYPE	type;

	// private
	int32_t		_secret;
};

/****************************** End Of Module ******************************/
