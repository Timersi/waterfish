#ifndef __MG200_H
#define __MG200_H
#include "stm32f4xx.h"
#include "lcd.h"
#include <stdio.h>
#define MG200_ON GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define MG200_OFF GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define MG200_DETECT (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))

typedef struct {
	u8 buf[8];
	u16 len;
	u8 flag;     //全部传输完成的标志位
}USART6_REC;


void MG200_Init(void);
u8 ReqCaptureAndExtract(u8 parameter);
uint8_t Registered_fingerprints(void);
u8 Matchln(void);
u8 GetUserNum(void);
#endif 
