#ifndef __usart_2_h
#define __usart_2_h
#include "stm32f10x.h"

void usart_2_init(u32 bound);
void USART2_IRQHandler(void);

#endif
