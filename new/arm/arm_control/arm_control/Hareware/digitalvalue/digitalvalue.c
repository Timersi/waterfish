#include "stm32f10x.h"
#include "digitalvalue.h"
#include "delay.h"

void digival_value_Init(void)//�������˿ڳ�ʼ��
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);//ʹ��PC�˿�ʱ��

	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;//�˿�����
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //��������
 //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz	 
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	//��ʼ��GPIOC
}
u8 digival_value_Scan(void)//�������˿�ȡ������
{
if(DIGVAL==0) return 1;
	else        return 0;
}

