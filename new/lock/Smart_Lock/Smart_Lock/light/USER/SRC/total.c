#include "total.h"

LOCK_THING LOCK_TH;

void Write_Psw_ToW25q64(const char *psw);
void Write_Adm_Psw_ToW25q64(const char *psw);
void Change_Adm_Password(void);
void Change_Password(void);
void Write_Adm_Psw_ToAT24C04(void);
void Write_Psw_ToAT24C04(void);
void Write_Data_ToW25q64(void);
void Read_Data_FromW25q64(void);

void judge_rfid(void)
{
	u8 card_id[4];  				//卡号
	unsigned char CT[2];//卡类型
	if(PcdRequest(PICC_REQALL,CT)==MI_OK)//寻卡成功  先寻卡
		{
		   
		  if(PcdAnticoll(card_id)==MI_OK)/*防冲撞成功 读取卡的数据*/
		  {
					printf("卡类型:");
					print_info(CT,2);//打印类型
					printf("卡号:");
					print_info(card_id,4);//打印卡号
					strcpy((char *)LOCK_TH.rfid_card_id,(char *)card_id);
					
					if(PcdSelect(card_id)==MI_OK)//选卡成功  对比是否为注册卡号
					{
						printf("卡片获取成功\r\n");	
						
					}
					if(card_id[0] == 0xCA)
					{
						TIM5_Init_DS_MS(1000);
						PWM_Open_door();//开门
						TIM5_Init_DS_MS(1000);
						Voice_Output(0x18); //播报开门成功《欢迎回家》
						while(VOICE_BUSY);
						PWM_Close_door();
						TIM5_Init_DS_MS(1000);
					}
					else
					{
						Voice_Output(0x19);  //开门失败
						while(VOICE_BUSY);
					}
			
		  }
		}
	
}



void Administrator_menu(void)
{
	static u8 cnt=0;
	u8 choose[3]={0};
	u8 buf[1]={0};
	u8 combine[15];
	st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
	Display_String(20,30,GREEN,BLACK,16,(u8 *)"管理员选项如下");
	Display_String(40,50,GREEN,BLACK,16,(u8 *)"1.修改管理员密码");
	Display_String(40,70,GREEN,BLACK,16,(u8 *)"2.修改门锁密码");
	Display_String(40,90,GREEN,BLACK,16,(u8 *)"3.查询指纹数");
	Display_String(40,110,GREEN,BLACK,16,(u8 *)"4.查询门锁密码");
	Display_String(10,130,GREEN,BLACK,16,(u8 *)"请输入需要的操作序号：");
	printf("\r\n\r\n管理员选项如下:\r\n");
	printf("1.修改管理员密码\r\n");
	printf("2.修改门锁密码\r\n");
	printf("3.查询指纹数\r\n");
	printf("4.查询门锁密码\r\n");
	printf("请输入需要的操作序号：\r\n");
	while(1)
	{
		if(!BS8116_IRQ)
		{
			buf[0] = Bs8116_ReadKey();
			while(!BS8116_IRQ);
			if(buf[0] !='#'&& (buf[0]=='1' ||buf[0]=='2'||buf[0]=='3'||buf[0]=='4'||buf[0]=='5'||buf[0]=='6'||buf[0]=='7'||buf[0]=='8'||buf[0]=='9'||buf[0]=='0'||buf[0]=='*'))
			{
				printf("%c",buf[0]);
				sprintf((char*)combine,"%c",buf[0]);
				Display_String(190,130,GREEN,BLACK,16,combine);
				choose[cnt] = buf[0];
				cnt++;
			}
			else if(buf[0] =='#')
			{
				cnt=0;
				printf("\r\n");
				break;
			}
		}
	}
	switch(choose[0])
	{
		case '1': Change_Adm_Password();break;
		case '2': Change_Password();break;	
		case '3': printf("指纹数：%d个\r\n",LOCK_TH.finger_num);
							st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
							sprintf((char*)combine,"指纹数有：%d个",LOCK_TH.finger_num);
							Display_String(30,120,GREEN,BLACK,16,combine);
							Display_String(30,145,GREEN,BLACK,16,(u8*)"3秒后返回主界面");
							TIM5_Init_DS_MS(3000);
							st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
							break;	
		case '4': printf("门锁密码:%s\r\n",LOCK_TH.lock_pwd); 
							st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
							sprintf((char*)combine,"门锁密码:%s",LOCK_TH.lock_pwd);
							Display_String(30,120,GREEN,BLACK,16,combine);
							Display_String(30,145,GREEN,BLACK,16,(u8*)"3秒后返回主界面");
							TIM5_Init_DS_MS(3000);
							st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
							break;	
		default:  printf("输入序号错误，已经退出界面\r\n");break;								 
	}
	memset(choose, '\0', sizeof(choose)); 
}

