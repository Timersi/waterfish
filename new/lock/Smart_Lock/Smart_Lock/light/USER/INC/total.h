#ifndef __TOTAL_H
#define __TOTAL_H
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "tim.h"
#include "tim3.h"
#include "rfid.h"
#include "at24c04.h"
#include "w25q64.h"
#include "voice.h"
#include "bs8116.h"
#include "lcd.h"

typedef struct{
	u8 lock_status[10];
	u8 lock_pwd[10];
	u8 adm_pwd[10];
	u8 rfid_card_id[10];
	u8 finger_id;
	u8 finger_num;
}LOCK_THING;

extern LOCK_THING LOCK_TH;

void judge_rfid(void);
void Write_Adm_Psw_ToAT24C04(void);
void Write_Psw_ToAT24C04(void);
void judge_psw(void);
void Key_Open_Door(void);
void judge_usart_recieve(void);
void Get_Adm_Psw_ToAT24C04(void);
void Get_Psw_ToAT24C04(void);
void Write_Data_ToW25q64(void);
void Read_Data_FromW25q64(void);
#endif 
