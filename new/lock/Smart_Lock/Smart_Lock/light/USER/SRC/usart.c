#include "usart.h"


/*
函数功能:串口初始化
函数形参:void
函数返回值:void
函数说明:PA9 TX ,PA10 RX
*/
void Usart_Init(void)
{
	
	USART_InitTypeDef  USART_InitStruct ={0};
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//配置GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_Init  (GPIOA, &GPIO_InitStruct);
	//PA9和PA10复用到USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//打开USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None ;
	
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//使能接受中断
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);	//使能空闲中断
	//	设置优先级
	NVIC_SetPriority(USART1_IRQn,NVIC_EncodePriority(7-2,1,1));
//	使能核心响应中断
	NVIC_EnableIRQ(USART1_IRQn);
	USART_Cmd(USART1,ENABLE);//使能USART1
}


String_rec Rdata={0};



void USART1_IRQHandler(void)
{
//	static u32 addr =0;
	u8 data;
//	if(USART_GetITStatus(USART1,USART_IT_RXNE))
//	{
//		fs.Font_Flag = 1;
//		data = USART_ReceiveData(USART1);
//		Page_Program(addr++,&data,1);
//		
//		Rdata.len +=1;
		
		
//	}
	if(USART_GetITStatus(USART1,USART_IT_IDLE))
	{
//		data = USART1->SR;
//		data = USART1->DR;
//		fs.Font_Flag = 0;
//		fs.Font_Sta = 0;
		data = USART1->SR;
		data = USART1->DR;
		Rdata.len = BUFFERSIZE - DMA_GetCurrDataCounter(DMA2_Stream5);
		Rdata.flag = USARTIDLE;
	}

}






//printf支持
#pragma import(__use_no_semihosting_swi) //取消半主机状态

struct __FILE { int handle; /* Add whatever you need here */ };
FILE __stdout;

int fputc(int ch, FILE *f) {
	while((USART1->SR &(0X01<<7))==0);
	USART1->DR=ch;
  return (ch);
}
int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
  while((USART1->SR &(0X01<<7))==0);
		USART1->DR=ch;
}


void _sys_exit(int return_code) {
label:  goto label;  /* endless loop */
}






