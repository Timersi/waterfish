#ifndef __DIGITAL_h
#define __DIGITAL_h
#include "stm32f10x.h"
void digival_value_Init(void);//IO��ʼ��
u8 digival_value_Scan(void);



#define DIGVAL  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��������ȡ





#endif

