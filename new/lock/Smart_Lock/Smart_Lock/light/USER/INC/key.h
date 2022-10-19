#ifndef __KEY_H
#define __KEY_H
#include "stm32f4xx.h"
#include "led.h"
void Key_Init(void);
u8 Key_Scan(void);

#define KEY1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#endif 
