#include "stm32f10x.h"
#include "usart_2.h"
#include "smartcar.h"

void usart_2_init(u32 bound)
	{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2时钟
  
	//USART2_TX   GPIOA.2	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART2_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3  
		
	 //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength =USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2	
	USART_Cmd(USART2, ENABLE);                    //使能串口2

  
	//Usart2 NVIC 配置
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口2接收中断，参数说明：1.哪个串口，2.重点方式为串口接收完成中断，3.使能中断
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);//根据指定的参数初始化VIC寄存器	 
}
u8 Res;
void USART2_IRQHandler(void)	//串口2的中断函数
{
  //u8 Res;
	if(USART_GetITStatus(USART2,USART_IT_RXNE))//判断是否是串口2接收完成中断
	{
   Res=USART_ReceiveData(USART2);//接收数据
	 USART_SendData(USART2,Res);//发送数据
		
	
 	 
	}
	
}		

