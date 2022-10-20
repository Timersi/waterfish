#include "voice.h"


/*
��������:Voice��ʼ��
�����β�:void
��������ֵ:void
����˵��:PB2��PA0

*/
void Voice_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct={0};
	//������ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init  (GPIOC, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	VOICE_SCL_L;

}

/*
��������:VOICE��ģ��
�����β�:u8 data
��������ֵ:void
����˵��:PB2��PA0

*/
void Voice_Output(u8 data)
{
	u8 i;
	VOICE_SCL_H;
	TIM5_Init_DS_MS(8);
	VOICE_SCL_L;
	TIM5_Init_DS_MS(1);
	for(i=0;i<8;i++)
	{
		if(data & (0x80>>i))
		{
			VOICE_SCL_H;
			TIM5_Init_DS_US(1500);
			VOICE_SCL_L;
			TIM5_Init_DS_US(500);
		}
		else
		{
			VOICE_SCL_H;
			TIM5_Init_DS_US(500);
			VOICE_SCL_L;
			TIM5_Init_DS_US(1500);
		}
	}
//	TIM5_Init_DS_US(200);
//	VOICE_SDA_H;
//	TIM5_Init_DS_MS(8);
//	VOICE_SDA_L;
//	TIM5_Init_DS_MS(1);
}






