#include "tim3.h"


/*
函数功能:Tim3和Tim2初始化 pwm输出
函数形参:void
函数返回值:void
函数说明:PB3:TIM2_CH2  PB4:TIM3_CH1

*/
void Tim3_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	TIM_OCInitTypeDef  TIM_OCInitStruct={0};

	//配置GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init  (GPIOB, &GPIO_InitStruct);
	//PB3复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);
	//PB4
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3);
	
	//定时器时基配置
	//打开TIM2,TIM3的时钟
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
//	//	设置优先级
//	NVIC_SetPriority(TIM3_IRQn,NVIC_EncodePriority(7-2,1,1));
////	使能核心响应中断
//	NVIC_EnableIRQ(TIM3_IRQn);
//	//	设置优先级
//	NVIC_SetPriority(TIM2_IRQn,NVIC_EncodePriority(7-2,1,1));
////	使能核心响应中断
//	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2,DISABLE);
	TIM_Cmd(TIM3,DISABLE);
}

/*
函数功能: pwm输出开门
函数形参:void
函数返回值:void
函数说明:PB3:TIM2_CH2  PB4:TIM3_CH1

*/
void PWM_Open_door(void)
{
	TIM_Cmd(TIM2,ENABLE);
	TIM5_Init_DS_MS(300);
	TIM_Cmd(TIM2,DISABLE);
}


/*
函数功能: pwm输出关门
函数形参:void
函数返回值:void
函数说明:PB3:TIM2_CH2  PB4:TIM3_CH1

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
//	printf("TIM2中断服务");
//}


//void TIM3_IRQHandler(void)
//{
//	while(TIM_GetITStatus(TIM3,TIM_IT_CC1)==0);
//	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
////	printf("TIM3中断服务");

//}






