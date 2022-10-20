#include "mg200.h"

USART6_REC MG200_REC={0};
/*
函数功能:MG200引脚初始化
函数形参:void
函数返回值:void
函数说明:PC0 -- DETECT
					PC1 -- PWR

*/
void MG200_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//打开外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	MG200_ON;
}

/*
函数功能:串口初始化
函数形参:void
函数返回值:void
函数说明:PC6 TX ,PC7 RX
*/
void Usart6_Init(void)
{
	
	USART_InitTypeDef  USART_InitStruct ={0};
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//配置GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init  (GPIOC, &GPIO_InitStruct);
	//PA9和PA10复用到USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	//打开USART6时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None ;
	
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART6,&USART_InitStruct);
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);	//使能接受中断
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);	//使能空闲中断
	//	设置优先级
	NVIC_SetPriority(USART6_IRQn,NVIC_EncodePriority(7-2,1,1));
//	使能核心响应中断
	NVIC_EnableIRQ(USART6_IRQn);
	
	USART_Cmd(USART6,ENABLE);//使能USART6
}


/*
函数功能:MG200初始化
函数形参:void
函数返回值:void
函数说明:PC0 -- DETECT
					PC1 -- PWR

*/
void MG200_Init(void)
{
	MG200_Pin_Init();
	Usart6_Init();
}

/*
函数功能:MG200发送数据
函数形参:u8 data
函数返回值:void
函数说明:PC0 -- DETECT
					PC1 -- PWR

*/
void MG200_SendByte(u8 data)
{
	while((USART6->SR & (0x1 << 6)) == 0 );
	USART6 ->SR &= ~(0X1<<6);
	USART6->DR = data;
}



/****************************
函数功能:MG200接收数据中断
函数形参:void
函数返回值:void
函数说明:

*****************************/
void USART6_IRQHandler(void)
{
	u8 data;
	if(USART_GetITStatus(USART6,USART_IT_RXNE))
	{
		MG200_REC.buf[MG200_REC.len++] = USART_ReceiveData(USART6);
	}
	if(USART_GetITStatus(USART6,USART_IT_IDLE))
	{
//		printf("%d",MG200_REC.buf[6]);
		data = USART6->SR;
		data = USART6->DR;
		data = data;
		MG200_REC.flag = 1;
		MG200_REC.len = 0;
	}
}

/****************************
函数功能:MG200发送指令
函数形参:u8 cmd
					u8 parameter
函数返回值:
函数说明:

*****************************/
void MG200_SendCmd(u8 cmd,u8 parameter)
{
	u8 check_sum = 0;
	MG200_SendByte(0x6c);
	MG200_SendByte(0x63);
	MG200_SendByte(0x62);
	MG200_SendByte(cmd);
	MG200_SendByte(parameter);
	MG200_SendByte(0);
	check_sum += (0x63+0x62+cmd+parameter)&0xff;
	MG200_SendByte(check_sum);
}


/****************************
函数功能:MG200接收数据包
函数形参:u8 cmd
					u8 *parameter
					u8 *result
函数返回值:u8 
函数说明:

*****************************/
u8 MG200_Receive(u8 cmd,u8 *parameter,u8 *result)
{
	u8 check_sum = 0;
	while(MG200_REC.flag ==0);
	MG200_REC.flag = 0;
	if(MG200_REC.buf[0] != 0x6c)
	{
		return 1;
	}
	if(MG200_REC.buf[1] != 0x62)
	{
		return 2;
	}
	if(MG200_REC.buf[2] != 0x63)
	{
		return 3;
	}
	if(MG200_REC.buf[3] != cmd)
	{
		return 4;
	}
	*result = MG200_REC.buf[4];
	*parameter = MG200_REC.buf[5];
	if(MG200_REC.buf[6] != 0)
	{
		return 5;
	}
	check_sum += (0x63+0x62+cmd+*parameter+*result)&0xff;
	if(MG200_REC.buf[7] != check_sum)
	{
		return 6;
	}
	return 0;
}


/****************************
函数功能:MG200提取指纹特征
函数形参:
					u8 parameter
函数返回值:u8 
函数说明: result为指纹特征

*****************************/
u8 ReqCaptureAndExtract(u8 parameter)
{
	u8 parame;
	u8 result;
	printf("请放下手指\r\n");
	while(!MG200_DETECT)
	{
		;
	}
	MG200_SendCmd(0x51,parameter);
	if(MG200_Receive(0x51,&parame,&result)==0)
	{
		switch(result)
		{
			case 0:	printf("Success!\r\n"); break;
			case 0xb1: printf("Small finger!\r\n"); break;
			case 0xb2: printf("No finger!\r\n"); break;
			case 0xb3: 	printf("指纹位置过于靠左，需要将手指靠右边摁按（ left finger）");break;
			case 0xb4:  printf("指纹位置过于靠右，需要将手指靠左边摁按（ right finger）");break;
			case 0xb5:  printf("指纹位置过于靠上，需要将手指靠下边摁按（ up finger）");break;
			case 0xb6: 	printf("指纹位置过于靠下，需要将手指靠上边摁按 (down finger）");break;
			case 0xb7:  printf("湿手指 （指纹表面出汗或者水渍过多） (wet finger)");break;
			case 0xb8:  printf("干燥手指(dry finger)");break;
			case 0xc0: 	printf("特征提取错误");break;
			case 0xc2: 	printf("特征提取错误 (图像质量差)");break;
			default :   printf("抓取指纹图像及特征量提取错误");break;
		}
	}
	else
		printf("通信失败\r\n");
	return result;
}


