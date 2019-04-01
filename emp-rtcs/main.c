#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver.h"
#include "tp.h"
#include "btn.h"
#include "led.h"
#include "ledctrl.h"
#include "clk.h"
#include "lcd.h"
#include "cli.h"

/*****************************    Defines    *******************************/

#define LAMBDA(c_) ({ c_ _; })

/*****************************   Constants   *******************************/

#define SYSTICK_DUR_MS 1

/*****************************   Variables   *******************************/

/************************    External Methods    ***************************/

/************************   Interrupt Handlers   ***************************/

void ISR_SYSTICK(void)
{
	tp.systick();
}

/*******************************   Main   **********************************/

int main(void)
{

	/** Initialization *****************************************************/

	// disable interrupts
	__disable_irq();

	// init SYSTICK
	tp.init_systick(SYSTICK_DUR_MS, ms);
	sys_tick_init(SYSTICK_DUR_MS);

	// init UART and assign to CLI
	UART* uart1 = uart.new(BAUD_9600);
	cli.uart_module = uart1;

	// init BUTTON instance (SW1)
	BUTTON* btn_sw1 = btn.new(SW1);

	// init LCD display
	lcd.init();

	// init clock with callback to LCD display
	clk.init(lcd.write_string_s);

	// set clock to RUN mode
	clk.toggle_mode();

	/** Callbacks **********************************************************/

	// set SINGLE PRESS callback
	btn.set_callback(btn_sw1, SINGLE_PRESS, LAMBDA(void _(void)
		{ clk.adjust(); }
	));

	// set DOUBLE PRESS callback
	btn.set_callback(btn_sw1, DOUBLE_PRESS, LAMBDA(void _(void)
		{ clk.toggle_dir();	}
	 ));

	// set LONG PRESS callback
	btn.set_callback(btn_sw1, LONG_PRESS, LAMBDA(void _(void)
		{ clk.toggle_mode(); }
	));

	/** Command Table ******************************************************/

	// init commands
	// max 8 types and 8 actions per type (#defined in cli.h)
	CMD_TABLE
	{
		{ UART_GET, {
			{ 0x01, CLI_LAMBDA({ cli.log((char*)clk.get_timestr()); }) }
		}},

		{ UART_SET, {
			{ 0x01, CLI_LAMBDA({ clk.set_time(args[0], args[1]); }) }
		}}
	};

	// enable interrupts
	__enable_irq();

	/** Main ***************************************************************/

	// super-loop
	for(;;)
	{
		cli.check();
		btn.controller(btn_sw1);
		clk.operate();
	}

	return 0;
}
