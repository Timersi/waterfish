#ifndef __I2C_H
#define __I2C_H
#include "stm32f4xx.h"
#include "tim.h"

#define SCL_H GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define SCL_L GPIO_ResetBits(GPIOA, GPIO_Pin_8)

#define SDA_H GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define SDA_L GPIO_ResetBits(GPIOC, GPIO_Pin_9)

#define SDA GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)


void I2C_Pin_Init(void);
void I2C_Start(u8 nus);
void I2C_Stop(u8 nus);
u8 I2C_Sent_Byte(u8 nus, u8 data);
u8 I2C_Recieve_Byte(u8 ask,u8 nus);


#endif 
