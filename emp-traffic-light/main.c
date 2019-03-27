#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver.h"
#include "tp.h"
#include "btn.h"
#include "led.h"
#include "ledctrl.h"

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

	// init LED instance in ACTIVE LOW
	LED* led1 = led.new();
	led1->mode = ACTIVE_LOW;

	led.set_color(led1, (RGB){0, 1, 0});
	led.set_state(led1, 1);

	// init BUTTON instance (SW1)
	BUTTON* btn_sw1 = btn.new(SW1);

	// init LED_CONTROLLER instance
	LED_CONTROLLER* ctrl1 = ledctrl.new();

	// set single press callback
	btn.set_callback(btn_sw1, SINGLE_PRESS, LAMBDA(void _(void)
		{
			ledctrl.set_mode(ctrl1, NORWEGIAN);
		}
	));

	// set double press callback
	btn.set_callback(btn_sw1, DOUBLE_PRESS, LAMBDA(void _(void)
		{
			ledctrl.set_mode(ctrl1, EMERGENCY);
		}
	 ));

	// set long press callback
	btn.set_callback(btn_sw1, LONG_PRESS, LAMBDA(void _(void)
		{
			ledctrl.set_mode(ctrl1, NORMAL);
		}
	));

	// super-loop
	for(;;)
	{
		btn.controller(btn_sw1);
		ledctrl.operate(ctrl1, led1);
	}

	return 0;
}
