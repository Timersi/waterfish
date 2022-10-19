#ifndef __LCD_H
#define __LCD_H
#include "stm32f4xx.h"
#include "tim.h"
#include "stdio.h"
#include "w25q64.h"

/* 颜色定义开始 */
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
/* 颜色定义结束 */


#define LCD_LED_H GPIO_SetBits(GPIOB, GPIO_Pin_1);
#define LCD_LED_L GPIO_ResetBits(GPIOB, GPIO_Pin_1);

#define LCD_RES_H GPIO_SetBits(GPIOB, GPIO_Pin_10);
#define LCD_RES_L GPIO_ResetBits(GPIOB, GPIO_Pin_10);

#define LCD_CS_H GPIO_SetBits(GPIOB, GPIO_Pin_12);
#define LCD_CS_L GPIO_ResetBits(GPIOB, GPIO_Pin_12);

#define SPI2_SCL_H GPIO_SetBits(GPIOB, GPIO_Pin_13);
#define SPI2_SCL_L GPIO_ResetBits(GPIOB, GPIO_Pin_13);

#define LCD_DC_H GPIO_SetBits(GPIOB, GPIO_Pin_14);
#define LCD_DC_L GPIO_ResetBits(GPIOB, GPIO_Pin_14);

#define LCD_SPI2_MOSI_H GPIO_SetBits(GPIOB, GPIO_Pin_15);
#define LCD_SPI2_MOSI_L GPIO_ResetBits(GPIOB, GPIO_Pin_15);


void st7789vm_init(void);
void Display_Picture(u16 x,u16 y,u8* pic);
void Display_Eng(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,u8 eng);
void Display_Chi(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,u8* chi);
void Display_String(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,u8* str);
void st7789vm_clear_display(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);

#endif 
