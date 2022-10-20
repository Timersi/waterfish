#ifndef __LCD_H
#define __LCD_H
#include "stm32f4xx.h"
#include "tim.h"
#include "stdio.h"
#include "w25q64.h"

/* ��ɫ���忪ʼ */
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
/* ��ɫ������� */


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
