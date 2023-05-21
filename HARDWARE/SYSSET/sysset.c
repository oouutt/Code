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
	"手动",
	"自动",
	"定时",
	"远程"
};    

u16 *Data[Data_Num]={&null,&null,&null,&onefood,&Y_SHUL,&null};

u16 Change[Data_Num]={0,0,0,10,2,0};

u8 *display_str[Data_Num]={	
    "工作模式",
	"系统时间",
	"定时时间",
	"投食重量",
	"余粮补充",
	"退出设置"
};

u16 obj=0,start=1;

void display_sysmode(u8 x,u8 y,u8 aim,u8 *srt);

void SYS_Set(u8 key)
{
	if(key == KEY1_Value)
	{
		OLED_Clear();			//清屏
		
		while(1)
		{
			key = KEY_Scan(0);

			OLED_DISPLAY1(&obj, key, Data_Num,display_str,Data,1);

			if(obj>=0&&obj<Element_MAX-Multilevel_Menus_Num)//单级菜单控制
			{
				set_sysmode(obj,0,key,(u16*)&sys_mode,sysmode_str,SMnum);
				Set_Systime(obj,key);
				Set_timing_time(obj,key);
			}else{		//退出控制
				if(key == KEY3_Value) 
				{
					OLED_Clear();	//清屏
					progress_bar(0,6);
					break;
				}
			}
		}
	}
}

void display_sysmode(u8 x,u8 y,u8 aim,u8 *srt)
{
	GUI_DrawFont16(x,y,"工作模式",0);
	OLED_ShowString(x+64,y,":",16);
	GUI_DrawFont16(x+72,y,srt,0);
}