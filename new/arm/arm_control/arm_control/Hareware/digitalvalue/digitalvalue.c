#include "stm32f10x.h"
#include "digitalvalue.h"
#include "delay.h"

void digival_value_Init(void)//数字量端口初始化
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//使能PC端口时钟

	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;//端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz	 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	//初始化GPIOC
}
u8 digival_value_Scan(void)//数字量端口取样函数
{
if(DIGVAL==0) return 1;
	else        return 0;
}

