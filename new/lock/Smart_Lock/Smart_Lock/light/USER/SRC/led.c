#include "led.h"


/*
函数功能:LED8和LED9初始化
函数形参:void
函数返回值:void
函数说明:PB8和PB9

*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//打开外设时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init  (GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	LED1_OFF;
	LED2_OFF;
}


/*
函数功能:LED8和LED9换为呼吸道
函数形参:void
函数返回值:void
函数说明:PB8 TIM4_CH3 和PB9 TIM4_CH4

*/
void breath_led(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	TIM_OCInitTypeDef  TIM_OCInitStruct={0};

	//配置GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init  (GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init  (GPIOB, &GPIO_InitStruct);
	
	//PB8复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	//PB9
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
	
	//定时器时基配置
	//打开TIM4的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //递增计数
	TIM_TimeBaseInitStruct.TIM_Period = 10000 -1; //重装载值  表示记多少次
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1; //分频值 表示多长时间记一次（1us）
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct );
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 5000;
	TIM_OC3Init(TIM4,&TIM_OCInitStruct);
	TIM_OC4Init(TIM4,&TIM_OCInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_CC3,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE);
	//	设置优先级
	NVIC_SetPriority(TIM4_IRQn,NVIC_EncodePriority(7-2,1,1));
	//	使能核心响应中断
	NVIC_EnableIRQ(TIM4_IRQn);
	
	TIM_Cmd(TIM4,ENABLE);
}


void TIM4_IRQHandler(void)
{
	static u8 dir =0;
	static u16 val = 0;
	while(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)==0);
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	if(dir == 0)
	{
		val+=50;
		TIM_SetCompare3 (TIM4,val);
		TIM_SetCompare4 (TIM4,val);
		if(val >= 9900)
		{
			dir = 1;
		}
	}
	else
	{
		val -= 50;
		TIM_SetCompare3 (TIM4,val);
		TIM_SetCompare4 (TIM4,val);
		if(val <= 0)
		{
			dir = 0;
		}
	}

}







