#include "mg200.h"

USART6_REC MG200_REC={0};
/*
��������:MG200���ų�ʼ��
�����β�:void
��������ֵ:void
����˵��:PC0 -- DETECT
					PC1 -- PWR

*/
void MG200_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//������ʱ��
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
��������:���ڳ�ʼ��
�����β�:void
��������ֵ:void
����˵��:PC6 TX ,PC7 RX
*/
void Usart6_Init(void)
{
	
	USART_InitTypeDef  USART_InitStruct ={0};
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//����GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_Init  (GPIOC, &GPIO_InitStruct);
	//PA9��PA10���õ�USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6);
	
	//��USART6ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl= USART_HardwareFlowControl_None ;
	
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART6,&USART_InitStruct);
	
	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);	//ʹ�ܽ����ж�
	USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);	//ʹ�ܿ����ж�
	//	�������ȼ�
	NVIC_SetPriority(USART6_IRQn,NVIC_EncodePriority(7-2,1,1));
//	ʹ�ܺ�����Ӧ�ж�
	NVIC_EnableIRQ(USART6_IRQn);
	
	USART_Cmd(USART6,ENABLE);//ʹ��USART6
}


/*
��������:MG200��ʼ��
�����β�:void
��������ֵ:void
����˵��:PC0 -- DETECT
					PC1 -- PWR

*/
void MG200_Init(void)
{
	MG200_Pin_Init();
	Usart6_Init();
}

/*
��������:MG200��������
�����β�:u8 data
��������ֵ:void
����˵��:PC0 -- DETECT
					PC1 -- PWR

*/
void MG200_SendByte(u8 data)
{
	while((USART6->SR & (0x1 << 6)) == 0 );
	USART6 ->SR &= ~(0X1<<6);
	USART6->DR = data;
}



/****************************
��������:MG200���������ж�
�����β�:void
��������ֵ:void
����˵��:

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
��������:MG200����ָ��
�����β�:u8 cmd
					u8 parameter
��������ֵ:
����˵��:

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
��������:MG200�������ݰ�
�����β�:u8 cmd
					u8 *parameter
					u8 *result
��������ֵ:u8 
����˵��:

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
��������:MG200��ȡָ������
�����β�:
					u8 parameter
��������ֵ:u8 
����˵��: resultΪָ������

*****************************/
u8 ReqCaptureAndExtract(u8 parameter)
{
	u8 parame;
	u8 result;
	printf("�������ָ\r\n");
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
			case 0xb3: 	printf("ָ��λ�ù��ڿ�����Ҫ����ָ���ұ������� left finger��");break;
			case 0xb4:  printf("ָ��λ�ù��ڿ��ң���Ҫ����ָ����������� right finger��");break;
			case 0xb5:  printf("ָ��λ�ù��ڿ��ϣ���Ҫ����ָ���±������� up finger��");break;
			case 0xb6: 	printf("ָ��λ�ù��ڿ��£���Ҫ����ָ���ϱ����� (down finger��");break;
			case 0xb7:  printf("ʪ��ָ ��ָ�Ʊ����������ˮ�չ��ࣩ (wet finger)");break;
			case 0xb8:  printf("������ָ(dry finger)");break;
			case 0xc0: 	printf("������ȡ����");break;
			case 0xc2: 	printf("������ȡ���� (ͼ��������)");break;
			default :   printf("ץȡָ��ͼ����������ȡ����");break;
		}
	}
	else
		printf("ͨ��ʧ��\r\n");
	return result;
}


