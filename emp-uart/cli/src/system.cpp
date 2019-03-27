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

void sys::write_spi(std::string args)
{
	// cmd: "write spi 1 17" sends the byte 17 to the 4-bit address 1
	// requires logic on the MCU
}

void sys::echo()
{
	// create payload vector and transmit data
	std::vector<uint8_t> tx_data = { 0x00 };
	uart::send(uart::UART_FRAME_TYPE::GET, tx_data);
}

void sys::set_pwm(std::string& args)
{
	// split input delimited by spaces into vector of strings
	auto args_vec = cli::split_str(args);

	// check that correct num of parameters was passed
	if (!args_vec.size() == 2) { return; }

	// construct variables to be correctly parsed by MCU & FPGA
	// MOT0 = 0x01 & MOT1 = 0x02
	// e.g. set pwm 1 200 = set motor MOT1 (0x02) to pwm_val 200
	uint8_t motor	= std::stoi(args_vec[0]) + 1;
	uint8_t pwm_val	= std::stoi(args_vec[1]);
	uint8_t uart_id	= 0x01;

	// construct and send frame
	std::vector<uint8_t> tx_data = { uart_id, motor, pwm_val };
	uart::send(uart::UART_FRAME_TYPE::SET, tx_data);
}

void sys::get_enc(std::string& args)
{
	// split input delimited by spaces into vector of strings
	auto args_vec = cli::split_str(args);

	// check that correct num of parameters was passed
	if (!args_vec.size() == 1) { return; }

	// construct variables to be correctly parsed by MCU & FPGA
	// ENC0 = 0x03 & ENC1 = 0x04
	// e.g. get enc 0 = read encoder ENC0 (0x03)
	uint8_t encoder	= std::stoi(args_vec[0]) + 3;
	uint8_t uart_id = 0x01;

	// construct and send frame
	std::vector<uint8_t> tx_data = { uart_id, encoder };
	uart::send(uart::UART_FRAME_TYPE::GET, tx_data);
}