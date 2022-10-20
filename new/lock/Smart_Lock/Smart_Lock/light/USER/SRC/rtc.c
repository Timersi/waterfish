#include "rtc.h"

char *time = __TIME__;
char *date = __DATE__;
u8 month[12][5] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
u8 month_day[13] ={0,31,28,31,30,31,30,31,31,30,31,30,31};

TIMEDATE mytctime;

/*
函数功能:解析时间和日期string
函数形参:TIMEDATE mytctime
函数返回值:void
函数说明:

*/
void Rtc_Analys_DateAndTime(TIMEDATE *mytctime)
{
	mytctime->hour = (time[0] - '0')*10 +(time[1]-'0');
	mytctime->minute = (time[3] - '0')*10 +(time[4]-'0');
	mytctime->second = (time[6] - '0')*10 +(time[7]-'0');
	u8 i;
	u8 str[4]={0};
	for(i=0;i<3;i++)
	{
		str[i] = date[i];
	}
	for(i=0;i<12;i++)
	{
		if(strcmp((const char*)str,(const char*)month[i])==0)
		{
			mytctime->month = i+1;
			break;
		}
	}
	if(date[4] ==' ')
	{
		mytctime->date = (date[5]-'0');
	}
	else
	{
		mytctime->date = (date[4] - '0')*10 +(date[5]-'0');
	}

	mytctime->year = (date[9] - '0')*10 +(date[10]-'0');
}

/*
函数功能:判断闰年
函数形参:TIMEDATE mytctime
函数返回值:u8
函数说明:
*/
u8 Judge_RunYear(u16 year)
{
	if(((year%4) ==0 && (year%100)!=0 )|| (year%400) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
函数功能:解析星期几
函数形参:TIMEDATE mytctime
函数返回值:void
函数说明:

*/
void Rtc_Analys_Weekday(TIMEDATE *mytctime)
{
	u16 sum=0;
	u16 i;
	for(i=1990;i<(2000+mytctime->year);i++)
	{
		sum+=365+Judge_RunYear(i);
	}
	if(Judge_RunYear(mytctime->year))
	{
		month_day[2] =29;
	}
	for(i=0;i<mytctime->month;i++)
	{
		sum+=month_day[i];
	}
	sum+=mytctime->date;
	
	switch(sum%7)
	{
		case 0: mytctime->weekday = 7; break;
		case 6: mytctime->weekday = 6; break;
		case 5: mytctime->weekday = 5; break;
		case 4: mytctime->weekday = 4; break;
		case 3: mytctime->weekday = 3; break;
		case 2: mytctime->weekday = 2; break;
		case 1: mytctime->weekday = 1; break;
	}
}




/*
函数功能:设置时间
函数形参:TIMEDATE mytctime
函数返回值:void
函数说明:

*/
void Rtc_Set_Time(TIMEDATE *mytctime)
{
	RTC_TimeTypeDef  RTC_TimeStruct={0}; 
	RTC_TimeStruct.RTC_Hours= mytctime->hour;
	RTC_TimeStruct.RTC_Minutes = mytctime->minute;
	RTC_TimeStruct.RTC_Seconds = mytctime->second;
	RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct);
}

/*
函数功能:设置日期
函数形参:TIMEDATE mytctime
函数返回值:void
函数说明:

*/
void Rtc_Set_Date(TIMEDATE *mytctime)
{
	RTC_DateTypeDef  RTC_DateStruct={0};
	RTC_DateStruct.RTC_Date = mytctime->date;
	RTC_DateStruct.RTC_Month = mytctime->month;
	RTC_DateStruct.RTC_WeekDay = mytctime->weekday;
	RTC_DateStruct.RTC_Year = mytctime->year;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct );
}

/*
函数功能:RTC初始化
函数形参:void
函数返回值:void
函数说明:

*/
u8 Rtc_Init(void)
{
	u8 timeout;
	u8 status;
	//打开PWR的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	
	//使能备份域的写保护
	PWR_BackupAccessCmd(ENABLE);
	
	//解除写保护
	 RTC_WriteProtectionCmd(DISABLE);
	//选择时钟源
	RCC_LSEConfig(RCC_LSE_ON);
	do
	{
		status =  RTC_GetFlagStatus(RCC_FLAG_LSERDY);
		timeout++;
		TIM5_Init_DS_MS(1);
	}while(status==0 && timeout <= 10);
	if(status == 1)
	{
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		RCC_RTCCLKCmd(ENABLE);
		
		RTC_InitTypeDef   RTC_InitStruct={0};
		RTC_InitStruct.RTC_AsynchPrediv = 127;
		RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
		RTC_InitStruct.RTC_SynchPrediv = 255;
		RTC_Init(&RTC_InitStruct );  
		
		Rtc_Analys_DateAndTime(&mytctime);
		Rtc_Analys_Weekday(&mytctime);
		//设置日期
		Rtc_Set_Date(&mytctime);
		//设置时间
		Rtc_Set_Time(&mytctime);
		

	}
	else
	{
		printf("打开LSE时钟源失败\r\n"); 
		return 1;
	}
	
	return 0;
}



