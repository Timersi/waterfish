#ifndef __KEY_h
#define __KEY_h
#include "stm32f10x.h"
void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);



#define KEYA  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键A
#define KEYB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//读取按键B
#define KEYC  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键C
#define KEYD  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//读取按键D
#define KEYE  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//读取按键E
#define KEYF  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//读取按键F




#define KEYA_PRES 	1	//KEYA按下
#define KEYB_PRES 	2	//KEYB按下
#define KEYC_PRES	  3	//KEYC按下
#define KEYD_PRES   4	//KEYD按下
#define KEYE_PRES   5	//KEYD按下
#define KEYF_PRES   6	//KEYD按下



#endif

