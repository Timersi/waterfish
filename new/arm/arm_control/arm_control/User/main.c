/************************************************************************
                舵机控制程序
								 
舵机控制周期限制在20ms，脉宽部分为0.5ms--2.5ms
  　0.5ms对应0度

　　1ms对应45度

　　1.5ms对应90度

　　2ms对应135度

　　2.5ms对应180度
************************************************************************/
#include "stm32f10x.h"
#include "smartcar.h"
#include "usart_2.h"
#include "pwm.h"
#include "delay.h"
#include "KEY.h"
#include "oled.h"
#include "digitalvalue.h"
 int main(void)
 {	
u16 numval31=150;u16 numval32=200;u16 numval33=200;u16 numval34=200;
u16 numval41=150;//u16 numval42=150;u16 numval43=150;u16 numval44=150;
u16 pwmval31=150;u16 pwmval32=200;u16 pwmval33=200;u16 pwmval34=200;	 
u16 pwmval41=150;u16 pwmval42=60;u16 pwmval43=150;u16 pwmval44=150;
	 
/*****************************************************/
	 
	 
/*****************************************************/
	 
vu8 key=0;	
vu8 num=1 ;	 
vu8 dig=0;		 
smartcar_Init();//智能小车GPIO初始化
delay_init();//延时函数初始化	  
KEY_Init(); //初始化与按键连接的硬件接口
digival_value_Init();	//数字开关输入初始化 
OLED_Init();			//初始化OLED  
OLED_Clear();
	  
TIM3_OC1_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz，定时器3的1通道初始化
TIM3_OC2_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz	 
TIM3_OC3_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz	 
TIM3_OC4_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz	 	 
TIM4_OC1_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz
TIM4_OC2_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz	 
TIM4_OC3_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz	 
TIM4_OC4_PWM_Init(1999,719);	 //720分频。PWM频率=72000000/2000*720=50hz	
	 
TIM_SetCompare1(TIM3,pwmval31);	//pwm输出函数	
TIM_SetCompare2(TIM3,pwmval32);	//pwm输出函数		 
TIM_SetCompare3(TIM3,pwmval33);	//pwm输出函数		 
TIM_SetCompare4(TIM3,pwmval34);	//pwm输出函数		 
TIM_SetCompare1(TIM4,pwmval41);	//pwm输出函数		 
TIM_SetCompare2(TIM4,pwmval42);	//pwm输出函数		 
TIM_SetCompare3(TIM4,pwmval43);	//pwm输出函数	
TIM_SetCompare4(TIM4,pwmval44);	//pwm输出函数	

   while(1) 
	{
		sprintf((char*)water_show,"water:%.1f%% %.0f ",tmp2,water_set);		
		OLED_ShowString(0,4,(u8*)water_show,16); 
		key=KEY_Scan(0);	//得到键值,0:不支持连续按，1：支持连续按.
		dig=digival_value_Scan();
	   	if(key)
		{						   
			switch(key)
			{	
      case KEYA_PRES://舵机按钮,左转
	
					switch(num)	
					{	
					case 1:numval31++; pwmval31=(numval31*1); if(numval31>260) numval31=260;TIM_SetCompare1(TIM3,pwmval31);  break;//
					case 2:numval32++; pwmval32=(numval32*1); if(numval32>250) numval32=250;TIM_SetCompare2(TIM3,pwmval32);  break;//
					case 3:numval33++; pwmval33=(numval33*1); if(numval33>250) numval33=250;TIM_SetCompare3(TIM3,pwmval33);  break;//
					case 4:numval34++; pwmval34=(numval34*1); if(numval34>250) numval34=250;TIM_SetCompare4(TIM3,pwmval34);  break;//
					case 5:numval41++; pwmval41=(numval41*1); if(numval41>250) numval41=250;TIM_SetCompare1(TIM4,pwmval41);  break;//
						
					} 		
					
					break;
					
			case KEYB_PRES://舵机按钮	，右转	
				
		    	switch(num)	
					{	
					case 1:numval31--; pwmval31=(numval31*1); if(numval31<40) numval31=50; TIM_SetCompare1(TIM3,pwmval31);  break;//
					case 2:numval32--; pwmval32=(numval32*1); if(numval32<55) numval32=55; TIM_SetCompare2(TIM3,pwmval32);  break;//
					case 3:numval33--; pwmval33=(numval33*1); if(numval33<55) numval33=55; TIM_SetCompare3(TIM3,pwmval33);  break;//
					case 4:numval34--; pwmval34=(numval34*1); if(numval34<55) numval34=55; TIM_SetCompare4(TIM3,pwmval34);  break;//
					case 5:numval41--; pwmval41=(numval41*1); if(numval41<55) numval41=55; TIM_SetCompare1(TIM4,pwmval41);  break;//		
					} 				
			    
		       break;
			
			case KEYC_PRES://选择哪个舵机运行+1
				   num++;
			if(num>5) num=5;
			break;
			
			case KEYD_PRES://选择哪个舵机运行-1 
				   num--;
			if(num<1) num=1;	    
			break; 
			
			case KEYE_PRES://机械手闭
					pwmval42++;
			if(pwmval42>145) pwmval42=145; 
			TIM_SetCompare2(TIM4,pwmval42); 
			break;
			
			case KEYF_PRES://机械手开
			pwmval42--;
			if(pwmval42<60)  pwmval42=60;  
			TIM_SetCompare2(TIM4,pwmval42); 		
			break;	
			}
	 }	

  /*   if(dig==1)
         {
					 u16 a=0;
			 numval32++; pwmval32=(numval32*1); if(numval32>200) numval32=200;TIM_SetCompare2(TIM3,pwmval32);	//2号舵机
			 delay_ms(a);
			 numval33++; pwmval33=(numval33*1); if(numval33>200) numval33=200;TIM_SetCompare3(TIM3,pwmval33);	//3号舵机		 
			 delay_ms(a);	 
			 numval34++; pwmval34=(numval34*1); if(numval34>250) numval34=250;TIM_SetCompare4(TIM3,pwmval34);//4号舵机			 
			 delay_ms(a);	 	 
			 pwmval42--;	if(pwmval42<55)   pwmval42=55;   TIM_SetCompare2(TIM4,pwmval42); //机械手闭	 
			 delay_ms(a);	 
			 numval34--; pwmval34=(numval34*1); if(numval34<150) numval34=150; TIM_SetCompare4(TIM3,pwmval34);//4号舵机			 
			 delay_ms(a);	 	  	 
       numval31--; pwmval31=(numval31*1); if(numval31<55) numval31=55; TIM_SetCompare1(TIM3,pwmval31);  //1号舵机
			 delay_ms(a);		 
				numval34++; pwmval34=(numval34*1); if(numval34>250) numval34=250;TIM_SetCompare4(TIM3,pwmval34);//4号舵机			 
			 delay_ms(a);	 	 	 
       pwmval42++; if(pwmval42>250) pwmval42=250; TIM_SetCompare2(TIM4,pwmval42); //机械手开
			 delay_ms(a);	 		 		 
			 numval31++; pwmval31=(numval31*1); if(numval31>150) numval31=150;TIM_SetCompare1(TIM3,pwmval31);//1号舵机		 
			 delay_ms(a);		 
				numval32--; pwmval32=(numval32*1); if(numval32<150) numval32=150; TIM_SetCompare2(TIM3,pwmval32);	 
         }  
	*/
		if(dig==1)

		{	
			vu8 a=1;//动作完成标志位
			u16 ms=500;
		
		if(a==1 ){ pwmval31=150; TIM_SetCompare1(TIM3,pwmval31); a=2 ;delay_ms(ms);}	 	
			
	  if(a==2) { pwmval32=150; TIM_SetCompare2(TIM3,pwmval32); a=3; delay_ms(ms);}
			 
		if(a==3) { pwmval33=200; TIM_SetCompare3(TIM3,pwmval33 );a=4; delay_ms(ms);}
		
		if(a==4) { pwmval34=250; TIM_SetCompare4(TIM3,pwmval34); a=5; delay_ms(ms);}
		
		if(a==5) { pwmval42=145; TIM_SetCompare2(TIM4,pwmval42); a=6; delay_ms(ms);}
		 
		if(a==6) { pwmval34=150; TIM_SetCompare4(TIM3,pwmval34); a=7; delay_ms(ms);}
		
		if(a==7) { pwmval31=55;  TIM_SetCompare1(TIM3,pwmval31); a=8; delay_ms(ms);}
		
		if(a==8) { pwmval34=250; TIM_SetCompare4(TIM3,pwmval34); a=9; delay_ms(ms);}
		
		if(a==9) { pwmval42=60;  TIM_SetCompare2(TIM4,pwmval42); a=10;delay_ms(ms);}
				
		if(a==10){ pwmval32=200; TIM_SetCompare2(TIM3,pwmval32); a=11;delay_ms(ms);}
		
		if(a==11){ pwmval31=150; TIM_SetCompare1(TIM3,pwmval31); a=1 ;delay_ms(ms);}
		}
 
		  

}
	
} 
           
