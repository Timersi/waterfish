#ifndef __ESP32_H
#define __ESP32_H
#include "stm32f4xx.h"
#include "tim.h"
#include <stdio.h>
#include <string.h>

typedef struct {
	u8 buf[1024];
	u16 len;
	u8 flag;     //全部传输完成的标志位
}USART2_REC;

extern USART2_REC ESP32_REC;
void Usart2_Init(void);
void Esp32_SendByte(u8 *data);
u8 ESP32_SendAndReceive(u8 *cmd,u8 *respond);
void ESP32_TCP_Init(void);
void ESP32_Wifi_connect(u8 *ssid,u8 *pwd);
void Esp32_Severconnect(u8 *remote_ip);
#endif 
