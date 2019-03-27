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

	// disable interrupts
	__disable_irq();

	// init SYSTICK
	tp.init_systick(SYSTICK_DUR_MS, ms);
	sys_tick_init(SYSTICK_DUR_MS);

	// enable interrupts
	__enable_irq();

	// init BUTTON instance (SW1)
	BUTTON* btn_sw1 = btn.new(SW1);

	// set single press callback
	btn.set_callback(btn_sw1, SINGLE_PRESS, LAMBDA(void _(void)
		{
			clk.adjust();
		}
	));

	// set double press callback
	btn.set_callback(btn_sw1, DOUBLE_PRESS, LAMBDA(void _(void)
		{
			clk.toggle_dir();
		}
	 ));

	// set long press callback
	btn.set_callback(btn_sw1, LONG_PRESS, LAMBDA(void _(void)
		{
			clk.toggle_mode();
		}
	));

	// init LCD display
	lcd.init();

	// init clock with callback to LCD display
	clk.init(lcd.write_string_s);

	// set clock to RUN mode
	clk.toggle_mode();

	// super-loop
	for(;;)
	{
		btn.controller(btn_sw1);
		clk.operate();
	}

	return 0;
}