/*
函数功能:RTC读取时间日期,并在LCD中显示
函数形参:void
函数返回值:void
函数说明:

*/
void Rtc_Read_timeAnddate(void)
{
	u8 timebuf[100]={0};
	u8 datebuf[100]={0};
	RTC_DateTypeDef  RTC_DateStruct;
	RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
	RTC_TimeTypeDef  RTC_TimeStruct;
	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	sprintf((char *)timebuf,"20%02d-%02d-%02d-%02d",RTC_DateStruct.RTC_Year,RTC_DateStruct.RTC_Month,RTC_DateStruct.RTC_Date,RTC_DateStruct.RTC_WeekDay);
	sprintf((char *)datebuf,"%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds);
	
	Display_String(135,0,WHITE,BLACK,16,timebuf);
	Display_String(0,0,WHITE,BLACK,16,datebuf);
}


/*
函数功能:RTC唤醒定时器的初始化
函数形参:void
函数返回值:void
函数说明:

*/
void  Rtc_Wake_Up_Init(void)
{
	//关闭唤醒定时器
	RTC_WakeUpCmd(DISABLE);
	
	//等待可以更新
	while(RTC_GetFlagStatus(RTC_FLAG_WUTWF)==0);
	
	//设置重装载值，记多少次
	RTC_SetWakeUpCounter(0);
	
	//设置为1HZ的计数周期，1Hz的周期是1秒
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//配置中断
	RTC_ITConfig(RTC_IT_WUT,ENABLE);
	//配置外部中断
	EXTI_InitTypeDef  EXTI_InitStruct={0};
	EXTI_InitStruct.EXTI_Line = EXTI_Line22;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(& EXTI_InitStruct); 
	//设置优先级
	NVIC_SetPriority(RTC_WKUP_IRQn,NVIC_EncodePriority(7-2,0,1));
	//	使能核心响应中断
	NVIC_EnableIRQ(RTC_WKUP_IRQn);
	
	//先清零标志位
	RTC_ClearITPendingBit(RTC_IT_WUT);
	EXTI_ClearITPendingBit(EXTI_Line22);
	
	//使能唤醒定时器
	RTC_WakeUpCmd(ENABLE);
}

/*
函数功能:RTC唤醒中断服务函数
函数形参:void
函数返回值:void
函数说明:

*/
u8 rtc_flag = 0;
void RTC_WKUP_IRQHandler(void)
{
	RTC_ClearITPendingBit(RTC_IT_WUT);
	EXTI_ClearITPendingBit(EXTI_Line22);
	rtc_flag = 1;
}


/*
函数功能:RTC闹钟A的初始化
函数形参:TIMEDATE alarmA
函数返回值:void
函数说明:如果想要某个数值无关则填入0xff
*/
void RTC_Alarm_A_Init(TIMEDATE alarmA)
{
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
	
	//等待闹钟A可写入
	while(RTC_GetFlagStatus(RTC_FLAG_ALRAWF)==0);
	
	RTC_AlarmTypeDef  RTC_AlarmStruct;
	if(alarmA.date ==0xff && alarmA.weekday==0xff)
	{
		RTC_AlarmStruct.RTC_AlarmMask |= RTC_AlarmMask_DateWeekDay; 
	}
	else
	{
		if(alarmA.date ==0xff)
		{
			RTC_AlarmStruct.RTC_AlarmDateWeekDaySel =  RTC_AlarmDateWeekDaySel_WeekDay ;
			RTC_AlarmStruct.RTC_AlarmDateWeekDay = alarmA.weekday;
		}
		else if(alarmA.weekday==0xff)
		{
			RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
			RTC_AlarmStruct.RTC_AlarmDateWeekDay = alarmA.date;
		}
	}
	
	RTC_TimeTypeDef  RTC_AlarmTime={0};
	RTC_AlarmTime.RTC_Hours = alarmA.hour;
	RTC_AlarmTime.RTC_Minutes = alarmA.minute;
	RTC_AlarmTime.RTC_Seconds = alarmA.second;
	RTC_AlarmStruct.RTC_AlarmTime = RTC_AlarmTime;   //初始化RTC_AlarmTime的时间用于比较
	
	if(alarmA.hour == 0xff)
	{
		RTC_AlarmStruct.RTC_AlarmMask |= RTC_AlarmMask_Hours;
	}
	if(alarmA.minute == 0xff)
	{
		RTC_AlarmStruct.RTC_AlarmMask |= RTC_AlarmMask_Minutes;
	}
	if(alarmA.second == 0xff)
	{
		RTC_AlarmStruct.RTC_AlarmMask |= RTC_AlarmMask_Seconds;
	}
	
	RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmStruct);
	
	RTC_ITConfig(RTC_IT_ALRA,ENABLE);
	//配置外部中断
	EXTI_InitTypeDef  EXTI_InitStruct={0};
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(& EXTI_InitStruct); 
	//设置优先级
	NVIC_SetPriority(RTC_Alarm_IRQn,NVIC_EncodePriority(7-2,0,1));
	//	使能核心响应中断
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
	
	//先清零标志位
	RTC_ClearITPendingBit(RTC_IT_ALRA);
	EXTI_ClearITPendingBit(EXTI_Line17);
	
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
}


/*
函数功能:RTC闹钟中断服务函数
函数形参:void
函数返回值:void
函数说明:

*/
void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetITStatus(RTC_IT_ALRA))
	{
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
		printf("闹钟中断\r\n");
	}
}

