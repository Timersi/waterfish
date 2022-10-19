#ifndef __pwm_h
#define __pwm_h
#include "stm32f10x.h"
void TIM3_OC1_PWM_Init(u16 arr31,u16 psc31);
void TIM3_OC2_PWM_Init(u16 arr32,u16 psc32);
void TIM3_OC3_PWM_Init(u16 arr33,u16 psc33);
void TIM3_OC4_PWM_Init(u16 arr34,u16 psc34);
void TIM4_OC1_PWM_Init(u16 arr41,u16 psc41);
void TIM4_OC2_PWM_Init(u16 arr42,u16 psc42);
void TIM4_OC3_PWM_Init(u16 arr43,u16 psc43);
void TIM4_OC4_PWM_Init(u16 arr44,u16 psc44);
#endif
