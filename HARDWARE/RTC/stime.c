#include "rtc.h" 
#include "key.h" 
#include "oled.h" 
#include "usart.h"
#include "stime.h"
#include "dtime.h"

_calendar_obj calendar;//时钟结构体

u8   rtc_timebuff[20];

long NOW_timestamp;
long D1_timestamp;
long D2_timestamp;
long D3_timestamp;
long D4_timestamp;

int LN_time[4][3]={
				{20,21,00},
				{20,21,30},	
				{20,22,00},
				{20,22,30},
};

u16  D_time[4][3]={
				{12,12,25},
				{12,12,35},
				{12,12,45},
				{12,12,55},
};

int Systime[3] = {20,20,00};

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
	{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
	{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
    seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(seccount);	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	return 0;	    
}


//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}


//系统时间戳初始化
void SYS_Timestamp_Init()
{
	D1_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[0][0],D_time[0][1],D_time[0][2]);
	D2_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[1][0],D_time[1][1],D_time[1][2]);	
	D3_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[2][0],D_time[2][1],D_time[2][2]);	
	D4_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[3][0],D_time[3][1],D_time[3][2]);	

	//	printf("%d %d\r\n",DS_timestamp,DE_timestamp);
}

//设置系统时间
void Set_Systime(u8 obj,u8 key)
{
	static u8 Systimeing_L;
	static u8 LastSystimeing_L;
	if(obj == 1 && key == 3)
	{
		OLED_Clear();
		GUI_DrawFont16(0,0,"系统时间设置",0);
		Systime[0]=calendar.hour;
		Systime[1]=calendar.min;
		Systime[2]=calendar.sec;
		while(1)
		{
			Key_Value = KEY_Scan(0);
			
			sprintf(rtc_timebuff,"SYST:%2d-%2d-%2d",Systime[0],Systime[1],Systime[2]);//指定时间格式
			OLED_ShowString(0,2,rtc_timebuff,16);//显示时间

			if(Key_Value==1)
			{
				Systimeing_L++;
				if(Systimeing_L>=3)  Systimeing_L = 0;
				OLED_ShowString(48+Systimeing_L*24,2,"*",16);
				LastSystimeing_L = Systimeing_L;
				delay_ms(100);
			}
			
			if(Key_Value==3) Systime[Systimeing_L]++;

			if(Key_Value==4) Systime[Systimeing_L]--;

			Datalimt_sfm(Systime);
			
			if(Key_Value == 2)
			{
				OLED_Clear();			//清屏
				//RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,Systime[0],Systime[1],Systime[2]);  //设置时间		
				time_stamp = GetTick(YAER,MONTH,DATA,Systime[0],Systime[1],Systime[2]);  //设置时间	
				break;
			}
		}
	}
}

void Set_timing_time(u8 obj,u8 key)//调节定时时间
{
	static u8 DH=0,DL=0,last_DH=1,last_DL=0;
	if(obj == 2 && key == 3)
	{
		OLED_Clear();		//清屏
		GUI_DrawFont16(0,0,"定时时间",0);
		while(1)
		{
			Key_Value = KEY_Scan(0);
			
			sprintf(rtc_timebuff,"TIME1:%2d-%2d-%2d",D_time[0][0],D_time[0][1],D_time[0][2]);//指定时间格式
			OLED_ShowString(0,0,rtc_timebuff,16);//显示时间
			sprintf(rtc_timebuff,"TIME2:%2d-%2d-%2d",D_time[1][0],D_time[1][1],D_time[1][2]);//指定时间格式
			OLED_ShowString(0,2,rtc_timebuff,16);//显示时间
			sprintf(rtc_timebuff,"TIME3:%2d-%2d-%2d",D_time[2][0],D_time[2][1],D_time[2][2]);//指定时间格式
			OLED_ShowString(0,4,rtc_timebuff,16);//显示时间
			sprintf(rtc_timebuff,"TIME4:%2d-%2d-%2d",D_time[3][0],D_time[3][1],D_time[3][2]);//指定时间格式
			OLED_ShowString(0,6,rtc_timebuff,16);//显示时间
			
			if(Key_Value==1)
			{
				DL++;
				if(DL>=3) 
				{	
					DL = 0;
					DH++;
				   if(DH>=4)  DH = 0;
				}
				OLED_ShowString(DL*24+56,DH*2,"*",16);
				delay_ms(100);
			}
			
			if(Key_Value==3) D_time[DH][DL]++;
			
			if(Key_Value==4) D_time[DH][DL]--;
		
			if(Key_Value == 2)
			{
				Key_Value = 0;
				
				SYS_Timestamp_Init();
				OLED_Clear();		//清屏
				break;
			}
		}
	}
}

//t = calendar.sec
//display_time( 0, 0, calendar.sec);
void display_time(u8 x,u8 y,u8 t)//显示时间
{
	static u8 last_t;
	if(last_t!=t)//获取时间
	{
		t=calendar.sec;
		if(t!=0)
		{
			sprintf(rtc_timebuff,"Time:%2d-%2d-%2d",calendar.hour,calendar.min,calendar.sec);//指定时间格式
		}else{
			sprintf(rtc_timebuff,"Time:%2d-%2d-00",calendar.hour,calendar.min);//指定时间格式
		}
		OLED_ShowString(x,y,rtc_timebuff,16);//显示时间
		NOW_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,calendar.hour,calendar.min,calendar.sec);//转化当前时间时间戳
	}
	last_t = t;
}




