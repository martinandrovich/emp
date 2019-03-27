/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	chksum.h
* MODULENAME.:	CHECKSUM
* API........:	https://goo.gl/cRHMYG
* VERSION....:	1.2.1
*
* DESCRIPTION:	4-bit and 8-bit BSD based checksum on variable length inputs.
*
****************************************************************************/

#pragma once

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>
#include <malloc.h>

/*****************************    Defines    *******************************/

/***********************     External Variables     ************************/

/*****************************   Constants   *******************************/

/*************************    Class Functions    ***************************/
extern const struct CHECKSUM_CLASS
{
	// 4 bit
	uint8_t(*gen_4bit)(uint16_t data, uint8_t num_of_nibbles);
	bool(*val_4bit)(uint16_t data, uint8_t num_of_nibbles, uint8_t checksum);

	// 8 bit
	uint8_t(*gen_8bit)(uint8_t* data_array, uint8_t size);
	bool(*val_8bit)(uint8_t* data_array, uint8_t size, uint8_t checksum);
} chksum;


/****************************** End Of Module ******************************/
