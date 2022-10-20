#ifndef __smartcar_h
#define __smartcar_h

void smartcar_Init(void);

#define INA1_0 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
#define INA1_1 GPIO_SetBits(GPIOB,GPIO_Pin_6);

#define INA2_0 GPIO_ResetBits(GPIOB,GPIO_Pin_5);
#define INA2_1 GPIO_SetBits(GPIOB,GPIO_Pin_5);

#define INA3_0 GPIO_ResetBits(GPIOB,GPIO_Pin_8);
#define INA3_1 GPIO_SetBits(GPIOB,GPIO_Pin_8);

#define INA4_0 GPIO_ResetBits(GPIOB,GPIO_Pin_7);
#define INA4_1 GPIO_SetBits(GPIOB,GPIO_Pin_7);

#define INB1_0 GPIO_ResetBits(GPIOB,GPIO_Pin_15);
#define INB1_1 GPIO_SetBits(GPIOB,GPIO_Pin_15);

#define INB2_0 GPIO_ResetBits(GPIOB,GPIO_Pin_14);
#define INB2_1 GPIO_SetBits(GPIOB,GPIO_Pin_14);

#define INB3_0 GPIO_ResetBits(GPIOB,GPIO_Pin_13);
#define INB3_1 GPIO_SetBits(GPIOB,GPIO_Pin_13);

#define INB4_0 GPIO_ResetBits(GPIOB,GPIO_Pin_12);
#define INB4_1 GPIO_SetBits(GPIOB,GPIO_Pin_12);

#define KEYA  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//��ȡ����A
#define KEYB  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)//��ȡ����B
#define KEYC  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����C
#define KEYD  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)//��ȡ����D
#define KEYE  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)//��ȡ����E
#define KEYF  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)//��ȡ����F

#endif

