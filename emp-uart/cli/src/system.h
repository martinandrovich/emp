#pragma once

#include <string>
#include <functional>

#include "uart.h"
#include "cli.h"

//// Defines //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SYSTEM_VERSION "1.5.0"

//// Public Declarations [Interface] //////////////////////////////////////////////////////////////////////////////////////////////

namespace sys
{

	// constructs

	enum class SYSTEM_MODE
	{
		STANDBY,
		IDLE,
		RUN
	};

	// public methods

	void connect(std::string com_port);

	void write_byte(std::string byte);
	void write_array(std::string args);
	void write_spi(std::string args);

	void echo();
	void set_pwm(std::string& args);
	void get_enc(std::string& args);
}
