#include "systick.h"


u8 key_task[2] = {0,5};
u8 rfid_task[2] = {0,5};
u8 mg200_task[2] = {0,5};

/*
 功能：SysTick_Init初始化中断方式
 形参:void
 说明:初始化,中断方式
  
*/
void SysTick_Init(u16 ms)
{
//	选择时钟源（选择了记一次数花的时间）
	SysTick->CTRL &= ~(0x1<<2);
//	给定重装载值（决定记多少次）
	SysTick->LOAD = 21*1000*ms -1;
//	写VAL(把重装载值加载到计数器里)
	SysTick->VAL = 0;
//	使能相关的中断使能位
	SysTick->CTRL |= 0x1<<1;
//	配置中断优先级
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(7-2,1,1));
//	使能系统定时器
	SysTick->CTRL |= 0x1<<0;
//	编写中断服务函数

	
}

//	编写中断服务函数
void SysTick_Handler(void)
{
	//清零标志位
	if(SysTick->CTRL & (0X1 << 16))
	{
		key_task[0]++;
		rfid_task[0]++;
		mg200_task[0]++;
	}	
}

///*
// 功能：SysTick_Init初始化中断方式
// 形参:void
// 说明:初始化,中断方式
//  最多写入798MS
//*/
//void SysTick_delay_ms(u16 ms)
//{
////	选择时钟源（选择了记一次数花的时间）
//	SysTick->CTRL &= ~(0x1<<2);
////	给定重装载值（决定记多少次）
//	SysTick->LOAD = 21*1000*ms -1;
////	写VAL(把重装载值加载到计数器里)
//	SysTick->VAL = 0;
////	使能系统定时器
//	SysTick->CTRL |= 0x1<<0;

////	while一直判断计数时间到的标志位置1的时候就是时间到了
//	while((SysTick->CTRL &(0x1<<16)) == 0);
////	关闭系统定时器
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



