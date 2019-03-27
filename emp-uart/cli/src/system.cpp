#include "system.h"

//// Private Declarations /////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace sys
{

}

//// Method Definitions ///////////////////////////////////////////////////////////////////////////////////////////////////////////

void sys::connect(std::string com_port)
{	
	// MSG callback (by-value)
	uart::reciever::callback_msg = [=](uart::UART_FRAME frame)
	{
		auto msg_str = std::string(frame.payload.begin(), frame.payload.end());
		cli::msgbox(msg_str, "Message from MCU");
		//cli::log_insert("MESSAGE: " + std::string(frame.payload.begin(), frame.payload.end()));
	};

	// STREAM callback
	uart::reciever::callback_stm = nullptr;

	// connect UART to specified or default COM port
	(com_port == "") ? uart::connect() : uart::connect(com_port.c_str());
}

void sys::write_byte(std::string byte)
{
	if (byte == "")
	{
		printf("WRITE ERROR: No write data specified.\n");
		return;
	}

	// create payload vector and transmit data
	std::vector<uint8_t> tx_data = { (uint8_t)std::stoi(byte) };
	uart::send(uart::UART_FRAME_TYPE::RAW, tx_data);
}

void sys::write_array(std::string args)
{
	// cmd: "write array 1 222 19 45" sends the bytes 1, 222, 19, 45
	// parsed from args string
}

void sys::echo()
{
	// create payload vector and transmit data
	std::vector<uint8_t> tx_data = { 0x00 };
	uart::send(uart::UART_FRAME_TYPE::GET, tx_data);
}

void sys::get_time()
{
	// create payload vector and transmit data
	std::vector<uint8_t> tx_data = { 0x01 };
	uart::send(uart::UART_FRAME_TYPE::GET, tx_data);
}

void sys::set_time(std::string args)
{

	// split args string into vector
	auto argsvec = cli::split_str(args);

	// check size
	if (argsvec.size() != 2) { return; }

	// create payload vector and transmit data
	std::vector<uint8_t> tx_data = { 0x01, (uint8_t)std::stoi(argsvec[0]), (uint8_t)std::stoi(argsvec[1]) };
	uart::send(uart::UART_FRAME_TYPE::SET, tx_data);
}