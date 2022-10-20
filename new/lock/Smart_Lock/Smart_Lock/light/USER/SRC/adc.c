#include "adc.h"

float adc_tempture =0;
u16 adc_buf[10]={0};

/*
函数功能:DMA传输初始化
函数形参:void
函数返回值:void
函数说明:DMA:2 channel:4 Stream:5

*/
void DMA_ADC_Transfer_Init(void)
{
	//打开时钟EN
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	DMA_InitTypeDef  DMA_InitStruct = {0}; 
	DMA_InitStruct.DMA_BufferSize = 10;//总共搬运多少次
	DMA_InitStruct.DMA_Channel = DMA_Channel_0;//选择通道0
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到内存
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;//使用直接模式
	DMA_InitStruct.DMA_Memory0BaseAddr = (u32)adc_buf;//内存的基地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//一个字节大小
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//让内存的地址递增
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;//循环模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&(ADC1->DR);//外设的基地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//一个字节大小
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//让外设的地址不递增
	DMA_Init(DMA2_Stream0, &DMA_InitStruct);
	
	DMA_ClearFlag (DMA2_Stream0,DMA_IT_TCIF0);
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);  //传输完成中断使能，即传输完一个扇区中断一次
	//	设置优先级
	NVIC_SetPriority(DMA2_Stream0_IRQn,NVIC_EncodePriority(7-2,1,1));
	//	使能核心响应中断
	NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	
	
	DMA_Cmd(DMA2_Stream0,ENABLE);


}


/*
	传输一个缓冲区完成中断服务函数
*/
void DMA2_Stream0_IRQHandler(void)
{
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
//	printf("DMA中断");
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
函数功能:ADC初始化
函数形参:void
函数返回值:void
函数说明:PC13

*/
void ADC1_Init(void)
{
	//打开ADC1的时钟 apb2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	
	
	//通用ADC初始化
	ADC_CommonInitTypeDef ADC_CommonInitStruct={0};
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //J禁止DMA
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4; //需小于32Mhz  APB2为84Mhz
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //采样间隔周期时间为5个周期
	ADC_CommonStructInit  ( & ADC_CommonInitStruct );
	
	
	//ADC1初始化
	ADC_InitTypeDef   ADC_InitStruct = {0};
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; //连续转换
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; //右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;//关闭外部触发ADC转化
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//配置为和硬件触发无关
	ADC_InitStruct.ADC_NbrOfConversion = 1;  //单个ADC转换
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;  //开启扫描模式
	ADC_Init(ADC1,& ADC_InitStruct);
	

	//480+12 / 21M = 23us
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_480Cycles);
	ADC_TempSensorVrefintCmd(ENABLE); //使能温度传感器
	
//	//ADC转换完成中断配置
//	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
//	//设置优先级
//	NVIC_SetPriority(ADC_IRQn,NVIC_EncodePriority(7-2,0,1));
//	//	使能核心响应中断
//	NVIC_EnableIRQ(ADC_IRQn);
	
	
	ADC_Cmd(ADC1,ENABLE);//开启ADC1
	ADC_SoftwareStartConv(ADC1);//软件触发转换
	ADC_DMACmd(ADC1,ENABLE);
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);

}

//Adc1_rec adc1_rec={0};
/*
函数功能:ADC1中断服务函数
函数形参:void
函数返回值:void
函数说明:
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







