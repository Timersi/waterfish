#include "rfid.h"


/*
��������:RFID��ʼ��
�����β�:void
��������ֵ:void
����˵��:PA15: RFID_SPI3_CS   Ƭѡ��
					PC10: SPI3_SCK       ʱ����
					PC11:MISO      			 ����ӳ�
					PC12: SPI3_MOSI      �������� 
					PD2:RFID_SPI3_RST    ��λ

*/
void RFID_SPI_GPIO_Init(void)
{
	//GPIO��ʼ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOD, ENABLE);//1.��ʱ��
	
	//PC10--SCL--�������,PC12--MOSI--�������
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//PA15--CS--�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//PD2--RST--�������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//PC11--MISO--��������
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
//	GPIO_InitTypeDef GPIO_InitStruct={0};

//	//����GPIO
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





//��RC632�Ĵ���
//Address:�Ĵ�����ַ
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


//дRC632�Ĵ���
//Address[IN]:�Ĵ�����ַ
//value[IN]:д���ֵ
void WriteRawRC(uint8_t Address, uint8_t value)
{  
    uint8_t ucAddr;

	CS_L;
    ucAddr = ((Address<<1)&0x7E);//0111 1110

	SPI3_Sent_byte(ucAddr);
	SPI3_Sent_byte(value);
	CS_H;
}

//��Ĵ���λ
void ClearBitMask(u8 reg,u8 mask) 
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 

//��RC522�Ĵ���λ
//reg[IN]:�Ĵ�����ַ
//mask[IN]:��λֵ
void SetBitMask(u8 reg,u8 mask)  
{
    char   tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}


//��λRC522
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
    
	WriteRawRC(ModeReg,0x3D);            //��Mifare��ͨѶ��CRC��ʼֵ0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);
	WriteRawRC(TPrescalerReg,0x3E);
	
	WriteRawRC(TxAutoReg,0x40);//����Ҫ
   
   return MI_OK;
	
}

//�ر�����
void PcdAntennaOff(void)
{
	ClearBitMask(TxControlReg, 0x03);
}

//��������
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
	PcdReset();       //��    �ܣ���λRC522
	PcdAntennaOff();  //�ر�����
	PcdAntennaOn();   //��������
}    

//����RC522����ģʽ
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
* ��������     PcdComMF522()
* �������� ��  ͨ��RC522��ISO14443��ͨѶ
* ���ߣ�         л���� 
* ����˵��:   Command[IN]:RC522������
							pIn [IN]:ͨ��RC522���͵���Ƭ������
							InLenByte[IN]:�������ݵ��ֽڳ���
							pOut [OUT]:���յ��Ŀ�Ƭ��������
							*pOutLenBit[OUT]:�������ݵ�λ����
* ����ֵ˵����
* �޸ļ�¼��
* ������
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
    ClearBitMask(ComIrqReg,0x80);	//�������ж�λ
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);	 	//��FIFO����
    
    for (i=0; i<InLenByte; i++)
    {   WriteRawRC(FIFODataReg, pIn [i]);    }
    WriteRawRC(CommandReg, Command);	  
//   	 n = ReadRawRC(CommandReg);
    
    if (Command == PCD_TRANSCEIVE)
    {    SetBitMask(BitFramingReg,0x80);  }	 //��ʼ����
    										 
      i = 600;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
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
* ��������     PcdRequest()
* �������� ��   	Ѱ��
* ���ߣ�         л���� 
*����˵��: req_code[IN]:Ѱ����ʽ
                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
                0x26 = Ѱδ��������״̬�Ŀ�
           pTagType[OUT]����Ƭ���ʹ���
                0x4400 = Mifare_UltraLight
                0x0400 = Mifare_One(S50)
                0x0200 = Mifare_One(S70)
                0x0800 = Mifare_Pro(X)
                0x4403 = Mifare_DESFire
* ����ֵ˵�����ɹ�����MI_OK
* �޸ļ�¼��
* ������
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
* ��������     PcdAnticoll()
* �������� ��   ����ײ
* ���ߣ�         л���� 
* ����˵����     pSnr[OUT]:��Ƭ���кţ�4�ֽ�
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
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
* ��������     CalulateCRC()
* �������� ��  ��MF522����CRC16����
* ���ߣ�         л���� 
*����˵��: 
* ����ֵ˵���� 
* �޸ļ�¼��
* ������
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
* ��������     PcdSelect()
* �������� ��   ѡ����Ƭ
* ���ߣ�         л���� 
* ����˵����     pSnr[IN]:��Ƭ���кţ�4�ֽ�
* ����ֵ˵����   �ɹ�����MI_OK
* �޸ļ�¼��
* ������
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




