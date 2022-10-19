#ifndef __SPI_H
#define __SPI_H
#include "stm32f4xx.h"



#define SPI1_CS_H GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define SPI1_CS_L GPIO_ResetBits(GPIOA,GPIO_Pin_4)

void W25q64_Init(void);


u8 Spi_ChangeData(u8 data);


#endif 
