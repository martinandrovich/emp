#pragma once

#include <string>
#include <functional>

#include "uart.h"
#include "cli.h"

//// Defines //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SYSTEM_VERSION "1.0.0"

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace sys
{
	// public methods

	void connect(std::string com_port);

	void write_byte(std::string byte);
	void write_array(std::string args);

	void echo();
	void get_time();
	void set_time(std::string args);
}
