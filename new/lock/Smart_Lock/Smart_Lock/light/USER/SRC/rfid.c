#include "rfid.h"


/*
函数功能:RFID初始化
函数形参:void
函数返回值:void
函数说明:PA15: RFID_SPI3_CS   片选线
					PC10: SPI3_SCK       时钟线
					PC11:MISO      			 主入从出
					PC12: SPI3_MOSI      主出从入 
					PD2:RFID_SPI3_RST    复位

*/
void RFID_SPI_GPIO_Init(void)
{
	//GPIO初始化
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD, ENABLE);//1.开时钟
	
	//PC10--SCL--推挽输出,PC12--MOSI--推挽输出
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//PA15--CS--推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//PD2--RST--推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//PC11--MISO--浮空输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
//	GPIO_InitTypeDef GPIO_InitStruct={0};

//	//配置GPIO
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
//	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_11;
//	GPIO_Init  (GPIOC, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
//	GPIO_Init  (GPIOD, &GPIO_InitStruct);

//	//PA15
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
//	GPIO_InitStruct.GPIO_Mode =  GPIO_Mode_OUT;
//	GPIO_Init  (GPIOA, &GPIO_InitStruct);
//	
//	//PC10
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_SPI3);
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_SPI3);
//	
//	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_SPI3);
//	
//	GPIO_SetBits(GPIOD, GPIO_Pin_2);
//	GPIO_SetBits(GPIOA,GPIO_Pin_15);
//	
//	
//	SPI_InitTypeDef   SPI_InitStruct;
//	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
//	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
//	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
//	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
//	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB ;
//	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
//	SPI_Init (SPI3,&SPI_InitStruct);
//	SPI_Cmd(SPI3,ENABLE);
}

u8 SPI3_Sent_byte(u8 data)
{
	u8 i,buf = 0;
	SPI3_SCL_H;
	for(i = 0; i < 8; i++)
	{
		SPI3_SCL_L;
		if(data & (0x80 >> i))
		{
			RIFD_MOSI_H;
		}
		else
		{
			RIFD_MOSI_L;
		}
		SPI3_SCL_H;
		buf <<= 1;
		if(RIFD_MISO)
			buf |= 1;
	}
	return buf;
//	u8 buf=0;
//	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==0);
//	SPI_I2S_SendData(SPI3,data);
//	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==0);
//	buf = SPI_I2S_ReceiveData(SPI3);
//	return buf;
}





//读RC632寄存器
//Address:寄存器地址
u8 ReadRawRC(u8 Address)
{
	u8 ucAddr;
	u8 ucResult=0;
	CS_L;
	ucAddr = ((Address<<1)&0x7E)|0x80;

	SPI3_Sent_byte(ucAddr);
	ucResult=SPI3_Sent_byte(0);
	CS_H;
	return ucResult;
}


//写RC632寄存器
//Address[IN]:寄存器地址
//value[IN]:写入的值
void WriteRawRC(uint8_t Address, uint8_t value)
{  
    uint8_t ucAddr;

	CS_L;
    ucAddr = ((Address<<1)&0x7E);//0111 1110

	SPI3_Sent_byte(ucAddr);
	SPI3_Sent_byte(value);
	CS_H;
}

//清寄存器位
void ClearBitMask(u8 reg,u8 mask) 
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

//置RC522寄存器位
//reg[IN]:寄存器地址
//mask[IN]:置位值
void SetBitMask(u8 reg,u8 mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}


//复位RC522
char PcdReset(void)
{
	RIFD_RES_H;
	TIM5_Init_DS_US(2);
	
	RIFD_RES_L;
	TIM5_Init_DS_US(2);
	
	RIFD_RES_H;
	TIM5_Init_DS_US(2);
	WriteRawRC(CommandReg,PCD_RESETPHASE);
	WriteRawRC(CommandReg,PCD_RESETPHASE);
	TIM5_Init_DS_US(2);
    
	WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);
	WriteRawRC(TPrescalerReg,0x3E);
	
	WriteRawRC(TxAutoReg,0x40);//必须要
   
   return MI_OK;
	
}

//关闭天线
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

//开启天线
void PcdAntennaOn(void)
{
    u8   i;
    i = ReadRawRC(TxControlReg);
    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}


void Reset_RC522(void)
{
	PcdReset();       //功    能：复位RC522
	PcdAntennaOff();  //关闭天线
	PcdAntennaOn();   //开启天线
}    

//设置RC522工作模式
char M500PcdConfigISOType(u8 type)
{
   if (type == 'A')                     //ISO14443_A
   { 
			ClearBitMask(Status2Reg,0x08);
			WriteRawRC(ModeReg,0x3D);//3F
			WriteRawRC(RxSelReg,0x86);//84
			WriteRawRC(RFCfgReg,0x7F);   //4F
			WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
			WriteRawRC(TReloadRegH,0);
			WriteRawRC(TModeReg,0x8D);
			WriteRawRC(TPrescalerReg,0x3E);
			TIM5_Init_DS_US(2);
			PcdAntennaOn();
   }
   else{ return 1; }
   
   return MI_OK;
}

