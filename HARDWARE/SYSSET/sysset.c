#include "stm32f10x.h"
#include "sysset.h"
#include "usart.h"	
#include "oled.h"
#include "key.h"
#include "led.h"
#include "stime.h"
#include <math.h>

u16 null = 0;

u8  Str_buff[20];

extern u8  sys_mode;
extern u16 onefood;
extern u16 Y_SHUL;

u8 *sysmode_str[SMnum]={	
	"�ֶ�",
	"�Զ�",
	"��ʱ",
	"Զ��"
};    

u16 *Data[Data_Num]={&null,&null,&null,&onefood,&Y_SHUL,&null};

u16 Change[Data_Num]={0,0,0,10,2,0};

u8 *display_str[Data_Num]={	
    "����ģʽ",
	"ϵͳʱ��",
	"��ʱʱ��",
	"Ͷʳ����",
	"��������",
	"�˳�����"
};

u16 obj=0,start=1;

void display_sysmode(u8 x,u8 y,u8 aim,u8 *srt);

void SYS_Set(u8 key)
{
	if(key == KEY1_Value)
	{
		OLED_Clear();			//����
		
		while(1)
		{
			key = KEY_Scan(0);

			OLED_DISPLAY1(&obj, key, Data_Num,display_str,Data,1);

			if(obj>=0&&obj<Element_MAX-Multilevel_Menus_Num)//�����˵�����
			{
				set_sysmode(obj,0,key,(u16*)&sys_mode,sysmode_str,SMnum);
				Set_Systime(obj,key);
				Set_timing_time(obj,key);
			}else{		//�˳�����
				if(key == KEY3_Value) 
				{
					OLED_Clear();	//����
					progress_bar(0,6);
					break;
				}
			}
		}
	}
}

void display_sysmode(u8 x,u8 y,u8 aim,u8 *srt)
{
	GUI_DrawFont16(x,y,"����ģʽ",0);
	OLED_ShowString(x+64,y,":",16);
	GUI_DrawFont16(x+72,y,srt,0);
}