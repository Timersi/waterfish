#include "w25q64.h"

/************************************************
函数功能：读ID
函数形参：void
函数返回值：void
说明：发送0X90的指令，得到0XEF16
************************************************/
void Read_W25q64ID(void)
{
	u16 id = 0;
	SPI1_CS_L;
	Spi_ChangeData(0X90);
	
	Spi_ChangeData(0X0);
	Spi_ChangeData(0X0);
	Spi_ChangeData(0X0);
	
	id = Spi_ChangeData(0xff);
	id = id << 8 | Spi_ChangeData(0xff);
	SPI1_CS_H;
	printf("%#x",id);
}

/************************************************
函数功能：写使能
函数形参：void
函数返回值：void
说明：发送0X06的指令
************************************************/
void Write_Enable(void)
{
	SPI1_CS_L;
	Spi_ChangeData(0X06);
	SPI1_CS_H;
}

/************************************************
函数功能：写失能
函数形参：void
函数返回值：void
说明：发送0X04的指令
************************************************/
void Write_Disable(void)
{
	SPI1_CS_L;
	Spi_ChangeData(0X04);
	SPI1_CS_H;
}

/************************************************
函数功能：读状态寄存器
函数形参：void
函数返回值：void
说明：发送0X04的指令
************************************************/
u8 Read_Status(void)
{
	u8 status;
	SPI1_CS_L;
	Spi_ChangeData(0X05);
	status = Spi_ChangeData(0Xff);
	SPI1_CS_H;
	if(status & (0x1 << 0))
	{
		return 1;
	}
	return 0;
}

/************************************************
函数功能：扇区擦除
函数形参：u32 addr
函数返回值：void
说明：
************************************************/
void Sector_Erase(u32 addr)
{
	Write_Enable();//写使能
	while(Read_Status())//读到不忙才往下走
	{
		
	}
	SPI1_CS_L;
	Spi_ChangeData(0X20);
	
	//把24位的数据从最高位开始发出去
	Spi_ChangeData(addr >> 16);
	Spi_ChangeData(addr >> 8);
	Spi_ChangeData(addr);
	SPI1_CS_H;

	while(Read_Status())//读到不忙才往下走
	{
		
	}
}

/************************************************
函数功能：页编程指令
函数形参：
			u32 addr -- 从哪个地址开始写入数据 
			u8 *data -- 写入的数据
			u8 len -- 写入的数据的长度
函数返回值：void
说明：
************************************************/
void Page_Program(u32 addr,u8 *data,u16 len)
{
	Write_Enable();//写使能
	while(Read_Status())//读到不忙才往下走
	{
		
	}
	SPI1_CS_L;
	Spi_ChangeData(0X02);
	
	//把24位的数据从最高位开始发出去
	Spi_ChangeData(addr >> 16);
	Spi_ChangeData(addr >> 8);
	Spi_ChangeData(addr);
	
	while(len--)
	{
		Spi_ChangeData(*data++);
	}
	
	SPI1_CS_H;
	while(Read_Status())//读到不忙才往下走
	{
		
	}
}

/************************************************
函数功能：读数据
函数形参：
			u32 addr -- 从哪个地址开始读数据 
			u8 *data -- 读到的数据
			u8 len -- 读数据的长度
函数返回值：void
说明：
************************************************/
void W25Q64_ReadData(u32 addr,u8 *data,u8 len)
{
	SPI1_CS_L;
	Spi_ChangeData(0X03);
	
	//把24位的数据从最高位开始发出去
	Spi_ChangeData(addr >> 16);
	Spi_ChangeData(addr >> 8);
	Spi_ChangeData(addr);
	
	while(len--)
	{
		*data++ = Spi_ChangeData(0xff);
	}
	
	SPI1_CS_H;
}



/*
函数功能:换页写
函数形参:u8 *data,  需要写入的数据
					u8 len, 		写入数据的长度
					u8 addr			从哪里写入数据
函数返回值:void
函数说明: 不超过256字节

*/
void W25q64_Auto_Page_Wtite(u8 *data, u16 len, u32 addr)
{
	u16 rec_cap;
	rec_cap = 256 - addr %256; //本页剩余容量
	if(rec_cap>=len)
	{
		rec_cap = len;
	}
	while(1)
	{
		 Page_Program(addr,data,rec_cap);
		if(rec_cap ==len)
		{
			break;
		}
		len -= rec_cap;
		data += rec_cap;
		addr +=rec_cap;
		
		if(len>256)
		{
			rec_cap = 256;
		}
		else
		{
			rec_cap = len;
		}
	}
//	TIM5_Init_DS_MS(5); //写周期
}



/************************************************
函数功能：块擦除
函数形参：u32 addr
函数返回值：void
说明：
************************************************/
void Block_Erase(u32 addr)
{
	Write_Enable();//写使能
	while(Read_Status())//读到不忙才往下走
	{
		
	}
	SPI1_CS_L;
	Spi_ChangeData(0XD8);
	
	//把24位的数据从最高位开始发出去
	Spi_ChangeData(addr >> 16);
	Spi_ChangeData(addr >> 8);
	Spi_ChangeData(addr);
	SPI1_CS_H;

	while(Read_Status())//读到不忙才往下走
	{
		
	}
}

Font_struct fs={0};
/************************************************
函数功能：接收
函数形参：u32 addr
函数返回值：void
说明：
************************************************/
void Font_Update(void)
{
	u8 i;
	printf("正在擦除块，请等待...\r\n");
	fs.Font_Sta = 1;
	for(i = 0; i < 30; i++)
	{
		Block_Erase(i*65536);
	}
	printf("擦除完毕，请发送字库文件\r\n");
	while(fs.Font_Flag == 0)
	{
		
	}
	printf("正在烧录字库文件...\r\n");
	while(fs.Font_Sta)
	{
		
	}
	printf("烧录完成\r\n");
}





