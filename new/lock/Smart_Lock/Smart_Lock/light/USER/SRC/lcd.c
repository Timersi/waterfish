#include "lcd.h"


/*
��������:spi2��ʼ��
�����β�:void
��������ֵ:void
����˵��:PB10 LCD_RESET
					PB12 LCD_CS
					PB13 LCD_SPI2_SCLK
					PB14 LCD_D/C
					PB15 LCD_SPI2_MOSI
					

*/
void Spi2_Init(void)
{
	//GPIO��ʼ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);//1.��ʱ��
	
	//PC10--SCL--�������,
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
��������:LCDд����
�����β�:void
��������ֵ:void
����˵��:
					
*/
void st7789vm_write_command(u8 data)
{
	LCD_CS_L;
	LCD_DC_L; //�͵�ƽ��ʾ���͵�������
	SPI2_Sent_byte(data);
	LCD_CS_H;
}



/*
��������:LCDд�������
�����β�:void
��������ֵ:void
����˵��:
					
*/
void st7789vm_write_command_parameter(u8 data)
{
	LCD_CS_L;
	LCD_DC_H; //�ߵ�ƽ��ʾ���͵��ǲ�������
//	SPI2_Sent_byte(data>>8);//�ٷ���8λ
	SPI2_Sent_byte(data);//�ٷ���8λ
	LCD_CS_H;
}


/*
**********************************************************************
�������ܣ�����16λ���ݵ�LCD��
�����βΣ�u16 data
��������ֵ��
��ע��
**********************************************************************
*/
void st7789vm_write_data(u16 data)
{
	LCD_CS_L;//������Ƭѡ
	LCD_DC_H; //�ߵ�ƽ��ʾ���͵��ǲ�������
	SPI2_Sent_byte(data >> 8);//�ȷ���8λ
	SPI2_Sent_byte(data & 0xff);//�ٷ���8λ
	LCD_CS_H;//����Ƭѡ
}


/*
��������:LCD��λ
�����β�:u8 sx,u8 sy,u8 ex,u8 ey,u16 color
��������ֵ:void
����˵��:
					
*/
void st7789vm_SetPosition(u16 sx,u16 sy,u16 ex,u16 ey)
{
	st7789vm_write_command(0X2A);//���� ������
	st7789vm_write_data(sx);
	st7789vm_write_data(ex);
	
	st7789vm_write_command(0X2B);//���� ������
	st7789vm_write_data(sy);
	st7789vm_write_data(ey);
	
	st7789vm_write_command(0X2C); //��ɫ����
//	SPI2_Sent_byte(color>>8);
//	SPI2_Sent_byte(color);
	
}


/*
��������:LCD����
�����β�:u16 sx,u16 sy,u16 ex,u16 ey,u16 color
��������ֵ:void
����˵��:
					
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
��������:LCD��ʼ��
�����β�:void
��������ֵ:void
����˵��:
					
*/
void st7789vm_init(void)
{
	Spi2_Init();
	
	/* ��λST7789VM������ */
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
	
	/* �����Ļ��ʾ */
	st7789vm_clear_display(0, 0, 239, 239, LIGHTBLUE);

	/* �������� */
	LCD_LED_H;
}


/*
��������:��ʾһ����
�����β�:u16 sx,u16 sy
					 fontsize -- �����С
					 fontcolor -- ������ɫ
��������ֵ:void
����˵��:
					
*/
void Display_OnePoint(u16 x,u16 y,u16 fontcolor)
{
	st7789vm_SetPosition(x,y,x,y);
	st7789vm_write_data(fontcolor);
}

/*
��������:��ʾһ��Ӣ���ַ�
�����β�:u16 sx,u16 sy
					 fontcolor -- ������ɫ
					 backcolor -- ������ɫ
					 fontsize -- �����С
					 u8 * eng -- ��ʾ���ַ�
��������ֵ:void
����˵��:
					
*/
void Display_Eng(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,	u8 eng)
{
	u8 i,j,k;
	u8 hang = 0;
	u32 addr =0;
	u8 data[70]={0};
	hang = fontsize /2 / 8;  //һ���м����ֽ�
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
	addr = addr + eng *hang*fontsize;  //hang * fontsize ��ʾһ���ַ���ռ�ĵ�ַλ��
	W25Q64_ReadData(addr,data,hang*fontsize);
	for(i=0;i<fontsize;i++) //���� һ���ֽڱ�ʾһ��
	{
		for(k=0;k<hang;k++)//k����ѭ������һ�е������ֽ�
		{
			for(j=0;j<8;j++) //��һ����Ҫ�ж�λ��(8λΪһ���ֽ�)
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
��������:��ʾһ�������ַ�
�����β�:u16 sx,u16 sy,u16 ex,u16 ey
					 fontsize -- �����С
					 fontcolor -- ������ɫ
��������ֵ:void
����˵��:
					
*/
void Display_Chi(u16 x,u16 y,u16 fontcolor,u16 backcolor,u8 fontsize,u8* chi)
{
	u32 addr =0;
	u8 data[130]={0};
	u8 i,j,k;
	u8 hang = 0;
	hang = fontsize / 8;  //һ���м����ֽ�
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
	for(i=0;i<fontsize;i++) //���� һ���ֽڱ�ʾһ��
	{
		for(k=0;k<hang;k++)//k����ѭ������һ�е������ֽ�
		{
			for(j=0;j<8;j++) //��һ����Ҫ�ж�λ��(8λΪһ���ֽ�)
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
��������:��ʾһ��ͼƬ
�����β�:u16 sx,u16 sy,u16 ex,u16 ey
					 fontsize -- �����С
					 fontcolor -- ������ɫ
					 u8 *pic -- ͼƬ
��������ֵ:void
����˵��:
					
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
��������:��ʾ�ַ���
�����β�:u16 sx,u16 sy,u16 ex,u16 ey
					 fontsize -- �����С
					 fontcolor -- ������ɫ
��������ֵ:void
����˵��:
					
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


