#include "main.h"

int main(void)
{
	u8 errorCode;
	u8 buff[20]={0};
	u8 test[50]="123456";
	u8 test2[50];
	TIMEDATE alarmA={.date = 0xff,.weekday = 0xff,.hour=8,.minute=0xff,.second=0xff};
//	LED_Init();
//	breath_led();	
	BS8116_I2C_Pin_Init();
	Key_Init();
	Usart_Init();
	NVIC_SetPriorityGrouping(7-2);
	Tim3_Init();
	RFID_Init();
	TIM5_Init_DS_MS(500);
	W25q64_Init();	
	AT24C04_Init();
//	Read_W25q64ID();
//	Sector_Erase( 0x000FFF06);
//	Page_Program(0x000FFF06,test,sizeof(test));
//	W25Q64_ReadData(0x000FFF06,test2,sizeof(test));
//	printf("%s\r\n",test2);
		
	
	Voice_Init();//����ģ���ʼ��
	TIM5_Init_DS_MS(500);
	Voice_Output(0x26);
	TIM5_Init_DS_MS(500);

	st7789vm_init();
//	Display_Picture(0,0,(u8*)gImage_zjj); //��ʾͼƬ
//	DMA_Transfer_Init();
//	Dma_FontUpdate();//DMA�����ֿ�
 
//	Display_String(0,170,WHITE,BLACK,24,(u8*)"One piece֣����!�����̻�!!!!!!aСʯͷ�����迴����");
	
	ADC1_Init();
	DMA_ADC_Transfer_Init();
	Rtc_Init();
	Rtc_Wake_Up_Init();
	RTC_Alarm_A_Init(alarmA);
	MG200_Init();
	Usart2_Init();
	ESP32_TCP_Init();
	ESP32_Wifi_connect((u8*)"FUCKNET_3",(u8*)"666000777");
		st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
		Display_String(30,120,GREEN,BLACK,16,(u8 *)"Wifi������...");
	Esp32_Severconnect((u8*)"gml2gCvFGUG.iot-as-mqtt.cn-shanghai.aliyuncs.com");
	errorCode = ESP32_SendAndReceive((u8*)"AT+CIPSEND\r\n",(u8*)"OK");
	if(!errorCode)
	{
		_mqtt.Init(rxbuf,0,txbuf,0);
		st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
		Display_String(30,120,GREEN,BLACK,16,(u8 *)"Wifi���ӳɹ�...");
		//MQTT����
		//static uint8_t Connect(char *ClientID,char *Username,char *Password)
		errorCode  = _mqtt.Connect("ESP32|securemode=3,signmethod=hmacsha1|",
															 "XXXLOCK&gml2gCvFGUG",
															 "8EBD8BE58AB12236FC5A5FA0F82B702138D07A9D");
		if(errorCode==1)
		{
			printf("MQTT����ɹ�\r\n");
			errorCode=_mqtt.SubscribeTopic("/sys/gml2gCvFGUG/XXXLOCK/thing/service/property/set",0,1);//����
			if(errorCode==1)
			{
				printf("MQTT�豸�������ö��ĳɹ�\r\n");
			}
			errorCode=_mqtt.SubscribeTopic("/sys/gml2gCvFGUG/XXXLOCK/thing/event/property/post_reply",0,1);//����
			if(errorCode==1)
			{
				printf("MQTT�ƶ���Ӧ�����ϱ����ĳɹ�\r\n");
			}
			u8 message[255];
			sprintf((char*)message,"{\"method\":\"thing.service.property.set\",\"id\":\"99515625\",\"params\":{\"Fingeprint_Num\":\"%d\",\"LockStatus\":\"%s\",\"LockPwd\":\"%s\",\"Adm_Pwd\":\"%s\"},\"version\":\"1.0.0\"}",LOCK_TH.finger_num,LOCK_TH.lock_status,LOCK_TH.lock_pwd,LOCK_TH.adm_pwd);
			errorCode=_mqtt.PublishData("/sys/gml2gCvFGUG/${deviceName}/thing/event/property/post",(char *)message,1);//����
			if(errorCode==1)
			{
				printf("MQTT���з����ɹ�\r\n");
			}
		}
	}


	char *buf;
	char LockPwd_buffer[15]={0};
	errorCode = 0;
	
	
	
//  ��ʼ������	
//	strcpy((char *)LOCK_TH.lock_status,"close");
//	strcpy((char *)LOCK_TH.adm_pwd,"222222");
//	strcpy((char *)LOCK_TH.lock_pwd,"123456");
//	strcpy((char *)LOCK_TH.rfid_card_id,"CA");
//	LOCK_TH.finger_id=15;
//	LOCK_TH.finger_num = 17;
//	Write_Data_ToW25q64();
	
	Read_Data_FromW25q64();
	st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
	while(1)
	{	
		
		if(ESP32_REC.flag)
		{
			u8 i;
			ESP32_REC.flag = 0;
			buf = strstr((char *)ESP32_REC.buf,"LockPwd");
			
			for(i=10;i<strlen(buf);i++)
			{
				if(*(buf+i) =='"')
				{
					break;
				}
				LockPwd_buffer[i-10] = *(buf+i);
			}
			printf("\r\nLockPwd_buffer:%s\r\n",LockPwd_buffer);
			
		}
		if(rtc_flag==1)
		{
			rtc_flag= 0;
			Rtc_Read_timeAnddate(); //��ȡRTCʱ��
			sprintf((char*)buff,"�¶�:%.1f",adc_tempture);
			Display_String(168,225,RED,BLACK,16,buff);
		}
		
		if(MG200_DETECT)//ָ�ƿ���
		{
			ReqCaptureAndExtract(0);
//			Registered_fingerprints();
//			TIM5_Init_DS_MS(1000);
			if(Matchln()!=0)
			{
				TIM5_Init_DS_MS(500);
				PWM_Open_door();//����
				TIM5_Init_DS_MS(500);
				Voice_Output(0x18); //�������ųɹ�����ӭ�ؼҡ�
				while(VOICE_BUSY);
				PWM_Close_door();
				TIM5_Init_DS_MS(500);
				printf("\r\n");
			}
			TIM5_Init_DS_MS(500);
			LOCK_TH.finger_num = GetUserNum();
			printf("%d\r\n",LOCK_TH.finger_num);
			Write_Data_ToW25q64();
		}
		
		
		
		//�жϰ������벢����
//		if(!BS8116_IRQ) //���Ѽ���
//		{
			judge_psw();
//		}
//		//��ť����
//		Key_Open_Door();    
		//�ж�rfid����
		judge_rfid();
//		if(Key_Scan())
//		{
//			Font_Update();
//		}
	}
	
}
