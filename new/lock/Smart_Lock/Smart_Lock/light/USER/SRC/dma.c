#include "dma.h"


/*
��������:DMA�����ʼ��
�����β�:void
��������ֵ:void
����˵��:DMA:2 channel:4 Stream:5

*/
void DMA_Transfer_Init(void)
{
	//��������DMAģʽ
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  //DMA��������
	//��ʱ��EN
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	DMA_InitTypeDef   DMA_InitStruct={0};
	DMA_InitStruct.DMA_BufferSize = 4096;  //ÿ������512���ֽڣ�ÿ���ֽ���8λ
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory; //����:���赽�ڴ淽��
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;  //ѡ��ֱ��ģʽ
	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)Rdata.buf[0]; //�ڴ����ַ 
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ڴ����ݴ�С �ֽ�
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����ʹ��
	DMA_InitStruct.DMA_Mode= DMA_Mode_Circular ; //ѭ��ģʽ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&(USART1->DR); //�������ַ�������
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݴ�С �ֽ�
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ������ʹ��
	
	DMA_Init (DMA2_Stream5, &DMA_InitStruct); 
	
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);  //��������ж�ʹ�ܣ���������һ�������ж�һ��
	//	�������ȼ�
	NVIC_SetPriority(DMA2_Stream5_IRQn,NVIC_EncodePriority(7-2,0,1));
	//	ʹ�ܺ�����Ӧ�ж�
	NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	
	DMA_DoubleBufferModeConfig(DMA2_Stream5,(u32)Rdata.buf[1],DMA_Memory_0);  //������2����ַ����ǰ�Ļ��������ڴ�0
	DMA_DoubleBufferModeCmd(DMA2_Stream5,ENABLE);
	
	DMA_Cmd(DMA2_Stream5,ENABLE);
	
}


/*
	����һ������������жϷ�����
*/
void DMA2_Stream5_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_Stream5,DMA_IT_TCIF5);
	Rdata.len = BUFFERSIZE;
	Rdata.flag = DMAIT;
}

/*
��������:DMA�ֿ����
�����β�:
��������ֵ:void
����˵��:DMA:2 channel:4 Stream:5

*/
void Dma_FontUpdate(void)
{
	u8 i;
	u16 sector =0;
	printf("���ڲ����飬��ȴ�...\r\n");
	for(i = 0; i < 30; i++)
	{
		Block_Erase(i*65536);
	}
	printf("������ϣ��뷢���ֿ��ļ�\r\n");
	while(1)
	{
		switch(Rdata.flag)
		{
			case FREE: break;
			case DMAIT: 
					Rdata.flag = FREE;
					W25q64_Auto_Page_Wtite(Rdata.buf[Rdata.dma_flag],BUFFERSIZE,Rdata.addr);
					Rdata.dma_flag = !Rdata.dma_flag;
					Rdata.addr += Rdata.len;
					printf("%d\r\n",sector++);
					break;
			case USARTIDLE: 
					W25q64_Auto_Page_Wtite(Rdata.buf[Rdata.dma_flag],Rdata.len,Rdata.addr);
					printf("�ļ�������ɣ�\r\n");
					return;
		}
	}
		
}
