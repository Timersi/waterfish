#ifndef __W25Q64_H
#define __W25Q64_H
#include "stm32f4xx.h"
#include <stdio.h>
#include "spi.h"


typedef struct{
	u8 Font_Sta;
	u8 Font_Flag;
}Font_struct;
extern Font_struct fs;
void Read_W25q64ID(void);
void Sector_Erase(u32 addr);
void Page_Program(u32 addr,u8 *data,u16 len);
void W25Q64_ReadData(u32 addr,u8 *data,u8 len);
void W25q64_Auto_Page_Wtite(u8 *data, u16 len, u32 addr);
void Block_Erase(u32 addr);
void Font_Update(void);

#endif

