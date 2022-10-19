#include "esp32.h"

USART2_REC ESP32_REC={0};


/*
函数功能:esp32chushihua 
函数形参:void
函数返回值:void
函数说明:PA2 USART_TX     PA3 USART_RX
*/
void Usart2_Init(void)
{
	
	USART_InitTypeDef  USART_InitStruct ={0};
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//配置GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_Init  (GPIOA, &GPIO_InitStruct);
	//PA2和PA3复用到USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	
	//打开USART6时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None ;
	
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2,&USART_InitStruct);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	//使能接受中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);	//使能空闲中断
	//	设置优先级
	NVIC_SetPriority(USART2_IRQn,NVIC_EncodePriority(7-2,1,1));
//	使能核心响应中断
	NVIC_EnableIRQ(USART2_IRQn);
	
	USART_Cmd(USART2,ENABLE);//使能USART2
}




/*
函数功能:ESP32_SendByte发送数据
函数形参:u8 *data
函数返回值:void
函数说明:PC0 -- DETECT
					PC1 -- PWR

*/
void Esp32_SendByte(u8 *data)
{
	while(*data != '\0')
	{
		while((USART2->SR & (0X1 << 6)) == 0);
		USART2->SR &= ~(0X1 << 6);
		USART2->DR = *data++;
	}
}



/****************************
函数功能:MG200接收数据中断
函数形参:void
函数返回值:void
函数说明:

*****************************/
void USART2_IRQHandler(void)
{
	u8 temp;
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		//假如收到有多个字符串组成的一个字符串时，拼接成一个字符串
		temp = USART_ReceiveData(USART2);
	
		if(temp == 0)
		{
			ESP32_REC.buf[ESP32_REC.len++] = ' ';
		}
		else
		{
			ESP32_REC.buf[ESP32_REC.len++] = temp;
		}		
	}
	else if(USART_GetITStatus(USART2,USART_IT_IDLE))//空闲中断
	{
		//清除空闲中断标志
		temp = USART2->SR;
		temp = USART2->DR;
		
		ESP32_REC.buf[ESP32_REC.len] = '\0';
		ESP32_REC.len = 0;
		ESP32_REC.flag= 1;
		printf("%s", ESP32_REC.buf);
	}
}


/****************************
函数功能:esp32是否接收到想要的信号
函数形参:u8 *cmd
					u8 *respond
函数返回值:u8
函数说明:

*****************************/
u8 ESP32_SendAndReceive(u8 *cmd,u8 *respond)
{
	Esp32_SendByte(cmd);
	TIM5_Init_DS_MS(200);
	while(!ESP32_REC.flag);
	
	ESP32_REC.flag =0;
	if(strstr((const char *)ESP32_REC.buf,(const char *)respond) != NULL)
	{
		return 0;
	}
	return 1;
}

/****************************
函数功能:esp32退出透传模式
函数形参:u8 *cmd
					u8 *respond
函数返回值:void
函数说明:

*****************************/
void Exit_CipMode(void)
{
	Esp32_SendByte((u8*)"+++");
	TIM5_Init_DS_MS(1000);
}


/****************************
函数功能:esp32回复出厂设置
函数形参:u8 *cmd
					u8 *respond
函数返回值:void
函数说明:

*****************************/
u8 ESP32_Restore(void)
{

	Esp32_SendByte((u8 *)"AT+RESTORE\r\n");

	while(1)
	{
		TIM5_Init_DS_MS(500);
		while(!ESP32_REC.flag);//等待传输完成标志
		ESP32_REC.flag=0;//清标志
				
		if(NULL!=strstr((const char *)ESP32_REC.buf,(const char *)"OK"))
		{
			printf("ESP32 RESTORE OK\r\n");		
			return 0;
		}
		return 1;
	}
	
}


/****************************
函数功能:TCP的初始化
函数形参:u8 *cmd
					u8 *respond
函数返回值:void
函数说明:

*****************************/
void ESP32_TCP_Init(void)
{
	//退出透传
	Exit_CipMode();
	
	Esp32_SendByte((u8 *)"AT\r\n");
	TIM5_Init_DS_MS(1500);
	ESP32_Restore();//复位，恢复出厂设置
	TIM5_Init_DS_MS(1500);
	
	
	if(ESP32_SendAndReceive((u8*)"AT+CWMODE=1\r\n",(u8*)"OK")) //正确返回0
	{
		printf("AT+CWMODE Error!\r\n");
	}
	TIM5_Init_DS_MS(500);
	//使能上电不自动连接AP
	if(ESP32_SendAndReceive((u8 *)"AT+CWAUTOCONN=0\r\n",(u8 *)"OK"))
	{
		printf("AT+CWAUTOCONN error\r\n");
	}
	TIM5_Init_DS_MS(500);
	//开启透传
	if(ESP32_SendAndReceive((u8*)"AT+CIPMODE=1\r\n",(u8*)"OK")) //正确返回0
	{
		printf("AT+CIPMODE Error!\r\n"); 
	}
	TIM5_Init_DS_MS(500);
	
	printf("TCP初始化完成!\r\n"); 
}


/****************************
函数功能:wifi的连接
函数形参:u8 *ssid
					u8 *pwd
函数返回值:void
函数说明:

*****************************/
void ESP32_Wifi_connect(u8 *ssid,u8 *pwd)
{
	u8 wifibuff[50]={"AT+CWJAP=\""};
//	Esp32_SendByte((u8*)"AT+CWQAP\r\n");
	strcat((char *)wifibuff,(char *)ssid);
	strcat((char *)wifibuff,"\",\"");
	strcat((char *)wifibuff,(char *)pwd);
	strcat((char *)wifibuff,"\"\r\n");

	Esp32_SendByte((u8*)wifibuff);
	while(1)
	{
		while(!ESP32_REC.flag);
		ESP32_REC.flag = 0;
		
		if(strstr((const char *)ESP32_REC.buf,(const char *)"OK")!=NULL)
		{
			printf("Wifi连接成功\r\n");		
			return;
		}
	}
}


/*****************************************************
函数功能：服务器的连接
函数形参：
			u8 *remote_ip
函数返回值：void
函数说明：	
		AT+CIPSTART="TCP","a1H38Of5FJU.iot-as-mqtt.cn-shanghai.aliyuncs.com",1883
*****************************************************/
void Esp32_Severconnect(u8 *remote_ip)
{
	u8 severbuff[100]={"AT+CIPSTART=\"TCP\",\""};
	strcat((char *)severbuff,(char *)remote_ip);
	strcat((char *)severbuff,"\",");
	strcat((char *)severbuff,(char*)"1883");
	strcat((char *)severbuff,"\r\n");

	Esp32_SendByte((u8*)severbuff);
	while(1)
	{
		while(!ESP32_REC.flag);
		ESP32_REC.flag = 0;
		
		if(strstr((const char *)ESP32_REC.buf,(const char *)"OK")!=NULL)
		{
			printf("服务器连接成功\r\n");		
			return;
		}
	}
	

}









