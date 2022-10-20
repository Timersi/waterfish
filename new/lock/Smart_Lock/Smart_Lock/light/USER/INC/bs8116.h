#ifndef __BS8116_H
#define __BS8116_H
#include "stm32f4xx.h"
#include "tim.h"
#include "stdio.h"

#define BS8116_SCL_H GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define BS8116_SCL_L GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define BS8116_SDA_H GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define BS8116_SDA_L GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define BS8116_SDA (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
#define BS8116_SCL (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6))
#define BS8116_IRQ (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))

#define BS8116_DELAYTIME 5
#define BS8116_WRITEADDRESS  0XA0
#define BS8116_READADDRESS   0XA1 

typedef enum{
	BS8166_NO_ERROR=0,
	BS8166_WRITE_ERROR,
	BS8166_READ_ADDR_ERROR,
}BS8166_ErrorStatus;

void BS8116_I2C_Pin_Init(void);
void BS8116_I2C_Start(u8 nus);
void BS8116_I2C_Stop(u8 nus);
u8 BS8116_I2C_Sent_Byte(u8 nus, u8 data);
u8 BS8116_I2C_Recieve_Byte(u8 ask,u8 nus);
u8 Bs8116_ReadKey(void);

#endif 
