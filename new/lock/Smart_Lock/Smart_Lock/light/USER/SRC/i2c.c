#include "i2c.h"


/*
��������:I2C�ܽų�ʼ��
�����β�:void
��������ֵ:void
����˵��:PA8��AT24CXX_IIC3_SCK
					PC9��AT24CXX_IIC3_SDA

*/
void I2C_Pin_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);//1.��ʱ��A,C
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC ,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; //SCL����Ϊ�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;	//SDA����Ϊ��©���
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}

/*
��������:��ʼ����
�����β�:u8 nus
��������ֵ:void
����˵��:PA8��AT24CXX_IIC3_SCK
					PC9��AT24CXX_IIC3_SDA

*/
void I2C_Start(u8 nus)
{
	SDA_H;
	SCL_H;
	TIM5_Init_DS_US(nus); //��ʼ�����Ľ���ʱ��
	SDA_L;
	TIM5_Init_DS_US(nus);	//��ʼ�����ı���ʱ��
	SCL_L;
}

/*
��������:ֹͣ����
�����β�:u8 nus
��������ֵ:void
����˵��:PA8��AT24CXX_IIC3_SCK
					PC9��AT24CXX_IIC3_SDA

*/
void I2C_Stop(u8 nus)
{
	SDA_L;
	SCL_H;
	TIM5_Init_DS_US(nus); //��ʼ�����Ľ���ʱ��
	SDA_H;
	TIM5_Init_DS_US(nus);	//ֹͣ����������֮������߿���ʱ��
}

/*
��������:����һ��Ӧ��λ
�����β�:u8 nus, u8 ask
��������ֵ:void
����˵��:PA8��AT24CXX_IIC3_SCK
					PC9��AT24CXX_IIC3_SDA

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
	TIM5_Init_DS_US(nus);//���ͷ�׼��������Ҫ��ʱ��
	SCL_H;
	TIM5_Init_DS_US(nus);//���շ�����������Ҫ��ʱ��
	SCL_L;
	TIM5_Init_DS_US(nus);//��ʾ���ڵ������� 
}

/*
��������:����һ��Ӧ��λ
�����β�:u8 nus, u8 ask
��������ֵ:u8
����˵��:PA8��AT24CXX_IIC3_SCK
					PC9��AT24CXX_IIC3_SDA

*/
u8 I2C_Recivce_Ask(u8 nus)
{
	SDA_H; //�Ͽ������· ��ʱ���豸��ռ����SDA��ʹ��Ȩ�������豸Ҫ����Ӧ��λ����Ҫ���豸�ó�SDA��ʹ��Ȩ
	u8 ack=0;
	SCL_L;	
	TIM5_Init_DS_US(nus);//���ͷ�׼��������Ҫ��ʱ��
	SCL_H;
	if(SDA)
	{
		ack = 1;
	}
	TIM5_Init_DS_US(nus);//���շ�����������Ҫ��ʱ��
	SCL_L;
	TIM5_Init_DS_US(nus);//��ʾ���ڵ������� 
	return ack;
}

/*
��������:����һ���ֽڲ�����һ��Ӧ��λ
�����β�:u8 nus, u8 *data, u8 addr
��������ֵ:u8
����˵��:PA8��AT24CXX_IIC3_SCK
					PC9��AT24CXX_IIC3_SDA

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
		TIM5_Init_DS_US(nus);//���ͷ�׼��������Ҫ��ʱ��
		SCL_H;
		TIM5_Init_DS_US(nus);//���շ�����������Ҫ��ʱ��
	}
	ack = I2C_Recivce_Ask(nus);

	return ack;
}


/*
��������:����һ���ֽڲ�����һ��Ӧ��λ
�����β�:u8 ask,u8 nus
��������ֵ:u8
����˵��:PA8��AT24CXX_IIC3_SCK
					PC9��AT24CXX_IIC3_SDA

*/
u8 I2C_Recieve_Byte(u8 ask,u8 nus)
{
	u8 i,buf=0;
	SDA_H;//�Ͽ������· 
	for(i=0;i<8;i++)
	{
		SCL_L;
		TIM5_Init_DS_US(nus);//���ͷ�׼��������Ҫ��ʱ��
		SCL_H;
		buf<<=1;
		if(SDA)
		{
			buf |= 1;
		}
		TIM5_Init_DS_US(nus);//���շ�����������Ҫ��ʱ��
	}
	I2C_Sent_Ask(nus,ask);
	return buf;
}

