#ifndef __ADC_H
#define __ADC_H
#include "stm32f4xx.h"
#include "tim.h"
#include <stdio.h>




typedef enum{
	ADC_FREE=0,
	ADC_DMAIT,
	ADC_EOC,
}ADC1_FLAG;



extern float adc_tempture;
void DMA_ADC_Transfer_Init(void);
void ADC1_Init(void);


#endif 
