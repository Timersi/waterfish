#include "pwm.h"
#include "stm32f10x.h"

/**********************************************
          ��ʱ��3��ͨ��1���pwm��ʼ��
************************************************/
void TIM3_OC1_PWM_Init(u16 arr31,u16 psc31)	// ��ʱ��3��ͨ��1���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��1��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.4��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	��ʱ��3ʱ��ʹ��

/*�˿���ӳ��*/	
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//����ʱ��3��ͨ��1��PA6��ӳ�䵽PB4��
	
/*��ʱ��3��ʼ��*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr31;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc31;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʱ��3��ʼ��

/*ͨ��1��ʼ������*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC1Init(TIM3,&TIM_OCInitStructure);//ͨ��1��ʼ��

TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*ʹ��SWD ����JTAG*/

GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);///*SWDʹ�ܡ�JTAGʹ�ܵ��ǲ�����JNRST���� */
}	

/**********************************************
          ��ʱ��3��ͨ��2���pwm��ʼ��
************************************************/
void TIM3_OC2_PWM_Init(u16 arr32,u16 psc32)	// ��ʱ��3��ͨ��2���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��2��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.5��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	��ʱ��3ʱ��ʹ��

/*�˿���ӳ��*/	
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//����ʱ��3��ͨ��2��PA7��ӳ�䵽PB5��
	
/*��ʱ��3��ʼ��*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr32;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc32;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʱ��3��ʼ��

/*ͨ��2��ʼ������*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC2Init(TIM3,&TIM_OCInitStructure);//ͨ��2��ʼ��

TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
}	

/**********************************************
          ��ʱ��3��ͨ��3���pwm��ʼ��
************************************************/
void TIM3_OC3_PWM_Init(u16 arr33,u16 psc33)	// ��ʱ��3��ͨ��2���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��2��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.0��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	��ʱ��3ʱ��ʹ��


/*��ʱ��3��ʼ��*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr33;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc33;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʱ��3��ʼ��

/*ͨ��3��ʼ������*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC3Init(TIM3,&TIM_OCInitStructure);//ͨ��3��ʼ��

TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
}	

/**********************************************
          ��ʱ��3��ͨ��4���pwm��ʼ��
************************************************/
void TIM3_OC4_PWM_Init(u16 arr34,u16 psc34)	// ��ʱ��3��ͨ��2���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��2��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.1��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	��ʱ��3ʱ��ʹ��


/*��ʱ��3��ʼ��*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr34;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc34;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʱ��3��ʼ��

/*ͨ��3��ʼ������*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC4Init(TIM3,&TIM_OCInitStructure);//ͨ��4��ʼ��

TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable); //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM3,ENABLE);//ʹ�ܶ�ʱ��3
}	


/*****************************************************************************************************
                           ��ʱ��4��ͨ��1���pwm��ʼ��
******************************************************************************************************/
void TIM4_OC1_PWM_Init(u16 arr41,u16 psc41)//��ʱ��4��ͨ��1���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��2��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.6��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	��ʱ��4ʱ��ʹ��
	
/*��ʱ��4��ʼ��*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr41;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc41;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʱ��4��ʼ��

/*ͨ��1��ʼ������*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC1Init(TIM4,&TIM_OCInitStructure);//ͨ��1��ʼ��

TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��4

}	


/*****************************************************************************************************
                           ��ʱ��4��ͨ��2���pwm��ʼ��
******************************************************************************************************/
void TIM4_OC2_PWM_Init(u16 arr42,u16 psc42)//��ʱ��4��ͨ��2���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��2��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.7��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	��ʱ��4ʱ��ʹ��
	
/*��ʱ��4��ʼ��*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr42;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc42;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʱ��4��ʼ��

/*ͨ��1��ʼ������*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC2Init(TIM4,&TIM_OCInitStructure);//ͨ��2��ʼ��

TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��4

}	


/*****************************************************************************************************
                           ��ʱ��4��ͨ��3���pwm��ʼ��
******************************************************************************************************/
void TIM4_OC3_PWM_Init(u16 arr43,u16 psc43)//��ʱ��4��ͨ��3���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��2��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.8��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	��ʱ��4ʱ��ʹ��
	
/*��ʱ��4��ʼ��*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr43;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc43;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʱ��4��ʼ��

/*ͨ��1��ʼ������*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC3Init(TIM4,&TIM_OCInitStructure);//ͨ��3��ʼ��

TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��4

}	


/*****************************************************************************************************
                           ��ʱ��4��ͨ��4���pwm��ʼ��
******************************************************************************************************/
void TIM4_OC4_PWM_Init(u16 arr44,u16 psc44)//��ʱ��4��ͨ��4���pwm��ʼ��
{
//˵��������ṹ�������ں���ʼ�˽�����������Ȼ�������ᱨ��
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO�˿ڳ�ʼ���ṹ��  �ṹ����	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//��ʱ��3��ʼ���ṹ��  �ṹ����	
TIM_OCInitTypeDef TIM_OCInitStructure;//ͨ��4��ʼ���ṹ��  �ṹ����	
	
/*	GPIO�˿ڳ�ʼ��*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//�˿�
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//�ٶ�
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	ģʽ�������������	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.9��ʼ��

/*ʱ��ʹ��*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOBʱ��ʹ�ܣ��˿ڸ��ù��ܶ˿�ʱ��ʹ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	��ʱ��4ʱ��ʹ��
	
/*��ʱ��4��ʼ��*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//��ƵģʽΪ����Ƶ
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//ģʽΪ���ϼ���ģʽ
TIM_TimeBaseInitStructure.TIM_Period=	arr44;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
TIM_TimeBaseInitStructure.TIM_Prescaler=psc44;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʱ��4��ʼ��

/*ͨ��1��ʼ������*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//ģʽΪpwmģʽ1��ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//�������Ϊ��
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////�Ƚ����ʹ��
TIM_OC4Init(TIM4,&TIM_OCInitStructure);//ͨ��4��ʼ��

TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); //ʹ��TIM4��CCR2�ϵ�Ԥװ�ؼĴ���

TIM_Cmd(TIM4,ENABLE);//ʹ�ܶ�ʱ��4

}	