/*
	功能：修改管理员密码
	形参：void
	说明： 修改管理员密码
	

*/
void Change_Adm_Password(void)
{
	Voice_Output(0x01); //播报设置管理员密码
	while(VOICE_BUSY);
	Voice_Output(0x0d); //播报输入新密码
	while(VOICE_BUSY);
	printf("\r\n");
	u8 psw[7];
	memset(psw, '\0', sizeof(psw)); 
	u8 buf[2]={0};
	static u8 cnt=0;
	printf("\r\n请输入新的管理员密码:\r\n");
	st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
	Display_String(0,120,GREEN,BLACK,16,(u8*)"请输入新的管理员密码");
	while(1)
	{
		if(!BS8116_IRQ)
		{
			buf[0] = Bs8116_ReadKey();
			while(!BS8116_IRQ);
			if(buf[0] !='#'&& (buf[0]=='1' ||buf[0]=='2'||buf[0]=='3'||buf[0]=='4'||buf[0]=='5'||buf[0]=='6'||buf[0]=='7'||buf[0]=='8'||buf[0]=='9'||buf[0]=='0'||buf[0]=='*'))
			{
					printf("%c",buf[0]);
					switch(cnt)
					{
						case 0:	
										Display_String(80,140,GREEN,BLACK,16,(u8 *)"*"); break;
						case 1:	
										Display_String(100,140,GREEN,BLACK,16,(u8 *)"*"); break;
						case 2:	
										Display_String(120,140,GREEN,BLACK,16,(u8 *)"*"); break;
						case 3:	
										Display_String(140,140,GREEN,BLACK,16,(u8 *)"*"); break;
						case 4:	
										Display_String(160,140,GREEN,BLACK,16,(u8 *)"*"); break;
						case 5:	
										Display_String(180,140,GREEN,BLACK,16,(u8 *)"*"); break;
						case 6:	
										Display_String(200,140,GREEN,BLACK,16,(u8 *)"*"); break;
					}
					psw[cnt] = buf[0];
					cnt++;
			}
			else if(buf[0] =='#')
			{
				st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
				cnt=0;
				strcpy((char *)LOCK_TH.adm_pwd,(char *)psw);
				Write_Data_ToW25q64();
				printf("\r\n修改管理员密码成功！\r\n");
				Display_String(40,120,GREEN,BLACK,16,(u8 *)"修改管理员密码成功！"); 
				Display_String(30,145,GREEN,BLACK,16,(u8*)"3秒后返回主界面");
				Voice_Output(0x11); //播报操作成功
				while(VOICE_BUSY);
				memset(psw, '\0', sizeof(psw));
				TIM5_Init_DS_MS(3000);
				st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
				break;
			}
		}
	}
}

