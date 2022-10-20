#include "adc.h"

float adc_tempture =0;
u16 adc_buf[10]={0};

/*
��������:DMA�����ʼ��
�����β�:void
��������ֵ:void
����˵��:DMA:2 channel:4 Stream:5

*/
void DMA_ADC_Transfer_Init(void)
{
	//��ʱ��EN
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	DMA_InitTypeDef  DMA_InitStruct = {0}; 
	DMA_InitStruct.DMA_BufferSize = 10;//�ܹ����˶��ٴ�
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;//ѡ��ͨ��0
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�ڴ�
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;//ʹ��ֱ��ģʽ
	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)adc_buf;//�ڴ�Ļ���ַ
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//һ���ֽڴ�С
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//���ڴ�ĵ�ַ����
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;//ѭ��ģʽ
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);//����Ļ���ַ
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//һ���ֽڴ�С
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//������ĵ�ַ������
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	
	DMA_ClearFlag (DMA2_Stream0,DMA_IT_TCIF0);
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);  //��������ж�ʹ�ܣ���������һ�������ж�һ��
	//	�������ȼ�
	NVIC_SetPriority(DMA2_Stream0_IRQn,NVIC_EncodePriority(7-2,1,1));
	//	ʹ�ܺ�����Ӧ�ж�
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	
	
	DMA_Cmd(DMA2_Stream0,ENABLE);


}


/*
	����һ������������жϷ�����
*/
void DMA2_Stream0_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
//	printf("DMA�ж�");
	u8 j,k;
	u16 temp;
	for(j=0;j<9;j++)
	{
		for(k=0;k<9-j;k++)
		{
			if(adc_buf[k]>adc_buf[k+1])
			{
				temp = adc_buf[k];
				adc_buf[k] = adc_buf[k+1];
				adc_buf[k+1] = temp;
			}
		}
	}
	adc_buf[0] = 0;
	for(j=1;j<9;j++)
	{
		adc_buf[0] += adc_buf[j];
	}
	adc_tempture= ((( (adc_buf[0]/8) * (3.3/4095)-0.76)/0.0025)+25);
}


/*
��������:ADC��ʼ��
�����β�:void
��������ֵ:void
����˵��:PC13

*/
void ADC1_Init(void)
{
	//��ADC1��ʱ�� apb2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	
	
	//ͨ��ADC��ʼ��
	ADC_CommonInitTypeDef ADC_CommonInitStruct={0};
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //J��ֹDMA
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4; //��С��32Mhz  APB2Ϊ84Mhz
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //�����������ʱ��Ϊ5������
	ADC_CommonStructInit  ( & ADC_CommonInitStruct );
	
	
	//ADC1��ʼ��
	ADC_InitTypeDef   ADC_InitStruct = {0};
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; //����ת��
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; //�Ҷ���
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//�ر��ⲿ����ADCת��
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//����Ϊ��Ӳ�������޹�
	ADC_InitStruct.ADC_NbrOfConversion = 1;  //����ADCת��
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;  //����ɨ��ģʽ
	ADC_Init(ADC1,& ADC_InitStruct);
	

	//480+12 / 21M = 23us
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_480Cycles);
	ADC_TempSensorVrefintCmd(ENABLE); //ʹ���¶ȴ�����
	
//	//ADCת������ж�����
//	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
//	//�������ȼ�
//	NVIC_SetPriority(ADC_IRQn,NVIC_EncodePriority(7-2,0,1));
//	//	ʹ�ܺ�����Ӧ�ж�
//	NVIC_EnableIRQ(ADC_IRQn);
	
	
	ADC_Cmd(ADC1,ENABLE);//����ADC1
	ADC_SoftwareStartConv(ADC1);//�������ת��
	ADC_DMACmd(ADC1,ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);

}

//Adc1_rec adc1_rec={0};
/*
��������:ADC1�жϷ�����
�����β�:void
��������ֵ:void
����˵��:
*/
//void ADC_IRQHandler(void)
//{
////	if(ADC_GetITStatus(ADC1,ADC_IT_EOC))
////	{
////		 ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
////		 adc1_rec.flag = ADC_EOC;
////	}
//	u8 j,k;
//	u16 temp;
//	static u8 i=0;
//	if(ADC_GetITStatus(ADC1,ADC_IT_EOC))
//	{
////	if(adc1_rec.flag == ADC_EOC)
////	{
//		 ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
////		 adc1_rec.flag = ADC_FREE;
//		 adc_buf[i] = ADC_GetConversionValue(ADC1);
////		 adc_buf[i] =DMA_GetCurrDataCounter(DMA2_Stream0);
//		 i++;
//		 if(i == 10)
//		 {
//				i =0;
//			  temp = 0;
//				for(j=0;j<9;j++)
//				{
//					for(k=0;k<9-j;k++)
//					{
//						if(adc_buf[k]>adc_buf[k+1])
//						{
//							temp = adc_buf[k];
//							adc_buf[k] = adc_buf[k+1];
//							adc_buf[k+1] = temp;
//						}
//					}
//				}
//				adc_buf[0] = 0;
//				for(j=1;j<9;j++)
//				{
//					adc_buf[0] += adc_buf[j];
//				}
//				adc_tempture= ((( (adc_buf[0]/8) * (3.3/4095)-0.76)/0.0025)+25);
//		 }
//	 }
//}







