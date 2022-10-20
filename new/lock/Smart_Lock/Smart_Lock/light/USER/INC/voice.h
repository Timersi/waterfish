#ifndef __VOICE_H
#define __VOICE_H
#include "stm32f4xx.h"
#include "tim.h"

#define VOICE_SCL_H GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define VOICE_SCL_L GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define VOICE_BUSY  GPIO_ReadInputDataBit (GPIOC, GPIO_Pin_4)

void Voice_Init(void);
void Voice_Output(u8 data);

#endif 
