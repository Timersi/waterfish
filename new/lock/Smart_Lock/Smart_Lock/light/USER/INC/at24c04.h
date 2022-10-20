#ifndef __AT24C04_H
#define __AT24C04_H
#include "stm32f4xx.h"
#include "i2c.h"
#include "tim.h"

#define AT24C04_DELAY_TIME 2
#define AT24C04_WRITE_ADDR 0XA0
#define AT24C04_READ_ADDR 0XA1

enum TYPE_ERROR{
	NO_ERROR =0,
	ASK_ERROR,
	WRITE_ADDR_ERROR,
	ADDR_ERROR,
};

void AT24C04_Init(void);
u8 AT24C04_Continue_Read(u8 *data, u8 len, u8 addr);
u8 AT24C04_Page_Wtite(u8 *data, u8 len, u8 addr);
void AT24C04_Auto_Page_Wtite(u8 *data, u8 len, u8 addr);




#endif 


