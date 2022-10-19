/************************************************************************
                ������Ƴ���
								 
�����������������20ms��������Ϊ0.5ms--2.5ms
  ��0.5ms��Ӧ0��

����1ms��Ӧ45��

����1.5ms��Ӧ90��

����2ms��Ӧ135��

����2.5ms��Ӧ180��
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
smartcar_Init();//����С��GPIO��ʼ��
delay_init();//��ʱ������ʼ��	  
KEY_Init(); //��ʼ���밴�����ӵ�Ӳ���ӿ�
digival_value_Init();	//���ֿ��������ʼ�� 
OLED_Init();			//��ʼ��OLED  
OLED_Clear();
	  
TIM3_OC1_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz����ʱ��3��1ͨ����ʼ��
TIM3_OC2_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz	 
TIM3_OC3_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz	 
TIM3_OC4_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz	 	 
TIM4_OC1_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz
TIM4_OC2_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz	 
TIM4_OC3_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz	 
TIM4_OC4_PWM_Init(1999,719);	 //720��Ƶ��PWMƵ��=72000000/2000*720=50hz	
	 
TIM_SetCompare1(TIM3,pwmval31);	//pwm�������	
TIM_SetCompare2(TIM3,pwmval32);	//pwm�������		 
TIM_SetCompare3(TIM3,pwmval33);	//pwm�������		 
TIM_SetCompare4(TIM3,pwmval34);	//pwm�������		 
TIM_SetCompare1(TIM4,pwmval41);	//pwm�������		 
TIM_SetCompare2(TIM4,pwmval42);	//pwm�������		 
TIM_SetCompare3(TIM4,pwmval43);	//pwm�������	
TIM_SetCompare4(TIM4,pwmval44);	//pwm�������	

   while(1) 
	{
		sprintf((char*)water_show,"water:%.1f%% %.0f ",tmp2,water_set);		
		OLED_ShowString(0,4,(u8*)water_show,16); 
		key=KEY_Scan(0);	//�õ���ֵ,0:��֧����������1��֧��������.
		dig=digival_value_Scan();
	   	if(key)
		{						   
			switch(key)
			{	
      case KEYA_PRES://�����ť,��ת
	
					switch(num)	
					{	
					case 1:numval31++; pwmval31=(numval31*1); if(numval31>260) numval31=260;TIM_SetCompare1(TIM3,pwmval31);  break;//
					case 2:numval32++; pwmval32=(numval32*1); if(numval32>250) numval32=250;TIM_SetCompare2(TIM3,pwmval32);  break;//
					case 3:numval33++; pwmval33=(numval33*1); if(numval33>250) numval33=250;TIM_SetCompare3(TIM3,pwmval33);  break;//
					case 4:numval34++; pwmval34=(numval34*1); if(numval34>250) numval34=250;TIM_SetCompare4(TIM3,pwmval34);  break;//
					case 5:numval41++; pwmval41=(numval41*1); if(numval41>250) numval41=250;TIM_SetCompare1(TIM4,pwmval41);  break;//
						
					} 		
					
					break;
					
			case KEYB_PRES://�����ť	����ת	
				
		    	switch(num)	
					{	
					case 1:numval31--; pwmval31=(numval31*1); if(numval31<40) numval31=50; TIM_SetCompare1(TIM3,pwmval31);  break;//
					case 2:numval32--; pwmval32=(numval32*1); if(numval32<55) numval32=55; TIM_SetCompare2(TIM3,pwmval32);  break;//
					case 3:numval33--; pwmval33=(numval33*1); if(numval33<55) numval33=55; TIM_SetCompare3(TIM3,pwmval33);  break;//
					case 4:numval34--; pwmval34=(numval34*1); if(numval34<55) numval34=55; TIM_SetCompare4(TIM3,pwmval34);  break;//
					case 5:numval41--; pwmval41=(numval41*1); if(numval41<55) numval41=55; TIM_SetCompare1(TIM4,pwmval41);  break;//		
					} 				
			    
		       break;
			
			case KEYC_PRES://ѡ���ĸ��������+1
				   num++;
			if(num>5) num=5;
			break;
			
			case KEYD_PRES://ѡ���ĸ��������-1 
				   num--;
			if(num<1) num=1;	    
			break; 
			
			case KEYE_PRES://��е�ֱ�
					pwmval42++;
			if(pwmval42>145) pwmval42=145; 
			TIM_SetCompare2(TIM4,pwmval42); 
			break;
			
			case KEYF_PRES://��е�ֿ�
			pwmval42--;
			if(pwmval42<60)  pwmval42=60;  
			TIM_SetCompare2(TIM4,pwmval42); 		
			break;	
			}
	 }	

  /*   if(dig==1)
         {
					 u16 a=0;
			 numval32++; pwmval32=(numval32*1); if(numval32>200) numval32=200;TIM_SetCompare2(TIM3,pwmval32);	//2�Ŷ��
			 delay_ms(a);
			 numval33++; pwmval33=(numval33*1); if(numval33>200) numval33=200;TIM_SetCompare3(TIM3,pwmval33);	//3�Ŷ��		 
			 delay_ms(a);	 
			 numval34++; pwmval34=(numval34*1); if(numval34>250) numval34=250;TIM_SetCompare4(TIM3,pwmval34);//4�Ŷ��			 
			 delay_ms(a);	 	 
			 pwmval42--;	if(pwmval42<55)   pwmval42=55;   TIM_SetCompare2(TIM4,pwmval42); //��е�ֱ�	 
			 delay_ms(a);	 
			 numval34--; pwmval34=(numval34*1); if(numval34<150) numval34=150; TIM_SetCompare4(TIM3,pwmval34);//4�Ŷ��			 
			 delay_ms(a);	 	  	 
       numval31--; pwmval31=(numval31*1); if(numval31<55) numval31=55; TIM_SetCompare1(TIM3,pwmval31);  //1�Ŷ��
			 delay_ms(a);		 
				numval34++; pwmval34=(numval34*1); if(numval34>250) numval34=250;TIM_SetCompare4(TIM3,pwmval34);//4�Ŷ��			 
			 delay_ms(a);	 	 	 
       pwmval42++; if(pwmval42>250) pwmval42=250; TIM_SetCompare2(TIM4,pwmval42); //��е�ֿ�
			 delay_ms(a);	 		 		 
			 numval31++; pwmval31=(numval31*1); if(numval31>150) numval31=150;TIM_SetCompare1(TIM3,pwmval31);//1�Ŷ��		 
			 delay_ms(a);		 
				numval32--; pwmval32=(numval32*1); if(numval32<150) numval32=150; TIM_SetCompare2(TIM3,pwmval32);	 
         }  
	*/
		if(dig==1)

		{	
			vu8 a=1;//������ɱ�־λ
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
           
