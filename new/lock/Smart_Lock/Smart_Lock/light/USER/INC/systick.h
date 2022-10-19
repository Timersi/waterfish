#ifndef __SYSTICK_H
#define __SYSTICK_H
#include "stm32f4xx.h"
#include <stdio.h>
void Exti_Init(void);
void SysTick_Init(u16 ms);
void SysTick_delay_ms(u16 ms);
void SysTick_delay_more_ms(u16 ms);
#endif

