#ifndef __LED_H
#define __LED_H
#include "stm32f4xx.h"


#define LED1  GPIO_ToggleBits(GPIOB,GPIO_Pin_8)
#define LED1_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_8)
#define LED1_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define LED2_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define LED2_OFF  GPIO_SetBits(GPIOB,GPIO_Pin_9)
void LED_Init(void);
void breath_led(void);
#endif 
