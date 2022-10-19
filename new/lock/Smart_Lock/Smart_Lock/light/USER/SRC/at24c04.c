#include "at24c04.h"


/*
��������:AT24C04��ʼ��
�����β�:void
��������ֵ:void
����˵��:PB8��PB9

*/
void AT24C04_Init(void)
{
	I2C_Pin_Init();
}


/*
��������:ҳд
�����β�:u8 *data,  ��Ҫд�������
					u8 len, 		д�����ݵĳ���
					u8 addr			������д������
��������ֵ:u8
����˵��: ������16�ֽ�

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
	TIM5_Init_DS_MS(5); //д����
	return NO_ERROR;
}

/*
��������:������
�����β�:u8 *data,  �������ݵĲ���
					u8 len, 		д�����ݵĳ���
					u8 addr			�����������
��������ֵ:u8
����˵��: ������16�ֽ�

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
		*data++ = I2C_Recieve_Byte(0,AT24C04_DELAY_TIME);	//ǰ���յ���Ҫ �� Ӧ��

	}
	*data = I2C_Recieve_Byte(1,AT24C04_DELAY_TIME);	//���յ����һ����ʾ�����ٷ���
	I2C_Stop(AT24C04_DELAY_TIME);
	return NO_ERROR;
}

/*
��������:��ҳд
�����β�:u8 *data,  ��Ҫд�������
					u8 len, 		д�����ݵĳ���
					u8 addr			������д������
��������ֵ:void
����˵��: ������16�ֽ�

*/
void AT24C04_Auto_Page_Wtite(u8 *data, u8 len, u8 addr)
{
	u8 rec_cap;
	rec_cap = 16 - addr %16; //��ҳʣ������
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
	TIM5_Init_DS_MS(5); //д����
}