/*
****************************************************************************************
* Function: Enroll
* Description: ע����ָ���û�
* Input: param-���� 00h, 01h, 02h, 03h, 04h ������ 3~5 ��֮�䣬 �Զ���ѡ��ע��ָ�ƵĴ�����
* Output: None
* Return: 00h: ע��ɹ�
					83h: ID ����(ID < 0 ���� ID > ����û���)����ͨ�Ŵ���
					91h: ע��ʧ��(�û���������)
					93h: �Ѿ�ע��� ID
					94h: ָ����ȡ���� < 3
* Author: weihaoMo
* Others: ���ص�Ԫ MCU ��ʹ�ø�ע���û�ָ��ǰ������ִ�� 3~5 �Σ�ִ�д����ɸ����û�
					�����ƣ� ReqCaptureAndExtract ָ���ȡҪע���û�ָ�Ƶ���������
* Date of completion: 2019-11-29
* Date of last modify: 2019-11-29
****************************************************************************************
*/
uint8_t Registered_fingerprints(void)
{
	uint8_t result;
	uint8_t ackparam;
	
	//ץȡָ�Ƶ�����3��
	//printf("��һ��¼�룬�������ָ\r\n");
	Display_String(60,77,WHITE,BLACK,16,(u8*)"��һ��¼��");
	while(!MG200_DETECT);//�ȴ�����
	result=ReqCaptureAndExtract(0x00);//��һ��ץȡָ�Ƶ�����-����0x00
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=ReqCaptureAndExtract(0x00);
	}
	//printf("�ڶ���¼�룬�������ָ\r\n");
	Display_String(60,77,WHITE,BLACK,16,(u8*)"�ڶ���¼��");
	while(!MG200_DETECT);//�ȴ�����
	result=ReqCaptureAndExtract(0x01);//�ڶ���ץȡָ�Ƶ�����-����0x01
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=ReqCaptureAndExtract(0x01);
	}
	//printf("������¼�룬�������ָ\r\n");
	Display_String(60,77,WHITE,BLACK,16,(u8*)"������¼��");
	while(!MG200_DETECT);//�ȴ�����
	result=ReqCaptureAndExtract(0x02);//������ץȡָ�Ƶ�����-����0x02
	while( result!=0x00 )
	{
		while(!MG200_DETECT);
		result=ReqCaptureAndExtract(0x02);
	}
	
	
	MG200_SendCmd(0x7F,0x00);//ע��� ID �ţ�ID ��Χ 1~100������û���Ϊ 100�� ���ò���Ϊ 00h ʱ��ģ��ע��ɹ��� ID ����ģ���ڲ��Զ�����ģ�
	if(0!=MG200_Receive(0x7F,&ackparam,&result))
		return 2;
	
	switch(result)
	{
		case 0x00:printf("ע��ɹ�\r\n");break;
		case 0x83:printf("ID ����(ID < 0 ���� ID > ����û���)����ͨ�Ŵ���\r\n");break;
		case 0x91:printf("ע��ʧ��(�û���������)\r\n");break;
		case 0x93:printf("�Ѿ�ע��� ID\r\n");break;
		case 0x94:printf("ָ����ȡ���� < 3\r\n");break;
		
	}

	return result;
	
}


/******************************
�������ܣ��ȶ�ָ��
�����βΣ�void 
��������ֵ��u8
˵����
	00h: �ȶԳɹ�
	92h: �ȶ�ʧ��
*******************************/
u8 Matchln(void)
{
	u8 result = 0;
	u8 rec_parameter = 0;
	printf("�������ָ��\r\n");
	while(ReqCaptureAndExtract(0x00))
	{
		ReqCaptureAndExtract(0x00);
	}
	
	MG200_SendCmd(0x71,0);//���ͱȶԵ���Ϣ
	if(MG200_Receive(0x71,&rec_parameter,&result))
	{
		return 0xff;
	}
	switch(result)
	{
		case 0: printf("�ȶԳɹ�\r\n");break;
		case 0x92: printf("�ȶ�ʧ��\r\n");break;
	}
	printf("ID��Ϊ��%d\r\n",rec_parameter);
	return rec_parameter;
}


/******************************
�������ܣ���ȡָ������
�����βΣ�void 
��������ֵ��u8
˵����
	00h: �ȶԳɹ�
	92h: �ȶ�ʧ��
*******************************/
u8 GetUserNum(void)
{
	u8 result = 0;
	u8 rec_parameter = 0;
	while(!MG200_DETECT);//�ȴ���ָ����
	MG200_SendCmd(0x55,0);//���ͱȶԵ���Ϣ
	if(MG200_Receive(0x55,&rec_parameter,&result))
	{
		return 0xff;
	}
	switch(result)
	{
		case 0:  printf("�ɹ�\r\n"); break;
		default: printf("ʧ��\r\n"); break;
	}
	printf("����Ϊ��%d\r\n",rec_parameter);
	return rec_parameter;
}




