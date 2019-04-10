#include <stdint.h>
#include <stdbool.h>

#include "../tm4c123gh6pm.h"

extern void     __enable_irq(void);
extern bool     __disable_irq(void);

extern bool  	get_irq(void);
extern void     sys_tick_init(uint32_t tick_period);