/*
****************************************************************************************
* Function: Enroll
* Description: 注册新指纹用户
* Input: param-参数 00h, 01h, 02h, 03h, 04h （可在 3~5 次之间， 自定义选择注册指纹的次数）
* Output: None
* Return: 00h: 注册成功
					83h: ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误
					91h: 注册失败(用户区域已满)
					93h: 已经注册的 ID
					94h: 指纹提取次数 < 3
* Author: weihaoMo
* Others: 主控单元 MCU 在使用该注册用户指令前必须先执行 3~5 次（执行次数可根据用户
					需求定制） ReqCaptureAndExtract 指令，提取要注册用户指纹的特征量；
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t Registered_fingerprints(void)
{
	uint8_t result;
	uint8_t ackparam;
	
	//抓取指纹的特征3次
	//printf("第一次录入，请放下手指\r\n");
	Display_String(60,77,WHITE,BLACK,16,(u8*)"第一次录入");
	while(!MG200_DETECT);//等待触摸
	result=ReqCaptureAndExtract(0x00);//第一次抓取指纹的特征-参数0x00
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=ReqCaptureAndExtract(0x00);
	}
	//printf("第二次录入，请放下手指\r\n");
	Display_String(60,77,WHITE,BLACK,16,(u8*)"第二次录入");
	while(!MG200_DETECT);//等待触摸
	result=ReqCaptureAndExtract(0x01);//第二次抓取指纹的特征-参数0x01
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=ReqCaptureAndExtract(0x01);
	}
	//printf("第三次录入，请放下手指\r\n");
	Display_String(60,77,WHITE,BLACK,16,(u8*)"第三次录入");
	while(!MG200_DETECT);//等待触摸
	result=ReqCaptureAndExtract(0x02);//第三次抓取指纹的特征-参数0x02
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=ReqCaptureAndExtract(0x02);
	}
	
	
	MG200_SendCmd(0x7F,0x00);//注册的 ID 号（ID 范围 1~100，最大用户数为 100； 当该参数为 00h 时，模块注册成功的 ID 号是模块内部自动分配的）
	if(0!=MG200_Receive(0x7F,&ackparam,&result))
		return 2;
	
	switch(result)
	{
		case 0x00:printf("注册成功\r\n");break;
		case 0x83:printf("ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误\r\n");break;
		case 0x91:printf("注册失败(用户区域已满)\r\n");break;
		case 0x93:printf("已经注册的 ID\r\n");break;
		case 0x94:printf("指纹提取次数 < 3\r\n");break;
		
	}

	return result;
	
}


/******************************
函数功能：比对指纹
函数形参：void 
函数返回值：u8
说明：
	00h: 比对成功
	92h: 比对失败
*******************************/
u8 Matchln(void)
{
	u8 result = 0;
	u8 rec_parameter = 0;
	printf("请放下手指！\r\n");
	while(ReqCaptureAndExtract(0x00))
	{
		ReqCaptureAndExtract(0x00);
	}
	
	MG200_SendCmd(0x71,0);//发送比对的信息
	if(MG200_Receive(0x71,&rec_parameter,&result))
	{
		return 0xff;
	}
	switch(result)
	{
		case 0: printf("比对成功\r\n");break;
		case 0x92: printf("比对失败\r\n");break;
	}
	printf("ID号为：%d\r\n",rec_parameter);
	return rec_parameter;
}


/******************************
函数功能：获取指纹数量
函数形参：void 
函数返回值：u8
说明：
	00h: 比对成功
	92h: 比对失败
*******************************/
u8 GetUserNum(void)
{
	u8 result = 0;
	u8 rec_parameter = 0;
	while(!MG200_DETECT);//等待手指按下
	MG200_SendCmd(0x55,0);//发送比对的信息
	if(MG200_Receive(0x55,&rec_parameter,&result))
	{
		return 0xff;
	}
	switch(result)
	{
		case 0:  printf("成功\r\n"); break;
		default: printf("失败\r\n"); break;
	}
	printf("数量为：%d\r\n",rec_parameter);
	return rec_parameter;
}




