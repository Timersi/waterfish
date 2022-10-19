#include "stm32f10x.h"
#include "KEY.h"
#include "delay.h"

void KEY_Init (void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PA端口时钟

	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//端口配置
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //下拉输入
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz	 
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	//初始化GPIOA
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEYA按下
//2，KEYB按下
//3，KEYC按下 
//4，KEYD按下 
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!


u8 KEY_Scan(u8 mode)
{	  
  static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEYA==1||KEYB==1||KEYC==1||KEYD==1||KEYE==1||KEYF==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		     if(KEYA==1){key_up=1; return KEYA_PRES; }//支持连续按
		else if(KEYB==1){key_up=1; return KEYB_PRES; }//支持连续按
		else if(KEYC==1)return KEYC_PRES;
		else if(KEYD==1)return KEYD_PRES;
		else if(KEYE==1){key_up=1; return KEYE_PRES;}//支持连续按
		else if(KEYF==1){key_up=1; return KEYF_PRES;}//支持连续按		
	}
	  else if(KEYA==0&&KEYB==0&&KEYC==0&&KEYD==0&&KEYE==0&&KEYF==0)key_up=1;	    
   	return 0;// 无按键按下
}
