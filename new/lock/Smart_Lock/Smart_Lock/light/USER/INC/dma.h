#ifndef __DMA_H
#define __DMA_H
#include "stm32f4xx.h"
#include "usart.h"
#include <stdio.h>
#include "w25q64.h"

#define BUFFERSIZE  4096

typedef enum{
	FREE=0,
	DMAIT,
	USARTIDLE,
}FONTFLAG;

void DMA_Transfer_Init(void);
void Dma_FontUpdate(void);


#endif 
