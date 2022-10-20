#ifndef __RTC_H
#define __RTC_H
#include "stm32f4xx.h"
#include "tim.h"
#include <stdio.h>
#include <string.h>
#include "lcd.h"

typedef struct{
	u8 year;
	u8 month;
	u8 date;
	u8 weekday;
	u8 hour;
	u8 minute;
	u8 second;
}TIMEDATE;

extern char *time;
extern char *date;
extern u8 rtc_flag;

u8 Rtc_Init(void);
void Rtc_Read_timeAnddate(void);
void  Rtc_Wake_Up_Init(void);
void RTC_Alarm_A_Init(TIMEDATE alarmA);
#endif 
