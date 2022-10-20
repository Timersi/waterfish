#include "bs8116.h"


/*
函数功能:I2C管脚初始化
函数形参:void
函数返回值:void
函数说明:PB6：BS8116_IIC1_SCLK
					PB7：BS8116_IIC1_SDA

*/
void BS8116_I2C_Pin_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA,ENABLE);//1.开时钟B
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //SCL设置为推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;	//SDA设置为开漏输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*
函数功能:起始条件
函数形参:u8 nus
函数返回值:void
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
void BS8116_I2C_Start(u8 nus)
{
	BS8116_SCL_L;
	TIM5_Init_DS_US(nus);//保证先是从低电平开始
	
	BS8116_SDA_H;
	BS8116_SCL_H;
	TIM5_Init_DS_US(nus); //起始条件的建立时间
	BS8116_SDA_L;
	TIM5_Init_DS_US(nus);	//起始条件的保持时间
	BS8116_SCL_L;
}

/*
函数功能:停止条件
函数形参:u8 nus
函数返回值:void
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
void BS8116_I2C_Stop(u8 nus)
{
	BS8116_SDA_L;
	BS8116_SCL_H;
	TIM5_Init_DS_US(nus); //停止条件的建立时间
	BS8116_SDA_H;
	TIM5_Init_DS_US(nus);	//停止和启动条件之间的总线空闲时间
}

/*
函数功能:发送一个应答位
函数形参:u8 nus, u8 ask
函数返回值:void
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
void BS8116_I2C_Sent_Ask(u8 nus,u8 ask)
{
	BS8116_SCL_L;
	if(ask)
	{
		BS8116_SDA_H;
	}
	else
	{
		BS8116_SDA_L;
	}
	TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
	BS8116_SCL_H;
	TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
//	BS8116_SCL_L;
//	TIM5_Init_DS_US(nus);//表示周期的完整性 
}

/*
函数功能:接收一个应答位
函数形参:u8 nus, u8 ask
函数返回值:u8
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
u8 BS8116_I2C_Recivce_Ask(u8 nus)
{
	BS8116_SDA_H; //断开输出电路 此时主设备正占用着SDA的使用权，而从设备要发送应答位则需要主设备让出SDA的使用权
	u8 ack=0;
	BS8116_SCL_L;	
	TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
	BS8116_SCL_H;
	if(BS8116_SDA)
	{
		ack = 1;
	}
	TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
//	BS8116_SCL_L;
//	TIM5_Init_DS_US(nus);//表示周期的完整性 
	return ack;
}

/*
函数功能:发送一个字节并接收一个应答位
函数形参:u8 nus, u8 *data, u8 addr
函数返回值:u8
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
u8 BS8116_I2C_Sent_Byte(u8 nus, u8 data)
{
	u8 i;
	u8 ack;
	for(i=0;i<8;i++)
	{
		BS8116_SCL_L;
		if(data & (0x80>>i))
		{
			BS8116_SDA_H;
		}
		else
		{
			BS8116_SDA_L;
		}
		TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
		BS8116_SCL_H;
		TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
	}
	ack = BS8116_I2C_Recivce_Ask(nus);
	return ack;
}


/*
函数功能:接收一个字节并发送一个应答位
函数形参:u8 ask,u8 nus
函数返回值:u8
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/
u8 BS8116_I2C_Recieve_Byte(u8 ask,u8 nus)
{
	u8 i,buf=0;
	BS8116_SDA_H;//断开输出电路 
	for(i=0;i<8;i++)
	{
		BS8116_SCL_L;
		TIM5_Init_DS_US(nus);//发送方准备数据需要的时间
		BS8116_SCL_H;
		buf<<=1;
		if(BS8116_SDA)
		{
			buf |= 1;
		}
		TIM5_Init_DS_US(nus);//接收方接收数据需要的时间
	}
	BS8116_I2C_Sent_Ask(nus,ask);
	return buf;
}



/*
函数功能:读取按键信息
函数形参:u8 ask,u8 nus
函数返回值:u8
函数说明:PA8：AT24CXX_IIC3_SCK
					PC9：AT24CXX_IIC3_SDA

*/

u8 Bs8116_ReadKey(void)
{
	u16 key=0;
	u16 buffer =0;
	BS8116_I2C_Start(BS8116_DELAYTIME);
	if(BS8116_I2C_Sent_Byte(BS8116_DELAYTIME,BS8116_WRITEADDRESS))
	{
		BS8116_I2C_Stop(BS8116_DELAYTIME);
		return BS8166_WRITE_ERROR;
	}
	
	while(!BS8116_SCL);
	if(BS8116_I2C_Sent_Byte(BS8116_DELAYTIME,0X08))
	{
		BS8116_I2C_Stop(BS8116_DELAYTIME);
		return BS8166_READ_ADDR_ERROR;
	}
	while(!BS8116_SCL);
	
	BS8116_I2C_Start(BS8116_DELAYTIME);
	if(BS8116_I2C_Sent_Byte(BS8116_DELAYTIME,BS8116_READADDRESS))
	{
		BS8116_I2C_Stop(BS8116_DELAYTIME);
		return BS8166_READ_ADDR_ERROR;
	}
	while(!BS8116_SCL);
	key = BS8116_I2C_Recieve_Byte(0,BS8116_DELAYTIME);
	key = BS8116_I2C_Recieve_Byte(1,BS8116_DELAYTIME)<<8 |key;
	BS8116_I2C_Stop(BS8116_DELAYTIME);
	switch(key)
	{
		case 0X8081:return   '1';//1000 0000 1000 0001 
		case 0X8480:return   '2';//1000 0100 1000 0000
		case 0X8080:return   '3';//1000 0000 1000 0000
		case 0X8082:return   '4';//1000 1000 1000 1010
		case 0X8880:return   '5';//1000 1000 1000 0000
		case 0X80C0:return   '6';//1000 0000 1100 0000
		case 0X8088:return   '7';//1000 0000 1000 1000
		case 0X8180:return   '8';
		case 0X80A0:return   '9';
		case 0X8084:return   '*';
		case 0X8280:return   '0';
		case 0X8090:return   '#';
	}
	return 0xff;

}











