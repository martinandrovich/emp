#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "driver.h"
#include "os.h"
#include "tp.h"

#include "led.h"

/*****************************    Defines    *******************************/

#define LAMBDA(c_) ({ c_ _; })

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

LED*	main_led;

/************************   Interrupt Handlers   ***************************/

void ISR_SYSTICK(void)
{
	tp.systick();
}

/*******************************   Tasks   *********************************/

uint32_t	task1_stack[STACK_SIZE];
uint32_t*	task1_sp = &task1_stack[40];
void task1(void)
{
	while(true)
	{
		for (int i = 0; i < 1000000; i++);
		led.set_color(main_led, (RGB){0, 0, 1});
	}
}

uint32_t	task2_stack[STACK_SIZE];
uint32_t*	task2_sp = &task2_stack[40];
void task2(void)
{
	while(true)
	{
		for (int i = 0; i < 1000000; i++);
		led.set_color(main_led, (RGB){0, 1, 0});
	}
}

/*******************************   Main   **********************************/

int main(void)
{

	/** Init Stacks ********************************************************/

	// task1 stack
	*(--task1_sp) = (1 << 24);						// xSPR
	*(--task1_sp) = (uint32_t)&task1;				// PC
	*(--task1_sp) = 0x0000000E;						// LR
	*(--task1_sp) = 0x0000000C;						// R12
	*(--task1_sp) = 0x00000003;						// R3
	*(--task1_sp) = 0x00000002;						// R2
	*(--task1_sp) = 0x00000001;						// R1
	*(--task1_sp) = 0x00000000;						// R0

	// task1 stack
	*(--task2_sp) = (1 << 24);						// xSPR
	*(--task2_sp) = (uint32_t)&task2;				// PC
	*(--task2_sp) = 0x0000000E;						// LR
	*(--task2_sp) = 0x0000000C;						// R12
	*(--task2_sp) = 0x00000003;						// R3
	*(--task2_sp) = 0x00000002;						// R2
	*(--task2_sp) = 0x00000001;						// R1
	*(--task2_sp) = 0x00000000;						// R0

	/** Init Hardware ******************************************************/

	main_led = led.new();

	led.set_color(main_led, (RGB){1, 0, 0});
	led.set_state(main_led, 1);

	/** Main ***************************************************************/

	while(true)
	{
		// let the scheduler do the work
	}

	return 0;
}
