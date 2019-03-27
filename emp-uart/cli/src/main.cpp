#include "system.h"
#include "cli.h"

// All essential functionality is located in system.cpp, where main.cpp simply acts
// as an entry point and overview of the system.

// Changelog can be found here:
// https://github.com/rb-pro4-f19/CLI/blob/master/CHANGELOG.md

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
			{ "spi",			[](std::string args) { return; }},
			/// ...
		})},

		{ "set",			[](std::string args) {}, subcmd{
			{ "mode",			[](std::string args) { return; }, subcmd{
				{ "standby",		[](std::string args) { return; } },
				{ "manual",			[](std::string args) { return; } },
				{ "auto",			[](std::string args) { return; } },
				/// ...
			})},
			{ "pwm",			[](std::string args) { sys::set_pwm(args); }},
			/// ...
		})},

		{ "get",			[](std::string args) {}, subcmd{
			{ "enc",			[](std::string args) { sys::get_enc(args); }},
			/// ...
		})},

		/// ...
	});

	// log system information
	cli::log_reset("Pan-Tilt System " + std::string(SYSTEM_VERSION));

	// main loop
	while (true)
	{
		cli::get_input();
	}

	// exit
	return 0;
}
