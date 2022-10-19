#include "tim3.h"


/*
��������:Tim3��Tim2��ʼ�� pwm���
�����β�:void
��������ֵ:void
����˵��:PB3:TIM2_CH2  PB4:TIM3_CH1

*/
void Tim3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	TIM_OCInitTypeDef  TIM_OCInitStruct={0};

	//����GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init  (GPIOB, &GPIO_InitStruct);
	//PB3����
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);
	//PB4
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);
	
	//��ʱ��ʱ������
	//��TIM2,TIM3��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 1000 -1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct );
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct );
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 300;
	TIM_OC1Init(TIM3,&TIM_OCInitStruct);
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);
	
//	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
//	TIM_ITConfig(TIM2,TIM_IT_CC2,ENABLE);
//	//	�������ȼ�
//	NVIC_SetPriority(TIM3_IRQn,NVIC_EncodePriority(7-2,1,1));
////	ʹ�ܺ�����Ӧ�ж�
//	NVIC_EnableIRQ(TIM3_IRQn);
//	//	�������ȼ�
//	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(7-2,1,1));
////	ʹ�ܺ�����Ӧ�ж�
//	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM3,DISABLE);
}

/*
��������: pwm�������
�����β�:void
��������ֵ:void
����˵��:PB3:TIM2_CH2  PB4:TIM3_CH1

*/
void PWM_Open_door(void)
{
	TIM_Cmd(TIM2,ENABLE);
	TIM5_Init_DS_MS(300);
	TIM_Cmd(TIM2,DISABLE);
}


/*
��������: pwm�������
�����β�:void
��������ֵ:void
����˵��:PB3:TIM2_CH2  PB4:TIM3_CH1

*/
void PWM_Close_door(void)
{
	TIM_Cmd(TIM3,ENABLE);
	TIM5_Init_DS_MS(300);
	TIM_Cmd(TIM3,DISABLE);
}


//void TIM2_IRQHandler(void)
//{
//	while(TIM_GetITStatus(TIM2,TIM_IT_CC2)==0);
//	TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
//	printf("TIM2�жϷ���");
//}


//void TIM3_IRQHandler(void)
//{
//	while(TIM_GetITStatus(TIM3,TIM_IT_CC1)==0);
//	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
////	printf("TIM3�жϷ���");

//}






