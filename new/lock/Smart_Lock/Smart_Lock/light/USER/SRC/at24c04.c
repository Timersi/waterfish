#include "at24c04.h"


/*
函数功能:AT24C04初始化
函数形参:void
函数返回值:void
函数说明:PB8和PB9

*/
void AT24C04_Init(void)
{
	I2C_Pin_Init();
}


/*
函数功能:页写
函数形参:u8 *data,  需要写入的数据
					u8 len, 		写入数据的长度
					u8 addr			从哪里写入数据
函数返回值:u8
函数说明: 不超过16字节

*/
u8 AT24C04_Page_Wtite(u8 *data, u8 len, u8 addr)
{
	I2C_Start(AT24C04_DELAY_TIME);
	if(I2C_Sent_Byte(AT24C04_DELAY_TIME,AT24C04_WRITE_ADDR))
	{
		I2C_Stop(AT24C04_DELAY_TIME);
		return WRITE_ADDR_ERROR;
	}
	if(I2C_Sent_Byte(AT24C04_DELAY_TIME,addr))
	{
		I2C_Stop(AT24C04_DELAY_TIME);
		return ADDR_ERROR;
	}
	while(len--)
	{
		if(I2C_Sent_Byte(AT24C04_DELAY_TIME,*data++))
		{
			I2C_Stop(AT24C04_DELAY_TIME);
			return ADDR_ERROR;
		}
	}
	I2C_Stop(AT24C04_DELAY_TIME);
	TIM5_Init_DS_MS(5); //写周期
	return NO_ERROR;
}

/*
函数功能:连续读
函数形参:u8 *data,  保存数据的参数
					u8 len, 		写入数据的长度
					u8 addr			从哪里读数据
函数返回值:u8
函数说明: 不超过16字节

*/
u8 AT24C04_Continue_Read(u8 *data, u8 len, u8 addr)
{
	I2C_Start(AT24C04_DELAY_TIME);
	if(I2C_Sent_Byte(AT24C04_DELAY_TIME,AT24C04_WRITE_ADDR))
	{
		I2C_Stop(AT24C04_DELAY_TIME);
		return WRITE_ADDR_ERROR;
	}
	if(I2C_Sent_Byte(AT24C04_DELAY_TIME,addr))
	{
		I2C_Stop(AT24C04_DELAY_TIME);
		return ADDR_ERROR;
	}
	
	I2C_Start(AT24C04_DELAY_TIME);
	if(I2C_Sent_Byte(AT24C04_DELAY_TIME,AT24C04_READ_ADDR))
	{
		I2C_Stop(AT24C04_DELAY_TIME);
		return WRITE_ADDR_ERROR;
	}
	len -=1;
	while(len--)
	{
		*data++ = I2C_Recieve_Byte(0,AT24C04_DELAY_TIME);	//前面收到的要 回 应答

	}
	*data = I2C_Recieve_Byte(1,AT24C04_DELAY_TIME);	//当收到最后一个表示不用再发了
	I2C_Stop(AT24C04_DELAY_TIME);
	return NO_ERROR;
}

/*
函数功能:换页写
函数形参:u8 *data,  需要写入的数据
					u8 len, 		写入数据的长度
					u8 addr			从哪里写入数据
函数返回值:void
函数说明: 不超过16字节

*/
void AT24C04_Auto_Page_Wtite(u8 *data, u8 len, u8 addr)
{
	u8 rec_cap;
	rec_cap = 16 - addr %16; //本页剩余容量
	if(rec_cap>=len)
	{
		rec_cap = len;
	}
	while(1)
	{
		AT24C04_Page_Wtite(data,rec_cap,addr);
		if(rec_cap ==len)
		{
			break;
		}
		len -= rec_cap;
		data += rec_cap;
		addr +=rec_cap;
		
		if(len>16)
		{
			rec_cap = 16;
		}
		else
		{
			rec_cap = len;
		}
	}
	TIM5_Init_DS_MS(5); //写周期
}



