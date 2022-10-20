#include "stm32f10x.h"
#include "smartcar.h"
void smartcar_Init (void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);//ʹ��PB�˿�ʱ��
	 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz	 
  GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��GPIOB
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//�˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
 // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��GPIOB
	
	 
  GPIO_ResetBits(GPIOB,GPIO_Pin_5);//INA4 �����
  GPIO_ResetBits(GPIOB,GPIO_Pin_6);//INA3 �����
  GPIO_ResetBits(GPIOB,GPIO_Pin_7);//INA2 �����
  GPIO_ResetBits(GPIOB,GPIO_Pin_8);//INA1 �����
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);//INB4 �����
  GPIO_ResetBits(GPIOB,GPIO_Pin_13);//INB3 �����
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);//INB2 �����
  GPIO_ResetBits(GPIOB,GPIO_Pin_15);//INB1 �����
	
	
	 
}

