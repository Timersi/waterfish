#include "tim.h"


/*
��������:TIM5��ʼ����ʱMS
�����β�:nms
��������ֵ:void
����˵��:PB8��PB9

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
��������:TIM5��ʼ����ʱMS
�����β�:nms
��������ֵ:void
����˵��:PB8��PB9

*/
void TIM5_Init_DS_US(u32 nus)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = nus  -1;  //Ԥ��Ƶ�Ĵ�����ֵ������ü�һ����
	TIM_TimeBaseInitStruct.TIM_Prescaler = 84-1;	//�Զ���װ�ؼĴ�����ֵ�����������ٴ�
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStruct );
	
	TIM_Cmd(TIM5,ENABLE);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	while(TIM_GetFlagStatus(TIM5,TIM_FLAG_Update)==0);
	
}








