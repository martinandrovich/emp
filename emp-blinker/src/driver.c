#include "driver.h"

// Missing definitions in tm4c123gh6pm.h file
#define NVIC_INT_CTRL_PEND_SYST   0x04000000  // Pend a systick int
#define NVIC_INT_CTRL_UNPEND_SYST 0x02000000  // Unpend a systick int
#define SYSTICK_PRIORITY    0xE

static BOOLEAN irq_status;

void __enable_irq(void)
{
    asm volatile
    (
        "CPSIE i"
    );

    irq_status = 1;
}

void __disable_irq(void)
{
    asm volatile
    (
        "CPSID i"
    );

    irq_status = 0;
}

BOOLEAN get_irq(void)
{
    return (BOOLEAN)irq_status;
}

static int set_systick_reload_value(INT32U tick_period)
{
    return tick_period * 16000;
}

void sys_tick_init(INT32U tick_period)
{
    //
    // Note flag is cleared by reading the STRCTL or writing CTCURRENT Register
    NVIC_ST_CTRL_R &=  ~(NVIC_ST_CTRL_ENABLE);
    // calculate (200 ms / 62.5 ns)-1 = 0x30D3FF
    // tick_period = SYSTICK_RELOAD_VALUE 3199999 // 200 mS
    NVIC_ST_CURRENT_R = set_systick_reload_value(tick_period);
    //
    // Set Reload value, Systick reload register
    NVIC_ST_RELOAD_R = set_systick_reload_value(tick_period);
    //
    // On a write, removes the pending state from the SysTick exception s. 159
    NVIC_INT_CTRL_R |= NVIC_INT_CTRL_UNPEND_SYST;
    //
    // First Clear Then Set - Low priority, s. 171
    NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M);
    NVIC_SYS_PRI3_R |= (NVIC_SYS_PRI3_TICK_M);
    //
    // Enable systick interrupt
    NVIC_ST_CTRL_R |= (NVIC_ST_CTRL_INTEN) | (NVIC_ST_CTRL_CLK_SRC);
    //
    // Enable and start timer
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}