/*
	功能：修改门锁密码成功
	形参：void
	说明： 修改门锁密码成功
	

*/
void Change_Password(void)
{
	Voice_Output(0x02); //播报设置门锁密码
	while(VOICE_BUSY);
	Voice_Output(0x0d); //播报输入新密码
	while(VOICE_BUSY);
	printf("\r\n");
	u8 psw[7];
	memset(psw, '\0', sizeof(psw)); 
	u8 buf[2]={0};
	static u8 cnt=0;
	printf("\r\n请输入新的门锁密码:\r\n");
	while(1)
	{
		if(!BS8116_IRQ)
		{
			buf[0] = Bs8116_ReadKey();
			while(!BS8116_IRQ);
			if(buf[0] !='#'&& (buf[0]=='1' ||buf[0]=='2'||buf[0]=='3'||buf[0]=='4'||buf[0]=='5'||buf[0]=='6'||buf[0]=='7'||buf[0]=='8'||buf[0]=='9'||buf[0]=='0'||buf[0]=='*'))
			{
					printf("%c",buf[0]);
					psw[cnt] = buf[0];
					cnt++;
			}
			else if(buf[0] =='#')
			{
				cnt=0;
				strcpy((char *)LOCK_TH.lock_pwd,(char *)psw);
				Write_Data_ToW25q64();
				printf("\r\n修改门锁密码成功！\r\n");
				Voice_Output(0x11); //播报操作成功
				while(VOICE_BUSY);
				memset(psw, '\0', sizeof(psw));
				break;
			}
		}
	}
}




void Key_Open_Door(void)
{
	if(Key_Scan())
		{
			PWM_Open_door();
			TIM5_Init_DS_MS(1000);
			PWM_Close_door();
			TIM5_Init_DS_MS(1000);
		}
}


/*******************************************
	功能：获取密码AT24C04，存放psw
	形参： u8 *psw  密码存放的位置
	说明： 16位存储一个密码 前16位存储管理员密码
	
*******************************************/
void Get_Psw_ToAT24C04(void)
{
		AT24C04_Continue_Read(LOCK_TH.lock_pwd,7,16);
		printf("\r\n门锁密码:%s\r\n",LOCK_TH.lock_pwd);
}

/*******************************************
	功能：获取管理员密码AT24C04，存放psw
	形参： u8 *psw  密码存放的位置
	说明： 16位存储一个密码 前16位存储管理员密码
	
*******************************************/
void Get_Adm_Psw_ToAT24C04(void)
{
	AT24C04_Continue_Read(LOCK_TH.adm_pwd,7,0);
	printf("\r\n管理员密码:%s\r\n",LOCK_TH.adm_pwd);
}



void Write_Psw_ToAT24C04(void)
{
	AT24C04_Auto_Page_Wtite((u8 *)LOCK_TH.lock_pwd,7,16);
}


/*
	功能：写入管理员密码存入w25q64
	形参： u8 *psw
	说明： 16位存储一个密码 前16位存储管理员密码
	
*/
void Write_Adm_Psw_ToAT24C04(void)
{
	AT24C04_Auto_Page_Wtite((u8 *)LOCK_TH.adm_pwd,7,0);
}


/*
	功能：写入数据存入w25q64
	形参： u8 *psw
	说明： 16位存储一个密码 前16位存储管理员密码
	
*/
void Write_Data_ToW25q64(void)
{
	Read_W25q64ID();
	Sector_Erase( 0x000FFF06);
	W25q64_Auto_Page_Wtite((u8 *)LOCK_TH.lock_status,6,0x000FFF06);
	W25q64_Auto_Page_Wtite((u8 *)LOCK_TH.lock_pwd,7,0x000FFF16);
	W25q64_Auto_Page_Wtite((u8 *)LOCK_TH.adm_pwd,7,0x000FFF26);
	W25q64_Auto_Page_Wtite((u8 *)LOCK_TH.rfid_card_id,2,0x000FFF36);
	W25q64_Auto_Page_Wtite(&LOCK_TH.finger_id,1,0x000FFF46);
	W25q64_Auto_Page_Wtite(&LOCK_TH.finger_num,1,0x000FFF56);
}

