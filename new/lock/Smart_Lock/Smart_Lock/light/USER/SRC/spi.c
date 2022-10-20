#include "spi.h"


/*
函数功能:LED8和LED9初始化
函数形参:void
函数返回值:void
函数说明:PA4 – CS – 推挽输出
					PA6 – MISO – 浮空输入
					PA5 – SCK – 推挽输出
					PA7 – MOSI – 推挽输出
	SPI_1
*/
void W25q64_Init(void)
{
	//1. 打开外设的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);
	
	//2. 配置PC4-7
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//配置为复用
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//5，6，7管脚
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//无上下拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;//低速
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//配置为输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3.复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	
	//4.配置SPI控制器
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_InitTypeDef  SPI_InitStruct = {0};
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//SCK是二分频
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//在第一个边沿采集数据
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//空闲时为低电平
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//一帧数据是8位
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线单向全双工
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//先发高位
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//主模式
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//选择软件从模式管理
	SPI_Init(SPI1,&SPI_InitStruct);
	
	
	//5.使能SPI
	 SPI_Cmd(SPI1,ENABLE);	
}

/************************************************
函数功能：SPI发送和接收数据
函数形参：u8 data
函数返回值：u8 
说明：高位先发
************************************************/
u8 Spi_ChangeData(u8 data)
{
	
	u8 buf=0;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI1,data);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	buf = SPI_I2S_ReceiveData(SPI1);
	return buf;
	
//	u8 buf = 0;
//	//等到发送寄存器为空就可以写入数据
//	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET)
//	{
//		
//	}
//	SPI_I2S_SendData(SPI1,data);
//	//等到接收数据寄存器不为空就可以读取数据
//	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		
//	}
//	buf = SPI_I2S_ReceiveData(SPI1);
//	return buf;
}








