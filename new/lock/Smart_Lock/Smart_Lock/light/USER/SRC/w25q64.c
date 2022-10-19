#include "w25q64.h"

/************************************************
�������ܣ���ID
�����βΣ�void
��������ֵ��void
˵��������0X90��ָ��õ�0XEF16
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
�������ܣ�дʹ��
�����βΣ�void
��������ֵ��void
˵��������0X06��ָ��
************************************************/
void Write_Enable(void)
{
	SPI1_CS_L;
	Spi_ChangeData(0X06);
	SPI1_CS_H;
}

/************************************************
�������ܣ�дʧ��
�����βΣ�void
��������ֵ��void
˵��������0X04��ָ��
************************************************/
void Write_Disable(void)
{
	SPI1_CS_L;
	Spi_ChangeData(0X04);
	SPI1_CS_H;
}

/************************************************
�������ܣ���״̬�Ĵ���
�����βΣ�void
��������ֵ��void
˵��������0X04��ָ��
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
�������ܣ���������
�����βΣ�u32 addr
��������ֵ��void
˵����
************************************************/
void Sector_Erase(u32 addr)
{
	Write_Enable();//дʹ��
	while(Read_Status())//������æ��������
	{
		
	}
	SPI1_CS_L;
	Spi_ChangeData(0X20);
	
	//��24λ�����ݴ����λ��ʼ����ȥ
	Spi_ChangeData(addr >> 16);
	Spi_ChangeData(addr >> 8);
	Spi_ChangeData(addr);
	SPI1_CS_H;

	while(Read_Status())//������æ��������
	{
		
	}
}

/************************************************
�������ܣ�ҳ���ָ��
�����βΣ�
			u32 addr -- ���ĸ���ַ��ʼд������ 
			u8 *data -- д�������
			u8 len -- д������ݵĳ���
��������ֵ��void
˵����
************************************************/
void Page_Program(u32 addr,u8 *data,u16 len)
{
	Write_Enable();//дʹ��
	while(Read_Status())//������æ��������
	{
		
	}
	SPI1_CS_L;
	Spi_ChangeData(0X02);
	
	//��24λ�����ݴ����λ��ʼ����ȥ
	Spi_ChangeData(addr >> 16);
	Spi_ChangeData(addr >> 8);
	Spi_ChangeData(addr);
	
	while(len--)
	{
		Spi_ChangeData(*data++);
	}
	
	SPI1_CS_H;
	while(Read_Status())//������æ��������
	{
		
	}
}

/************************************************
�������ܣ�������
�����βΣ�
			u32 addr -- ���ĸ���ַ��ʼ������ 
			u8 *data -- ����������
			u8 len -- �����ݵĳ���
��������ֵ��void
˵����
************************************************/
void W25Q64_ReadData(u32 addr,u8 *data,u8 len)
{
	SPI1_CS_L;
	Spi_ChangeData(0X03);
	
	//��24λ�����ݴ����λ��ʼ����ȥ
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
��������:��ҳд
�����β�:u8 *data,  ��Ҫд�������
					u8 len, 		д�����ݵĳ���
					u8 addr			������д������
��������ֵ:void
����˵��: ������256�ֽ�

*/
void W25q64_Auto_Page_Wtite(u8 *data, u16 len, u32 addr)
{
	u16 rec_cap;
	rec_cap = 256 - addr %256; //��ҳʣ������
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
//	TIM5_Init_DS_MS(5); //д����
}



/************************************************
�������ܣ������
�����βΣ�u32 addr
��������ֵ��void
˵����
************************************************/
void Block_Erase(u32 addr)
{
	Write_Enable();//дʹ��
	while(Read_Status())//������æ��������
	{
		
	}
	SPI1_CS_L;
	Spi_ChangeData(0XD8);
	
	//��24λ�����ݴ����λ��ʼ����ȥ
	Spi_ChangeData(addr >> 16);
	Spi_ChangeData(addr >> 8);
	Spi_ChangeData(addr);
	SPI1_CS_H;

	while(Read_Status())//������æ��������
	{
		
	}
}

Font_struct fs={0};
/************************************************
�������ܣ�����
�����βΣ�u32 addr
��������ֵ��void
˵����
************************************************/
void Font_Update(void)
{
	u8 i;
	printf("���ڲ����飬��ȴ�...\r\n");
	fs.Font_Sta = 1;
	for(i = 0; i < 30; i++)
	{
		Block_Erase(i*65536);
	}
	printf("������ϣ��뷢���ֿ��ļ�\r\n");
	while(fs.Font_Flag == 0)
	{
		
	}
	printf("������¼�ֿ��ļ�...\r\n");
	while(fs.Font_Sta)
	{
		
	}
	printf("��¼���\r\n");
}





