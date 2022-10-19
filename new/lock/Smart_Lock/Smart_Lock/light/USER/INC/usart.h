#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"
#include <stdio.h>
#include "w25q64.h"
#include "dma.h"

typedef struct {
	u8 buf[2][4096];
	u16 len;
	u8 flag;     //ȫ��������ɵı�־λ
	u8 dma_flag; //һ������������ɵı�־λ
	u32 addr;
	u16 Sector_num;
}String_rec;

extern String_rec Rdata;

void Usart_Init(void);


#endif 



