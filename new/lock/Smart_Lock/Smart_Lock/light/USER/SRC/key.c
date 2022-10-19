#include "key.h"


/*
��������:������ʼ��
�����β�:void
��������ֵ:void
����˵��:PC13

*/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	//������ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Low_Speed;;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/*
��������:�������
�����β�:void
��������ֵ:u8 0/1
����˵��:PC13

*/
u8 Key_Scan(void)
{
	static u8 key_flag = 1;
	u32 cnt = 840000;
	if((KEY1==0) && key_flag)
	{
		while(cnt--);
		if(KEY1==0)
		{
			key_flag = 0;
			return 1;
		}
	}
	else if((KEY1))
	{
		if(KEY1)
		{
			key_flag = 1;
		}
	}
	return 0;
}










