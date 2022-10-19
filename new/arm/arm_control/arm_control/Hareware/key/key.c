#include "stm32f10x.h"
#include "KEY.h"
#include "delay.h"

void KEY_Init (void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PA�˿�ʱ��

	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//�˿�����
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz	 
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIOA
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEYA����
//2��KEYB����
//3��KEYC���� 
//4��KEYD���� 
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!


u8 KEY_Scan(u8 mode)
{	  
  static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEYA==1||KEYB==1||KEYC==1||KEYD==1||KEYE==1||KEYF==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		     if(KEYA==1){key_up=1; return KEYA_PRES; }//֧��������
		else if(KEYB==1){key_up=1; return KEYB_PRES; }//֧��������
		else if(KEYC==1)return KEYC_PRES;
		else if(KEYD==1)return KEYD_PRES;
		else if(KEYE==1){key_up=1; return KEYE_PRES;}//֧��������
		else if(KEYF==1){key_up=1; return KEYF_PRES;}//֧��������		
	}
	  else if(KEYA==0&&KEYB==0&&KEYC==0&&KEYD==0&&KEYE==0&&KEYF==0)key_up=1;	    
   	return 0;// �ް�������
}
