#include "led.h"
#include "key.h"
#include "sys.h"
#include "usart.h"	
#include "usart3.h"
#include "oled.h"
#include "rtc.h" 
#include "ADC.h"
#include "key.h"
#include "buj.h"
#include <time.h>
#include <stdio.h>
#include "hx711.h" 
#include "stime.h"
#include "sysset.h"
#include "hc05.h"
#include "gizwits_product.h"

dataPoint_t currentDataPoint;

extern u8 DHT11_Temp,DHT11_Hum;

//变量定义
u8  buff[30];
u8  sys_mode=0;
u8  app_mode=0;
u8  time1_flag=0;//定时时间1投食标志位
u8  time2_flag=0;//定时时间2投食标志位
u8  time3_flag=0;//定时时间3投食标志位
u8  time4_flag=0;//定时时间4投食标志位
u8  yybb_flag=0;//定时时间4投食标志位
u8  toufg_flag=0;//投食标志位
u16 allfood=0;	 //总食物
u16 onefood=500; //单次投放食物量
u16 yfood;		 //剩余食物
u16 Weight_Shiwu;//称重
u16 distance;
u16 Y_SHUL=10;
u8  WIFI_flag=0;

void display1(u8 x,u8 y,u16 value);
void display2(u8 x,u8 y,u16 value);
void display4(u8 x,u8 y,u16 value);
void display5(u8 x,u8 y,u16 value);
void display9(u8 x,u8 y,u16 value1,u16 value2);
void Set_System(u8 key);//系统设置
void Set_TouShi(u8 key);//投食配置
void display3(u8 x,u8 y,u16 value);
void WIFI_Contection(u8 key);//WiFi连接控制
void Set_System(u8 key);//系统设置
void yyti();

//协议初始化
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MS系统定时
    usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}

//数据采集
void userHandle(void)
{
	static u8 count=0;
	
	currentDataPoint.valuemode=app_mode;
	currentDataPoint.valuetoushi=toufg_flag;
	if(yfood <= Y_SHUL) 
	{
		currentDataPoint.valuebuchong = 1;
	}else currentDataPoint.valuebuchong = 0;
	
	if(++count >= 10){
		count = 0;
		currentDataPoint.valueshengyu=yfood;
	}
	
}

int main(void)
{	
	u32 temp;
	uart_init(115200);	 	 //串口1初始化为9600
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	BUJ_Init();
	LED_Init();				//实质上的步进电机端口初始化
	KEY_Init();	
	HX711_Init();
	JR6001_Init();
    OLED_Init();			//OLED初始化
    OLED_Clear();			//清屏
	Gizwits_Init();	
	SYS_Timestamp_Init();
	progress_bar(0,6);
	TIM1_PWM_Init(899,0); 			//不分频。PWM频率=72000/(899+1)=80Khz
 	TIM2_Cap_Init(0XFFFF,72-1);		//以1Mhz的频率计数 
  	while(1) 
	{
		
		//键值获取
		Key_Value = KEY_Scan(0);
		//压力检测
		Weight_Shiwu = hx711_wg = Get_Weight();		
		
		//超声波距离
		Wave_SRD_Strat();
		if(TIM2CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//溢出时间总和
			temp+=TIM2CH1_CAPTURE_VAL;		//得到总的高电平时间
			hc_05_distance = temp/59.0;
			distance = (int)hc_05_distance;
			TIM2CH1_CAPTURE_STA=0;			//开启下一次捕获
		}

		
		//定时时间点1读取
		if(time1_flag == 0 && NOW_timestamp == D1_timestamp)
		{
			time1_flag = 1;
		}if(NOW_timestamp != D1_timestamp) time1_flag = 0;
		//定时时间点2读取
		if(time2_flag == 0 && NOW_timestamp == D2_timestamp)
		{
			time2_flag = 1;
		}if(NOW_timestamp != D2_timestamp) time2_flag = 0;
		//定时时间点3读取
		if(time3_flag == 0 && NOW_timestamp == D3_timestamp)
		{
			time3_flag = 1;
		}if(NOW_timestamp != D3_timestamp) time3_flag = 0;
		//定时时间点4读取
		if(time4_flag == 0 && NOW_timestamp == D4_timestamp)
		{
			time4_flag = 1;
		}if(NOW_timestamp != D4_timestamp) time4_flag = 0;
		
		// 手动 自动 定时 模式控制
		if(sys_mode == 0 && Key_Value == 3) toufg_flag = 1;
		if(sys_mode == 1 && yfood < Y_SHUL) toufg_flag = 1;
		if(sys_mode == 2 && (time1_flag == 1 || time2_flag == 1 || time3_flag == 1 || time4_flag == 1)) toufg_flag = 1;
		
		
		//按键控制和定时控制投放食物
		if(toufg_flag == 1)
		{
			yybb_flag = 1;
			toufg_flag = 2;
			//设置定时时间点标志位
			time1_flag = 2;
			time2_flag = 2;
			time3_flag = 2;
			time4_flag = 2;
			if(Weight_Shiwu < onefood) BUJ_Mode = 1;//开启投放食物
		}
		
		//判断投放重量是否达标
		if(BUJ_Mode == 1 && Weight_Shiwu >= onefood)
		{
			toufg_flag = 0;
			BUJ_Mode = 0;
		}
		
		if(yybb_flag == 1 && distance >= 10) {
			JR6001_SongControl(1,1);
		}else	yybb_flag = 0;
		
		userHandle();					//用户数据采集
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//机智云协议处理
		WIFI_Contection(Key_Value);//WiFi连接控制
		if(currentDataPoint.valuemode == 0 && app_mode == 1) sys_mode = 0;
		if(currentDataPoint.valuemode == 1 && app_mode == 0) sys_mode = 3;
		app_mode = currentDataPoint.valuemode;
		
		//远程投食控制
		if(sys_mode == 3 && currentDataPoint.valuetoushi == 1) toufg_flag = 1;
		
		//系统显示
		display_time(0,0,calendar.sec);
		display2(0,2,Weight_Shiwu);
		display3(0,4,distance);
		
		//剩余食物
		yfood = hx711_wg/(onefood*1.0)*100;
		if(yfood >= 100) yfood = 100;
		Moving_progress_bar(yfood,6);
		
		SYS_Set(Key_Value);
	}			
}

void WIFI_Contection(u8 key)//WiFi连接控制
{
	if(key==4)
	{
		printf("WIFI进入AirLink连接模式\r\n");
		gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-link模式接入
	}		
}

void display1(u8 x,u8 y,u16 value)
{
	GUI_DrawFont16(x,y,"剩余食物",0);
	OLED_ShowString(x+64,y,":",16);
}

void display2(u8 x,u8 y,u16 value)
{
	GUI_DrawFont16(x,y,"称重检测",0);
	OLED_ShowString(x+64,y,":",16);
	OLED_ShowNumber(x+72,y,value,4,16);
	OLED_ShowString(x+104,y,"g",16);
}

void display3(u8 x,u8 y,u16 value)
{
	GUI_DrawFont16(x,y,"超声测距",0);
	OLED_ShowString(x+64,y,":",16);
	sprintf(buff,"%3dcm",value);
	OLED_ShowString(x+72,y,buff,16);
}


