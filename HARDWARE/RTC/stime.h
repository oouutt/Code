#ifndef __stime_H
#define __stime_H	    

#include "sys.h"

#define RTC_YAER  2020
#define RTC_MONTH 12
#define RTC_DATA  12

//时间结构体
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//公历日月年周
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//日历结构体

extern long NOW_timestamp;
extern long D1_timestamp;
extern long D2_timestamp;
extern long D3_timestamp;
extern long D4_timestamp;
extern long LS_timestamp;
extern long LE_timestamp;
extern long NS_timestamp;
extern long NE_timestamp;

extern int  LN_time[4][3];
extern u16  D_time[4][3];
extern int  Systime[3];

#endif


