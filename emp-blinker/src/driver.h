#include "emp_type.h"
#include "../tm4c123gh6pm.h"

extern void     __enable_irq(void);
extern void     __disable_irq(void);

extern BOOLEAN  get_irq(void);
extern void     sys_tick_init(INT32U tick_period);
