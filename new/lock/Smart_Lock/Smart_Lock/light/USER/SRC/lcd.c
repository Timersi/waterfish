#include "lcd.h"


/*
函数功能:spi2初始化
函数形参:void
函数返回值:void
函数说明:PB10 LCD_RESET
					PB12 LCD_CS
					PB13 LCD_SPI2_SCLK
					PB14 LCD_D/C
					PB15 LCD_SPI2_MOSI
					

*/
void Spi2_Init(void)
{
	//GPIO初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);//1.开时钟
	
	//PC10--SCL--推挽输出,
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15| GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
	GPIO_SetBits(GPIOB, GPIO_Pin_10);
	LCD_LED_L;
}


u8 SPI2_Sent_byte(u8 data)
{
	u8 i,buf = 0;
	SPI2_SCL_H;
	for(i = 0; i < 8; i++)
	{
		SPI2_SCL_L;
		if(data & (0x80 >> i))
		{
			LCD_SPI2_MOSI_H;
		}
		else
		{
			LCD_SPI2_MOSI_L;
		}
		SPI2_SCL_H;
		buf |= 1;
	}
	return buf;
}

/*
函数功能:LCD写命令
函数形参:void
函数返回值:void
函数说明:
					
*/
void st7789vm_write_command(u8 data)
{
	LCD_CS_L;
	LCD_DC_L; //低电平表示发送的是命令
	SPI2_Sent_byte(data);
	LCD_CS_H;
}



/*
函数功能:LCD写命令参数
函数形参:void
函数返回值:void
函数说明:
					
*/
void st7789vm_write_command_parameter(u8 data)
{
	LCD_CS_L;
	LCD_DC_H; //高电平表示发送的是参数数据
//	SPI2_Sent_byte(data>>8);//再发高8位
	SPI2_Sent_byte(data);//再发低8位
	LCD_CS_H;
}


/*
**********************************************************************
函数功能：发送16位数据到LCD屏
函数形参：u16 data
函数返回值：
备注：
**********************************************************************
*/
void st7789vm_write_data(u16 data)
{
	LCD_CS_L;//先拉低片选
	LCD_DC_H; //高电平表示发送的是参数数据
	SPI2_Sent_byte(data >> 8);//先发高8位
	SPI2_Sent_byte(data & 0xff);//再发低8位
	LCD_CS_H;//拉高片选
}


/*
函数功能:LCD定位
函数形参:u8 sx,u8 sy,u8 ex,u8 ey,u16 color
函数返回值:void
函数说明:
					
*/
void st7789vm_SetPosition(u16 sx,u16 sy,u16 ex,u16 ey)
{
	st7789vm_write_command(0X2A);//命令 列设置
	st7789vm_write_data(sx);
	st7789vm_write_data(ex);
	
	st7789vm_write_command(0X2B);//命令 行设置
	st7789vm_write_data(sy);
	st7789vm_write_data(ey);
	
	st7789vm_write_command(0X2C); //颜色设置
//	SPI2_Sent_byte(color>>8);
//	SPI2_Sent_byte(color);
	
}


