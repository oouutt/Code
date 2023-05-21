#include "rtc.h" 
#include "key.h" 
#include "oled.h" 
#include "usart.h"
#include "stime.h"
#include "dtime.h"

_calendar_obj calendar;//ʱ�ӽṹ��

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

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //�����ܱ�4����
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}	 			   
//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//��������ݵ��������
	{
		if(Is_Leap_Year(t))seccount+=31622400;//�����������
		else seccount+=31536000;			  //ƽ���������
	}
	smon-=1;
	for(t=0;t<smon;t++)	   //��ǰ���·ݵ����������
	{
		seccount+=(u32)mon_table[t]*86400;//�·����������
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//����2�·�����һ���������	   
	}
	seccount+=(u32)(sday-1)*86400;//��ǰ�����ڵ���������� 
	seccount+=(u32)hour*3600;//Сʱ������
    seccount+=(u32)min*60;	 //����������
	seccount+=sec;//�������Ӽ���ȥ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(seccount);	//����RTC��������ֵ

	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	return 0;	    
}


//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// ���Ϊ21����,�������100  
	if (yearH>19)yearL+=100;
	// ����������ֻ��1900��֮���  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}


//ϵͳʱ�����ʼ��
void SYS_Timestamp_Init()
{
	D1_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[0][0],D_time[0][1],D_time[0][2]);
	D2_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[1][0],D_time[1][1],D_time[1][2]);	
	D3_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[2][0],D_time[2][1],D_time[2][2]);	
	D4_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,D_time[3][0],D_time[3][1],D_time[3][2]);	

	//	printf("%d %d\r\n",DS_timestamp,DE_timestamp);
}

//����ϵͳʱ��
void Set_Systime(u8 obj,u8 key)
{
	static u8 Systimeing_L;
	static u8 LastSystimeing_L;
	if(obj == 1 && key == 3)
	{
		OLED_Clear();
		GUI_DrawFont16(0,0,"ϵͳʱ������",0);
		Systime[0]=calendar.hour;
		Systime[1]=calendar.min;
		Systime[2]=calendar.sec;
		while(1)
		{
			Key_Value = KEY_Scan(0);
			
			sprintf(rtc_timebuff,"SYST:%2d-%2d-%2d",Systime[0],Systime[1],Systime[2]);//ָ��ʱ���ʽ
			OLED_ShowString(0,2,rtc_timebuff,16);//��ʾʱ��

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
				OLED_Clear();			//����
				//RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,Systime[0],Systime[1],Systime[2]);  //����ʱ��		
				time_stamp = GetTick(YAER,MONTH,DATA,Systime[0],Systime[1],Systime[2]);  //����ʱ��	
				break;
			}
		}
	}
}

void Set_timing_time(u8 obj,u8 key)//���ڶ�ʱʱ��
{
	static u8 DH=0,DL=0,last_DH=1,last_DL=0;
	if(obj == 2 && key == 3)
	{
		OLED_Clear();		//����
		GUI_DrawFont16(0,0,"��ʱʱ��",0);
		while(1)
		{
			Key_Value = KEY_Scan(0);
			
			sprintf(rtc_timebuff,"TIME1:%2d-%2d-%2d",D_time[0][0],D_time[0][1],D_time[0][2]);//ָ��ʱ���ʽ
			OLED_ShowString(0,0,rtc_timebuff,16);//��ʾʱ��
			sprintf(rtc_timebuff,"TIME2:%2d-%2d-%2d",D_time[1][0],D_time[1][1],D_time[1][2]);//ָ��ʱ���ʽ
			OLED_ShowString(0,2,rtc_timebuff,16);//��ʾʱ��
			sprintf(rtc_timebuff,"TIME3:%2d-%2d-%2d",D_time[2][0],D_time[2][1],D_time[2][2]);//ָ��ʱ���ʽ
			OLED_ShowString(0,4,rtc_timebuff,16);//��ʾʱ��
			sprintf(rtc_timebuff,"TIME4:%2d-%2d-%2d",D_time[3][0],D_time[3][1],D_time[3][2]);//ָ��ʱ���ʽ
			OLED_ShowString(0,6,rtc_timebuff,16);//��ʾʱ��
			
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
				OLED_Clear();		//����
				break;
			}
		}
	}
}

//t = calendar.sec
//display_time( 0, 0, calendar.sec);
void display_time(u8 x,u8 y,u8 t)//��ʾʱ��
{
	static u8 last_t;
	if(last_t!=t)//��ȡʱ��
	{
		t=calendar.sec;
		if(t!=0)
		{
			sprintf(rtc_timebuff,"Time:%2d-%2d-%2d",calendar.hour,calendar.min,calendar.sec);//ָ��ʱ���ʽ
		}else{
			sprintf(rtc_timebuff,"Time:%2d-%2d-00",calendar.hour,calendar.min);//ָ��ʱ���ʽ
		}
		OLED_ShowString(x,y,rtc_timebuff,16);//��ʾʱ��
		NOW_timestamp = GetTick(RTC_YAER,RTC_MONTH,RTC_DATA,calendar.hour,calendar.min,calendar.sec);//ת����ǰʱ��ʱ���
	}
	last_t = t;
}




