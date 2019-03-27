#include "system.h"
#include "cli.h"

// All essential functionality is located in system.cpp, where main.cpp simply acts
// as an entry point and overview of the system.

// main function
int main()
{
	// initialize CLI command table
	cli::init(
	{
		{ "connect",		[](std::string args) { sys::connect(args); } },
		{ "cls",			[](std::string args) { cli::log_reset(); } },
		{ "exit",			[](std::string args) { exit(0); } },

		{ "echo",			[](std::string args) { sys::echo(); } },

		{ "write",			[](std::string args) {}, subcmd{	
			{ "byte",			[](std::string args) { sys::write_byte(args); }},
			{ "array",			[](std::string args) { return; }},
			/// ...
		})},

		{ "set",			[](std::string args) {}, subcmd{
			{ "time",			[](std::string args) { sys::set_time(args); }},
			/// ...
		})},

		{ "get",			[](std::string args) {}, subcmd{
			{ "time",			[](std::string args) { sys::get_time(); }},
			/// ...
		})},

		/// ...
	});

	// log system information
	cli::log_reset("UART System " + std::string(SYSTEM_VERSION));

	// main loop
	while (true)
	{
		cli::get_input();
	}

	// exit
	return 0;
}