/*
	功能：从w25q64读取数据
	形参： u8 *psw
	说明： 16位存储一个密码 前16位存储管理员密码
	
*/
void Read_Data_FromW25q64(void)
{
	u8 lock_status[10];
	u8 lock_pwd[10];
	u8 adm_pwd[10];
	u8 rfid_card_id[10];
	u8 finger_id;
	u8 finger_num;
	W25Q64_ReadData(0x000FFF06,lock_status,6);
	W25Q64_ReadData(0x000FFF16,lock_pwd,7);
	W25Q64_ReadData(0x000FFF26,adm_pwd,7);
	W25Q64_ReadData(0x000FFF36,rfid_card_id,2);
	W25Q64_ReadData(0x000FFF46,&finger_id,1);
	W25Q64_ReadData(0x000FFF56,&finger_num,1);
	strcpy((char *)LOCK_TH.lock_status,(char *)lock_status);
	strcpy((char *)LOCK_TH.adm_pwd,(char *)adm_pwd);
	strcpy((char *)LOCK_TH.lock_pwd,(char *)lock_pwd);
	strcpy((char *)LOCK_TH.rfid_card_id,(char *)rfid_card_id);
	LOCK_TH.finger_id = finger_id;
	LOCK_TH.finger_num = finger_num;
	printf("lock_status:%s\r\n",lock_status);
	printf("lock_pwd:%s\r\n",lock_pwd);
	printf("adm_pwd:%s\r\n",adm_pwd);
	printf("rfid_card_id:%s\r\n",rfid_card_id);
	printf("finger_id:%d\r\n",finger_id);
	printf("finger_num:%d\r\n",finger_num);
}



void judge_psw(void)
{
	Bs8116_ReadKey();
	static u8 cnt=0;
	static u8 psw[7]; //存放输入的密码
	static u8 buf[2] ="\0"; //存放正确密码
	u8 fg=0;
	Display_String(50,120,GREEN,BLACK,16,(u8 *)"请输入密码:");
	if(!BS8116_IRQ)
	{
			buf[0] = Bs8116_ReadKey();
			while(!BS8116_IRQ);
			if(buf[0] !='#' && (buf[0]=='1' ||buf[0]=='2'||buf[0]=='3'||buf[0]=='4'||buf[0]=='5'||buf[0]=='6'||buf[0]=='7'||buf[0]=='8'||buf[0]=='9'||buf[0]=='0'||buf[0]=='*'))
			{
				printf("%c",buf[0]);
				switch(cnt)
				{
					case 0:	
									Display_String(139,120,GREEN,BLACK,16,(u8 *)"*"); break;
					case 1:	
									Display_String(149,120,GREEN,BLACK,16,(u8 *)"*"); break;
					case 2:	
									Display_String(159,120,GREEN,BLACK,16,(u8 *)"*"); break;
					case 3:	
									Display_String(169,120,GREEN,BLACK,16,(u8 *)"*"); break;
					case 4:	
									Display_String(179,120,GREEN,BLACK,16,(u8 *)"*"); break;
					case 5:	
									Display_String(189,120,GREEN,BLACK,16,(u8 *)"*"); break;
					case 6:	
									Display_String(199,120,GREEN,BLACK,16,(u8 *)"*"); break;
				}
				
				psw[cnt] = buf[0];
				cnt++;
			}
			else if(buf[0] =='#')
			{
				if(strcmp((const char*)psw ,(const char*)LOCK_TH.adm_pwd)==0)
				{
					st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
					Display_String(50,120,GREEN,BLACK,16,(u8 *)"正在进入管理员界面...");
					TIM5_Init_DS_MS(2000);
					cnt=0;
					memset(psw, '\0', sizeof psw); 
					//进入管理员界面
					Administrator_menu();	
					fg =1;
				}
				if(fg==0)
				{
					st7789vm_clear_display(0,16,239,223,LIGHTBLUE);
					if((strcmp((const char*)psw ,(const char*)LOCK_TH.lock_pwd)==0))
					{
						cnt=0;
						memset(psw, '\0', sizeof psw); 
						//开门
						TIM5_Init_DS_MS(500);
						PWM_Open_door();//开门
						TIM5_Init_DS_MS(500);
						Voice_Output(0x18); //播报开门成功《欢迎回家》
						while(VOICE_BUSY);
						PWM_Close_door();
						TIM5_Init_DS_MS(500);
						printf("\r\n");
					}
				else
				{
					st7789vm_clear_display(0,0,239,239,LIGHTBLUE);
					cnt=0;
					memset(psw, '\0', sizeof psw); 
					//开门失败
					Voice_Output(0x19);  //开门失败
					while(VOICE_BUSY);
					printf("\r\n");
				}
			}
				
		}
			
	}
}



