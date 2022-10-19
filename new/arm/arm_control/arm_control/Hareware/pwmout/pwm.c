#include "pwm.h"
#include "stm32f10x.h"

/**********************************************
          定时器3的通道1输出pwm初始化
************************************************/
void TIM3_OC1_PWM_Init(u16 arr31,u16 psc31)	// 定时器3的通道1输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道1初始化结构体  结构体名	
	
/*	GPIO端口初始化*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.4初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	定时器3时钟使能

/*端口重映射*/	
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//将定时器3的通道1（PA6重映射到PB4）
	
/*定时器3初始化*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr31;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc31;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//定时器3初始化

/*通道1初始化函数*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC1Init(TIM3,&TIM_OCInitStructure);//通道1初始化

TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIM3在CCR2上的预装载寄存器

TIM_Cmd(TIM3,ENABLE);//使能定时器3
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);      /*使能SWD 禁用JTAG*/

GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);///*SWD使能、JTAG使能但是不包括JNRST引脚 */
}	

/**********************************************
          定时器3的通道2输出pwm初始化
************************************************/
void TIM3_OC2_PWM_Init(u16 arr32,u16 psc32)	// 定时器3的通道2输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道2初始化结构体  结构体名	
	
/*	GPIO端口初始化*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.5初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	定时器3时钟使能

/*端口重映射*/	
GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);//将定时器3的通道2（PA7重映射到PB5）
	
/*定时器3初始化*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr32;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc32;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//定时器3初始化

/*通道2初始化函数*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC2Init(TIM3,&TIM_OCInitStructure);//通道2初始化

TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIM3在CCR2上的预装载寄存器

TIM_Cmd(TIM3,ENABLE);//使能定时器3
}	

/**********************************************
          定时器3的通道3输出pwm初始化
************************************************/
void TIM3_OC3_PWM_Init(u16 arr33,u16 psc33)	// 定时器3的通道2输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道2初始化结构体  结构体名	
	
/*	GPIO端口初始化*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.0初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	定时器3时钟使能


/*定时器3初始化*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr33;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc33;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//定时器3初始化

/*通道3初始化函数*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC3Init(TIM3,&TIM_OCInitStructure);//通道3初始化

TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIM3在CCR2上的预装载寄存器

TIM_Cmd(TIM3,ENABLE);//使能定时器3
}	

/**********************************************
          定时器3的通道4输出pwm初始化
************************************************/
void TIM3_OC4_PWM_Init(u16 arr34,u16 psc34)	// 定时器3的通道2输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道2初始化结构体  结构体名	
	
/*	GPIO端口初始化*/

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.1初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);//	定时器3时钟使能


/*定时器3初始化*/		

TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr34;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc34;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//定时器3初始化

/*通道3初始化函数*/	

TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC4Init(TIM3,&TIM_OCInitStructure);//通道4初始化

TIM_OC4PreloadConfig(TIM3,TIM_OCPreload_Enable); //使能TIM3在CCR2上的预装载寄存器

TIM_Cmd(TIM3,ENABLE);//使能定时器3
}	


/*****************************************************************************************************
                           定时器4的通道1输出pwm初始化
******************************************************************************************************/
void TIM4_OC1_PWM_Init(u16 arr41,u16 psc41)//定时器4的通道1输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道2初始化结构体  结构体名	
	
/*	GPIO端口初始化*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.6初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	定时器4时钟使能
	
/*定时器4初始化*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr41;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc41;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//定时器4初始化

/*通道1初始化函数*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC1Init(TIM4,&TIM_OCInitStructure);//通道1初始化

TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIM4在CCR2上的预装载寄存器

TIM_Cmd(TIM4,ENABLE);//使能定时器4

}	


/*****************************************************************************************************
                           定时器4的通道2输出pwm初始化
******************************************************************************************************/
void TIM4_OC2_PWM_Init(u16 arr42,u16 psc42)//定时器4的通道2输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道2初始化结构体  结构体名	
	
/*	GPIO端口初始化*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.7初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	定时器4时钟使能
	
/*定时器4初始化*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr42;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc42;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//定时器4初始化

/*通道1初始化函数*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC2Init(TIM4,&TIM_OCInitStructure);//通道2初始化

TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIM4在CCR2上的预装载寄存器

TIM_Cmd(TIM4,ENABLE);//使能定时器4

}	


/*****************************************************************************************************
                           定时器4的通道3输出pwm初始化
******************************************************************************************************/
void TIM4_OC3_PWM_Init(u16 arr43,u16 psc43)//定时器4的通道3输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道2初始化结构体  结构体名	
	
/*	GPIO端口初始化*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.8初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	定时器4时钟使能
	
/*定时器4初始化*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr43;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc43;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//定时器4初始化

/*通道1初始化函数*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC3Init(TIM4,&TIM_OCInitStructure);//通道3初始化

TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIM4在CCR2上的预装载寄存器

TIM_Cmd(TIM4,ENABLE);//使能定时器4

}	


/*****************************************************************************************************
                           定时器4的通道4输出pwm初始化
******************************************************************************************************/
void TIM4_OC4_PWM_Init(u16 arr44,u16 psc44)//定时器4的通道4输出pwm初始化
{
//说明：定义结构体必须放在函数始端进行声明，不然编译器会报错
	
GPIO_InitTypeDef  GPIO_InitStructure;//GPIO端口初始化结构体  结构体名	
TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;//定时器3初始化结构体  结构体名	
TIM_OCInitTypeDef TIM_OCInitStructure;//通道4初始化结构体  结构体名	
	
/*	GPIO端口初始化*/
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//端口
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//速度
GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;//	模式：复用推挽输出	
GPIO_Init(GPIOB,&GPIO_InitStructure);//GPIOB.9初始化

/*时钟使能*/	
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);//GOIOB时钟使能，端口复用功能端口时钟使能
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);//	定时器4时钟使能
	
/*定时器4初始化*/		
TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//分频模式为不分频
TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;	//模式为向上计数模式
TIM_TimeBaseInitStructure.TIM_Period=	arr44;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
TIM_TimeBaseInitStructure.TIM_Prescaler=psc44;//设置用来作为TIMx时钟频率除数的预分频值 
TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//定时器4初始化

/*通道1初始化函数*/	
TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//模式为pwm模式1，选择定时器模式:TIM脉冲宽度调制模式1
TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性为高
TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;////比较输出使能
TIM_OC4Init(TIM4,&TIM_OCInitStructure);//通道4初始化

TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); //使能TIM4在CCR2上的预装载寄存器

TIM_Cmd(TIM4,ENABLE);//使能定时器4

}	






