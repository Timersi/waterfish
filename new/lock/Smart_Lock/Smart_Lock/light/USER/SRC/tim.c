#include "tim.h"


/*
函数功能:TIM5初始化延时MS
函数形参:nms
函数返回值:void
函数说明:PB8和PB9

*/
void TIM5_Init_DS_MS(u32 nms)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = nms *1000 -1;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct );
	
	TIM_Cmd(TIM5,ENABLE);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==0);
	
}


/*
函数功能:TIM5初始化延时MS
函数形参:nms
函数返回值:void
函数说明:PB8和PB9

*/
void TIM5_Init_DS_US(u32 nus)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = nus  -1;  //预分频寄存器的值决定多久记一次数
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;	//自动重装载寄存器的值决定计数多少次
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct );
	
	TIM_Cmd(TIM5,ENABLE);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==0);
	
}








