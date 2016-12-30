#ifndef __SYSTICK_H
#define __SYSTICK_H


#include "stm32f10x.h"


void SysTick_Init(void);
void Delay_10us(__IO u32 nTime);         // µ¥Î»1us

#define   Delay_ms(x)		Delay_10us(100*x)

#endif /* __SYSTICK_H */
