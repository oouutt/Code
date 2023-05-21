#include "rtc.h" 
#include "key.h" 
#include "oled.h" 
#include "usart.h"
#include "stime.h"
#include "dtime.h"

u32  time_stamp = 1577880732;

//得到当前的时间
//返回值:0,成功;其他:错误代码.
u8 RTC_Time_Get(u32 times)
{
	static u16 daycnt=0;
	static u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=times;	 
 	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		calendar.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//得到月份
		calendar.w_date=temp+1;  	//得到日期 
	}
	temp=timecount%86400;     		//得到秒钟数   	   
	calendar.hour=temp/3600;     	//小时
	calendar.min=(temp%3600)/60; 	//分钟	
	calendar.sec=(temp%3600)%60; 	//秒钟
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//获取星期 
	//printf("%d-%d-%d %d:%d:%d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
	return 0;
}	

//// TIM4_Int_Init(10000,7199);//1S系统定时
//// time_stamp = GetTick(YAER,MONTH,DATA,Systime[0],Systime[1],Systime[2]);  //设置时间	
////使能定时器4,使能中断.(定时器4位系统时间提供时基)
//void TIM4_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4时钟使能 
// 
//	//TIM4初始化设置
// 	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//TIM4 允许更新，触发中断

//	//TIM4中断分组配置
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级03级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	TIM_Cmd(TIM4, ENABLE);  //使能TIM4	

//	//SYS_Timestamp_Init();
//}

////定时器4中断服务程序	 
//void TIM4_IRQHandler(void)
//{ 		    		  			    
//	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)//溢出中断
//	{
//		time_stamp++;
//		RTC_Time_Get(time_stamp);
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//	}				     
//}



