#include "systick.h"


u8 key_task[2] = {0,5};
u8 rfid_task[2] = {0,5};
u8 mg200_task[2] = {0,5};

/*
 ���ܣ�SysTick_Init��ʼ���жϷ�ʽ
 �β�:void
 ˵��:��ʼ��,�жϷ�ʽ
  
*/
void SysTick_Init(u16 ms)
{
//	ѡ��ʱ��Դ��ѡ���˼�һ��������ʱ�䣩
	SysTick->CTRL &= ~(0x1<<2);
//	������װ��ֵ�������Ƕ��ٴΣ�
	SysTick->LOAD = 21*1000*ms -1;
//	дVAL(����װ��ֵ���ص���������)
	SysTick->VAL = 0;
//	ʹ����ص��ж�ʹ��λ
	SysTick->CTRL |= 0x1<<1;
//	�����ж����ȼ�
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,1,1));
//	ʹ��ϵͳ��ʱ��
	SysTick->CTRL |= 0x1<<0;
//	��д�жϷ�����

	
}

//	��д�жϷ�����
void SysTick_Handler(void)
{
	//�����־λ
	if(SysTick->CTRL & (0X1 << 16))
	{
		key_task[0]++;
		rfid_task[0]++;
		mg200_task[0]++;
	}	
}

///*
// ���ܣ�SysTick_Init��ʼ���жϷ�ʽ
// �β�:void
// ˵��:��ʼ��,�жϷ�ʽ
//  ���д��798MS
//*/
//void SysTick_delay_ms(u16 ms)
//{
////	ѡ��ʱ��Դ��ѡ���˼�һ��������ʱ�䣩
//	SysTick->CTRL &= ~(0x1<<2);
////	������װ��ֵ�������Ƕ��ٴΣ�
//	SysTick->LOAD = 21*1000*ms -1;
////	дVAL(����װ��ֵ���ص���������)
//	SysTick->VAL = 0;
////	ʹ��ϵͳ��ʱ��
//	SysTick->CTRL |= 0x1<<0;

////	whileһֱ�жϼ���ʱ�䵽�ı�־λ��1��ʱ�����ʱ�䵽��
//	while((SysTick->CTRL &(0x1<<16)) == 0);
////	�ر�ϵͳ��ʱ��
//	SysTick->CTRL &= ~(0x1<<0);
//	
//}


//void SysTick_delay_more_ms(u16 ms)
//{
//	u16 a,b;
//	a = ms/798;
//	b = ms%798;
//	while(a--)
//	{
//		SysTick_delay_ms(798);
//	}
//	if(b)
//	{
//		SysTick_delay_ms(b);
//	}
//}



