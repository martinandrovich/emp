#pragma once

#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <windows.h>

extern "C"
{
#include "chksum.h"
}

//// Defines //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define UART_COM_PORT			"COM4"
#define UART_BAUD				CBR_9600
#define UART_BYTESIZE			8
#define UART_ACK_TIMEOUT		3000

#define UART_FRAME_TYPE_BITS	3
#define UART_FRAME_SIZE_BITS	5
#define UART_MAX_PAYLOAD_SIZE	256

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace uart
{
	// public constructs

	struct UART_FRAME
	{
		uint8_t		type : 3; /// should match parse_byte();
		uint8_t		size : 5; /// ^
		std::vector
		<uint8_t>	payload;
		uint8_t		checksum;
	};

	enum UART_FRAME_FIELD
	{
		TYPE,
		SIZE,
		PAYLOAD,
		CHECKUM
	};

	enum UART_FRAME_TYPE
	{
		CONNECT		= 0x00,
		RAW			= 0x01,
		GET			= 0x02,
		SET			= 0x03,
		ACK			= 0x04,
		RESPONSE	= 0x05,
		STREAM		= 0x06,
		MSG			= 0x07
	};

	// public methods

	void		connect(const char* com_port = UART_COM_PORT);
	void		disconnect();

	bool		send(UART_FRAME_TYPE type, std::vector<uint8_t>& data);
	bool		request(uint8_t* buffer);

	void		print_frame(UART_FRAME& frame);

	// public members

	namespace reciever
	{
		extern std::function<void(UART_FRAME frame)> callback_ack;
		extern std::function<void(UART_FRAME frame)> callback_msg;
		extern std::function<void(UART_FRAME frame)> callback_stm;
	}
}