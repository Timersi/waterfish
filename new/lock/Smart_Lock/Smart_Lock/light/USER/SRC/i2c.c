#include "i2c.h"


/*
函数功能:I2C管脚初始化
函数形参:void
函数返回值:void
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
void I2C_Pin_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);//1.开时钟A,C
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //SCL设置为推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;	//SDA设置为开漏输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}

/*
函数功能:起始条件
函数形参:u8 nus
函数返回值:void
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
void I2C_Start(u8 nus)
{
	SDA_H;
	SCL_H;
	TIM5_Init_DS_US(nus); //起始条件的建立时间
	SDA_L;
	TIM5_Init_DS_US(nus);	//起始条件的保持时间
	SCL_L;
}

/*
函数功能:停止条件
函数形参:u8 nus
函数返回值:void
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
void I2C_Stop(u8 nus)
{
	SDA_L;
	SCL_H;
	TIM5_Init_DS_US(nus); //起始条件的建立时间
	SDA_H;
	TIM5_Init_DS_US(nus);	//停止和启动条件之间的总线空闲时间
}

/*
函数功能:发送一个应答位
函数形参:u8 nus, u8 ask
函数返回值:void
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
void I2C_Sent_Ask(u8 nus,u8 ask)
{
	SCL_L;
	if(ask)
	{
		SDA_H;
	}
	else
	{
		SDA_L;
	}
	TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
	SCL_H;
	TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
	SCL_L;
	TIM5_Init_DS_US(nus);//表示周期的完整性 
}

/*
函数功能:接收一个应答位
函数形参:u8 nus, u8 ask
函数返回值:u8
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
u8 I2C_Recivce_Ask(u8 nus)
{
	SDA_H; //断开输出电路 此时主设备正占用着SDA的使用权，而从设备要发送应答位则需要主设备让出SDA的使用权
	u8 ack=0;
	SCL_L;	
	TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
	SCL_H;
	if(SDA)
	{
		ack = 1;
	}
	TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
	SCL_L;
	TIM5_Init_DS_US(nus);//表示周期的完整性 
	return ack;
}

/*
函数功能:发送一个字节并接收一个应答位
函数形参:u8 nus, u8 *data, u8 addr
函数返回值:u8
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
u8 I2C_Sent_Byte(u8 nus, u8 data)
{
	u8 i;
	u8 ack;
	for(i=0;i<8;i++)
	{
		SCL_L;
		if(data & (0x80>>i))
		{
			SDA_H;
		}
		else
		{
			SDA_L;
		}
		TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
		SCL_H;
		TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
	}
	ack = I2C_Recivce_Ask(nus);

	return ack;
}


/*
函数功能:接收一个字节并发送一个应答位
函数形参:u8 ask,u8 nus
函数返回值:u8
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
u8 I2C_Recieve_Byte(u8 ask,u8 nus)
{
	u8 i,buf=0;
	SDA_H;//断开输出电路 
	for(i=0;i<8;i++)
	{
		SCL_L;
		TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
		SCL_H;
		buf<<=1;
		if(SDA)
		{
			buf |= 1;
		}
		TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
	}
	I2C_Sent_Ask(nus,ask);
	return buf;
}