void RFID_Init(void)
{
	RFID_SPI_GPIO_Init();
	Reset_RC522();
	M500PcdConfigISOType('A');
}

/*******************************************************************
* 函数名：     PcdComMF522()
* 功能描述 ：  通过RC522和ISO14443卡通讯
* 作者：         谢国卿 
* 参数说明:   Command[IN]:RC522命令字
							pIn [IN]:通过RC522发送到卡片的数据
							InLenByte[IN]:发送数据的字节长度
							pOut [OUT]:接收到的卡片返回数据
							*pOutLenBit[OUT]:返回数据的位长度
* 返回值说明：
* 修改记录：
* 其他：
*******************************************************************/
char PcdComMF522(u8   Command, 
                 u8 *pIn , 
                 u8   InLenByte,
                 u8 *pOut , 
                 u8 *pOutLenBit)
{
    char   status = MI_ERR;
    u8   irqEn   = 0x00;
    u8   waitFor = 0x00;
    u8   lastBits;
    u8   n;
    u16   i;
    switch (Command)
    {
        case PCD_AUTHENT:
													irqEn   = 0x12;
													waitFor = 0x10;
													break;
				case PCD_TRANSCEIVE:
													irqEn   = 0x77;
													waitFor = 0x30;
													break;
				default:	break;
    }
   
    WriteRawRC(ComIEnReg,irqEn|0x80);
    ClearBitMask(ComIrqReg,0x80);	//清所有中断位
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);	 	//清FIFO缓存
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pIn [i]);    }
    WriteRawRC(CommandReg, Command);	  
//   	 n = ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }	 //开始传送
    										 
      i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
// 	  i = 100000;
    do 
    {
        n = ReadRawRC(ComIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x01) && !(n&waitFor));
    ClearBitMask(BitFramingReg,0x80);

    if (i!=0)
    {    
        if(!(ReadRawRC(ErrorReg)&0x1B))
        {
            status = MI_OK;
            if (n & irqEn & 0x01)
            {   status = MI_NOTAGERR;   }
            if (Command == PCD_TRANSCEIVE)
            {
               	n = ReadRawRC(FIFOLevelReg);
              	lastBits = ReadRawRC(ControlReg) & 0x07;
                if (lastBits)
                {   *pOutLenBit = (n-1)*8 + lastBits;   }
                else
                {   *pOutLenBit = n*8;   }
                if (n == 0)
                {   n = 1;    }
                if (n > MAXRLEN)
                {   n = MAXRLEN;   }
                for (i=0; i<n; i++)
                {   pOut [i] = ReadRawRC(FIFODataReg);    }
            }
        }
        else
        {   status = MI_ERR;   }
        
    }
   

    SetBitMask(ControlReg,0x80);           // stop timer now
    WriteRawRC(CommandReg,PCD_IDLE); 
    return status;
}






/*******************************************************************
* 函数名：     PcdRequest()
* 功能描述 ：   	寻卡
* 作者：         谢国卿 
*参数说明: req_code[IN]:寻卡方式
                0x52 = 寻感应区内所有符合14443A标准的卡
                0x26 = 寻未进入休眠状态的卡
           pTagType[OUT]：卡片类型代码
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
* 返回值说明：成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/               
char PcdRequest(u8 req_code,u8 *pTagType)
{
	char   status;  
	u8   unLen;
	u8   ucComMF522Buf[MAXRLEN]; 

	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
 
	ucComMF522Buf[0] = req_code;

	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   status = MI_ERR;   }
   
	return status;
}


/*******************************************************************
* 函数名：     PcdAnticoll()
* 功能描述 ：   防冲撞
* 作者：         谢国卿 
* 参数说明：     pSnr[OUT]:卡片序列号，4字节
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdAnticoll(u8 *pSnr)
{
    char   status;
    u8   i,snr_check=0;
    u8   unLen;
    u8   ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/*******************************************************************
* 函数名：     CalulateCRC()
* 功能描述 ：  用MF522计算CRC16函数
* 作者：         谢国卿 
*参数说明: 
* 返回值说明： 
* 修改记录：
* 其他：
*******************************************************************/
void CalulateCRC(u8 *pIn ,u8   len,u8 *pOut )
{
    u8   i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIn +i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOut [0] = ReadRawRC(CRCResultRegL);
    pOut [1] = ReadRawRC(CRCResultRegM);
}


/*******************************************************************
* 函数名：     PcdSelect()
* 功能描述 ：   选定卡片
* 作者：         谢国卿 
* 参数说明：     pSnr[IN]:卡片序列号，4字节
* 返回值说明：   成功返回MI_OK
* 修改记录：
* 其他：
*******************************************************************/
char PcdSelect(u8 *pSnr)
{
    char   status;
    u8   i;
    u8   unLen;
    u8   ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}


void print_info(unsigned char *p,int cnt)
{
  int i;
  for(i=0;i<cnt;i++)
  {
		if(!i)
			printf("0x%02X ",p[i]);
		else
			printf("%02X ",p[i]);
  }
  printf("\r\n");
}




