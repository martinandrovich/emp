/****************************************************************************
* University of Southern Denmark
* RB-PRO4 F19
*
* FILENAME...:	chksum.h
* MODULENAME.:	CHECKSUM
*
* For an API and DESCRIPTION, please refer to the  module
* specification file (.h-file).
*
****************************************************************************/

/***************************** Include files *******************************/

#include <stdint.h>
#include <stdbool.h>

#include "chksum.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/************************  Function Declarations ***************************/

static uint8_t		CHECKSUM_gen_4bit(uint16_t data, uint8_t num_of_nibbles);
static bool 		CHECKSUM_val_4bit(uint16_t data, uint8_t num_of_nibbles, uint8_t checksum);

static uint8_t		CHECKSUM_gen_8bit(uint8_t* data_array, uint8_t size);
static bool 		CHECKSUM_val_8bit(uint8_t* data_array, uint8_t size, uint8_t checksum);

static uint8_t		_CHECKSUM_extract_nibble(uint16_t data, uint8_t pos);
static uint8_t 		_CHECKSUM_ror_nibble(uint8_t data);
static uint8_t 		_CHECKSUM_rol_nibble(uint8_t data);
static uint8_t 		_CHECKSUM_ror_byte(uint8_t data);
static uint8_t 		_CHECKSUM_rol_byte(uint8_t data);

/****************************   Class Struct   *****************************/

const struct CHECKSUM_CLASS chksum =
{
	.gen_4bit		= &CHECKSUM_gen_4bit,
	.val_4bit		= &CHECKSUM_val_4bit,
	.gen_8bit		= &CHECKSUM_gen_8bit,
	.val_8bit		= &CHECKSUM_val_8bit
};

/*****************************   Functions   *******************************/

static uint8_t CHECKSUM_gen_4bit(uint16_t data, uint8_t num_of_nibbles)
{
	// https://en.wikipedia.org/wiki/BSD_checksum

	// variables
	uint8_t checksum = 0;

	// algorithm starting from MSB
	for (int i = num_of_nibbles; i >= 0; --i)
	{
		checksum = _CHECKSUM_ror_nibble(checksum);
		checksum = checksum + _CHECKSUM_extract_nibble(data, i);
		checksum = checksum & 0xF;
	}

	return checksum;
}

static bool CHECKSUM_val_4bit(uint16_t data, uint8_t num_of_nibbles, uint8_t checksum)
{
	return (chksum.gen_4bit(data, num_of_nibbles) == checksum);
}

static uint8_t CHECKSUM_gen_8bit(uint8_t* data_array, uint8_t size)
{
	// https://en.wikipedia.org/wiki/BSD_checksum

	// variables
	uint8_t checksum = 0;

	// algorithm starting from MSB
	for (int i = 0; i < size; i++)
	{
		checksum = _CHECKSUM_ror_byte(checksum);
		checksum = (uint8_t)checksum + data_array[i];
		checksum = (uint8_t)checksum & 0xFF;
	}

	return checksum;
}

static bool CHECKSUM_val_8bit(uint8_t* data_array, uint8_t size, uint8_t checksum)
{
	return (chksum.gen_8bit(data_array, size) == checksum);
}

static uint8_t _CHECKSUM_extract_nibble(uint16_t data, uint8_t pos)
{
	// extract nibble at position n
	// test = 0101 1111 1010 0000 -> extract_nibble(test, 2) = 1111

	return ((data & (0xF << pos * 4)) >> (pos * 4));
}

static uint8_t _CHECKSUM_ror_nibble(uint8_t data)
{
	return ((data >> 1) | (data << 3) & 0xF);
}

static uint8_t _CHECKSUM_rol_nibble(uint8_t data)
{
	return ((data << 1) | (data >> 3) & 0xF);
}

static uint8_t _CHECKSUM_ror_byte(uint8_t data)
{
	return ((data >> 1) | (data << 7) & 0xFF);
}

static uint8_t _CHECKSUM_rol_byte(uint8_t data)
{
	return ((data << 1) | (data >> 7) & 0xFF);
}

/****************************** End Of Module ******************************/
