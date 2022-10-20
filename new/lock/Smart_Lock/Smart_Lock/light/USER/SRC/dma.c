#include "dma.h"


/*
函数功能:DMA传输初始化
函数形参:void
函数返回值:void
函数说明:DMA:2 channel:4 Stream:5

*/
void DMA_Transfer_Init(void)
{
	//开启串口DMA模式
	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  //DMA接收请求
	//打开时钟EN
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	DMA_InitTypeDef   DMA_InitStruct={0};
	DMA_InitStruct.DMA_BufferSize = 4096;  //每个扇区512个字节，每个字节有8位
	DMA_InitStruct.DMA_Channel = DMA_Channel_4;
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory; //方向:外设到内存方向
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;  //选择直接模式
	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)Rdata.buf[0]; //内存基地址 
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //内存数据大小 字节
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增使能
	DMA_InitStruct.DMA_Mode= DMA_Mode_Circular ; //循环模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&(USART1->DR); //外设基地址（到哪里）
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据大小 字节
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址递增不使能
	
	DMA_Init (DMA2_Stream5, &DMA_InitStruct); 
	
	DMA_ITConfig(DMA2_Stream5,DMA_IT_TC,ENABLE);  //传输完成中断使能，即传输完一个扇区中断一次
	//	设置优先级
	NVIC_SetPriority(DMA2_Stream5_IRQn,NVIC_EncodePriority(7-2,0,1));
	//	使能核心响应中断
	NVIC_EnableIRQ(DMA2_Stream5_IRQn);
	
	DMA_DoubleBufferModeConfig(DMA2_Stream5,(u32)Rdata.buf[1],DMA_Memory_0);  //缓冲区2基地址，当前的缓冲区是内存0
	DMA_DoubleBufferModeCmd(DMA2_Stream5,ENABLE);
	
	DMA_Cmd(DMA2_Stream5,ENABLE);
	
}


/*
	传输一个缓冲区完成中断服务函数
*/
void DMA2_Stream5_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_Stream5,DMA_IT_TCIF5);
	Rdata.len = BUFFERSIZE;
	Rdata.flag = DMAIT;
}

/*
函数功能:DMA字库更新
函数形参:
函数返回值:void
函数说明:DMA:2 channel:4 Stream:5

*/
void Dma_FontUpdate(void)
{
	u8 i;
	u16 sector =0;
	printf("正在擦除块，请等待...\r\n");
	for(i = 0; i < 30; i++)
	{
		Block_Erase(i*65536);
	}
	printf("擦除完毕，请发送字库文件\r\n");
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
					printf("文件传输完成！\r\n");
					return;
		}
	}
		
}
