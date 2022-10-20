#include "spi.h"


/*
��������:LED8��LED9��ʼ��
�����β�:void
��������ֵ:void
����˵��:PA4 �C CS �C �������
					PA6 �C MISO �C ��������
					PA5 �C SCK �C �������
					PA7 �C MOSI �C �������
	SPI_1
*/
void W25q64_Init(void)
{
	//1. �������ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);
	
	//2. ����PC4-7
	GPIO_InitTypeDef  GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//����Ϊ����
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//5��6��7�ܽ�
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;//����
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//����Ϊ���
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//3.����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	
	//4.����SPI������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_InitTypeDef  SPI_InitStruct = {0};
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//SCK�Ƕ���Ƶ
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//�ڵ�һ�����زɼ�����
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//����ʱΪ�͵�ƽ
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//һ֡������8λ
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫�ߵ���ȫ˫��
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//�ȷ���λ
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//��ģʽ
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//ѡ�������ģʽ����
	SPI_Init(SPI1,&SPI_InitStruct);
	
	
	//5.ʹ��SPI
	 SPI_Cmd(SPI1,ENABLE);	
}

/************************************************
�������ܣ�SPI���ͺͽ�������
�����βΣ�u8 data
��������ֵ��u8 
˵������λ�ȷ�
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
//	//�ȵ����ͼĴ���Ϊ�վͿ���д������
//	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET)
//	{
//		
//	}
//	SPI_I2S_SendData(SPI1,data);
//	//�ȵ��������ݼĴ�����Ϊ�վͿ��Զ�ȡ����
//	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		
//	}
//	buf = SPI_I2S_ReceiveData(SPI1);
//	return buf;
}








