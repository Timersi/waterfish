#ifndef __KEY_h
#define __KEY_h
#include "stm32f10x.h"
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);



#define KEYA  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����A
#define KEYB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����B
#define KEYC  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����C
#define KEYD  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ����D
#define KEYE  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����E
#define KEYF  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����F




#define KEYA_PRES 	1	//KEYA����
#define KEYB_PRES 	2	//KEYB����
#define KEYC_PRES	  3	//KEYC����
#define KEYD_PRES   4	//KEYD����
#define KEYE_PRES   5	//KEYD����
#define KEYF_PRES   6	//KEYD����



#endif