/*
函数功能:LCD清屏
函数形参:u16 sx,u16 sy,u16 ex,u16 ey,u16 color
函数返回值:void
函数说明:
					
*/
void st7789vm_clear_display(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{
	u32 i;
	st7789vm_SetPosition(sx,sy,ex,ey);
	for(i = 0; i < 240*240; i++)
	{
		st7789vm_write_data(color);
	}
}


/*
函数功能:LCD初始化
函数形参:void
函数返回值:void
函数说明:
					
*/
void st7789vm_init(void)
{
	Spi2_Init();
	
	/* 复位ST7789VM驱动器 */
	LCD_RES_L;
	TIM5_Init_DS_MS(100);
	LCD_RES_H;
	TIM5_Init_DS_MS(100);

	/* start initial sequence */ 
	st7789vm_write_command(0x36);
	st7789vm_write_command_parameter(0x00);

	st7789vm_write_command(0x3A); 
	st7789vm_write_command_parameter(0x05);

	st7789vm_write_command(0xB2);
	st7789vm_write_command_parameter(0x0C);
	st7789vm_write_command_parameter(0x0C);
	st7789vm_write_command_parameter(0x00);
	st7789vm_write_command_parameter(0x33);
	st7789vm_write_command_parameter(0x33); 

	st7789vm_write_command(0xB7); 
	st7789vm_write_command_parameter(0x35);  

	st7789vm_write_command(0xBB);
	st7789vm_write_command_parameter(0x19);

	st7789vm_write_command(0xC0);
	st7789vm_write_command_parameter(0x2C);

	st7789vm_write_command(0xC2);
	st7789vm_write_command_parameter(0x01);

	st7789vm_write_command(0xC3);
	st7789vm_write_command_parameter(0x12);   

	st7789vm_write_command(0xC4);
	st7789vm_write_command_parameter(0x20);  

	st7789vm_write_command(0xC6); 
	st7789vm_write_command_parameter(0x0F);    

	st7789vm_write_command(0xD0); 
	st7789vm_write_command_parameter(0xA4);
	st7789vm_write_command_parameter(0xA1);

	st7789vm_write_command(0xE0);
	st7789vm_write_command_parameter(0xD0);
	st7789vm_write_command_parameter(0x04);
	st7789vm_write_command_parameter(0x0D);
	st7789vm_write_command_parameter(0x11);
	st7789vm_write_command_parameter(0x13);
	st7789vm_write_command_parameter(0x2B);
	st7789vm_write_command_parameter(0x3F);
	st7789vm_write_command_parameter(0x54);
	st7789vm_write_command_parameter(0x4C);
	st7789vm_write_command_parameter(0x18);
	st7789vm_write_command_parameter(0x0D);
	st7789vm_write_command_parameter(0x0B);
	st7789vm_write_command_parameter(0x1F);
	st7789vm_write_command_parameter(0x23);

	st7789vm_write_command(0xE1);
	st7789vm_write_command_parameter(0xD0);
	st7789vm_write_command_parameter(0x04);
	st7789vm_write_command_parameter(0x0C);
	st7789vm_write_command_parameter(0x11);
	st7789vm_write_command_parameter(0x13);
	st7789vm_write_command_parameter(0x2C);
	st7789vm_write_command_parameter(0x3F);
	st7789vm_write_command_parameter(0x44);
	st7789vm_write_command_parameter(0x51);
	st7789vm_write_command_parameter(0x2F);
	st7789vm_write_command_parameter(0x1F);
	st7789vm_write_command_parameter(0x1F);
	st7789vm_write_command_parameter(0x20);
	st7789vm_write_command_parameter(0x23);

	st7789vm_write_command(0x21); 
	st7789vm_write_command(0x11); 
	st7789vm_write_command(0x29); 
	
	/* 清空屏幕显示 */
	st7789vm_clear_display(0, 0, 239, 239, LIGHTBLUE);

	/* 开启背光 */
	LCD_LED_H;
}


/*
函数功能:显示一个点
函数形参:u16 sx,u16 sy
					 fontsize -- 字体大小
					 fontcolor -- 字体颜色
函数返回值:void
函数说明:
					
*/
void Display_OnePoint(u16 x,u16 y,u16 fontcolor)
{
	st7789vm_SetPosition(x,y,x,y);
	st7789vm_write_data(fontcolor);
}

/*
函数功能:显示一个英文字符
函数形参:u16 sx,u16 sy
					 fontcolor -- 字体颜色
					 backcolor -- 背景颜色
					 fontsize -- 字体大小
					 u8 * eng -- 显示的字符
函数返回值:void
函数说明:
					
*/
void Display_Eng(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,	u8 eng)
{
	u8 i,j,k;
	u8 hang = 0;
	u32 addr =0;
	u8 data[70]={0};
	hang = fontsize /2 / 8;  //一行有几个字节
	if(fontsize/2%8)
	{
		hang += 1;
	}
	switch(fontsize)
	{
		case 16:	addr += 0x00000000;	break;
		case 24:	addr += 0x0004064C; break;
		case 32:	addr += 0x000D1A68; break;
		default:	break;
	};
	addr = addr + eng *hang*fontsize;  //hang * fontsize 表示一个字符所占的地址位数
	W25Q64_ReadData(addr,data,hang*fontsize);
	for(i=0;i<fontsize;i++) //行数 一个字节表示一行
	{
		for(k=0;k<hang;k++)//k代表循环完这一行的所有字节
		{
			for(j=0;j<8;j++) //这一行需要判断位数(8位为一个字节)
			{
				if(data[i*hang+k] & 0x80 >> j)
				{
					Display_OnePoint(x+j+8*k,y+i,fontcolor);
				}
				else
				{
					Display_OnePoint(x+j+8*k,y+i,backcolor);
				}
			}
		
		}
	}
	
}

/*
函数功能:显示一个中文字符
函数形参:u16 sx,u16 sy,u16 ex,u16 ey
					 fontsize -- 字体大小
					 fontcolor -- 字体颜色
函数返回值:void
函数说明:
					
*/
void Display_Chi(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,u8* chi)
{
	u32 addr =0;
	u8 data[130]={0};
	u8 i,j,k;
	u8 hang = 0;
	hang = fontsize / 8;  //一行有几个字节
	if(fontsize%8)
	{
		hang += 1;
	}
	switch(fontsize)
	{
		case 16:	addr += 0x00000806;	break;
		case 24:	addr += 0x00041E52; break;
		case 32:	addr += 0x000D3A6E; break;
		default:	break;
	};
	addr = addr + ((*chi-0xa1)*94 + (*(chi+1)-0xa1))*hang*fontsize;
	W25Q64_ReadData(addr,data,hang*fontsize);
	for(i=0;i<fontsize;i++) //行数 一个字节表示一行
	{
		for(k=0;k<hang;k++)//k代表循环完这一行的所有字节
		{
			for(j=0;j<8;j++) //这一行需要判断位数(8位为一个字节)
			{
				if(data[i*hang+k] & 0x80 >> j)
				{
					Display_OnePoint(x+j+8*k,y+i,fontcolor);
				}
				else
				{
					Display_OnePoint(x+j+8*k,y+i,backcolor);
				}
			}
		
		}
	}

}




/*
函数功能:显示一个图片
函数形参:u16 sx,u16 sy,u16 ex,u16 ey
					 fontsize -- 字体大小
					 fontcolor -- 字体颜色
					 u8 *pic -- 图片
函数返回值:void
函数说明:
					
*/
void Display_Picture(u16 x,u16 y,u8* pic)
{
	u32 i;
//	u16 weight=240,hight=180;
	u16 weight=180 ,hight=239;
	st7789vm_SetPosition(x,y,x+weight-1,y+hight-1);
	for(i = 0; i < weight*hight; i++)
	{
		st7789vm_write_data(pic[8+i*2]<<8 | pic[8+i*2+1]);
	}
}


/*
函数功能:显示字符串
函数形参:u16 sx,u16 sy,u16 ex,u16 ey
					 fontsize -- 字体大小
					 fontcolor -- 字体颜色
函数返回值:void
函数说明:
					
*/
void Display_String(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,u8* str)
{
	while(*str)
	{
		if(*str>=0xa1)
		{
			Display_Chi(x,y,fontcolor,backcolor,fontsize,str);
			str += 2;
			x+=fontsize;
			if(x>240-(fontsize))
			{
				x=0;
				y+=fontsize;
			}
		}
		else
		{
			Display_Eng(x,y,fontcolor,backcolor,fontsize,*str);
			str++;
			x+=(fontsize/2);
			if(x>240-(fontsize/2))
			{
				x=0;
				y+=fontsize;
			}
		}
	}
}


