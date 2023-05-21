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

//��������
u8  buff[30];
u8  sys_mode=0;
u8  app_mode=0;
u8  time1_flag=0;//��ʱʱ��1Ͷʳ��־λ
u8  time2_flag=0;//��ʱʱ��2Ͷʳ��־λ
u8  time3_flag=0;//��ʱʱ��3Ͷʳ��־λ
u8  time4_flag=0;//��ʱʱ��4Ͷʳ��־λ
u8  yybb_flag=0;//��ʱʱ��4Ͷʳ��־λ
u8  toufg_flag=0;//Ͷʳ��־λ
u16 allfood=0;	 //��ʳ��
u16 onefood=500; //����Ͷ��ʳ����
u16 yfood;		 //ʣ��ʳ��
u16 Weight_Shiwu;//����
u16 distance;
u16 Y_SHUL=10;
u8  WIFI_flag=0;

void display1(u8 x,u8 y,u16 value);
void display2(u8 x,u8 y,u16 value);
void display4(u8 x,u8 y,u16 value);
void display5(u8 x,u8 y,u16 value);
void display9(u8 x,u8 y,u16 value1,u16 value2);
void Set_System(u8 key);//ϵͳ����
void Set_TouShi(u8 key);//Ͷʳ����
void display3(u8 x,u8 y,u16 value);
void WIFI_Contection(u8 key);//WiFi���ӿ���
void Set_System(u8 key);//ϵͳ����
void yyti();

//Э���ʼ��
void Gizwits_Init(void)
{	
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
    usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}

//���ݲɼ�
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
	uart_init(115200);	 	 //����1��ʼ��Ϊ9600
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	BUJ_Init();
	LED_Init();				//ʵ���ϵĲ�������˿ڳ�ʼ��
	KEY_Init();	
	HX711_Init();
	JR6001_Init();
    OLED_Init();			//OLED��ʼ��
    OLED_Clear();			//����
	Gizwits_Init();	
	SYS_Timestamp_Init();
	progress_bar(0,6);
	TIM1_PWM_Init(899,0); 			//����Ƶ��PWMƵ��=72000/(899+1)=80Khz
 	TIM2_Cap_Init(0XFFFF,72-1);		//��1Mhz��Ƶ�ʼ��� 
  	while(1) 
	{
		
		//��ֵ��ȡ
		Key_Value = KEY_Scan(0);
		//ѹ�����
		Weight_Shiwu = hx711_wg = Get_Weight();		
		
		//����������
		Wave_SRD_Strat();
		if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;					//���ʱ���ܺ�
			temp+=TIM2CH1_CAPTURE_VAL;		//�õ��ܵĸߵ�ƽʱ��
			hc_05_distance = temp/59.0;
			distance = (int)hc_05_distance;
			TIM2CH1_CAPTURE_STA=0;			//������һ�β���
		}

		
		//��ʱʱ���1��ȡ
		if(time1_flag == 0 && NOW_timestamp == D1_timestamp)
		{
			time1_flag = 1;
		}if(NOW_timestamp != D1_timestamp) time1_flag = 0;
		//��ʱʱ���2��ȡ
		if(time2_flag == 0 && NOW_timestamp == D2_timestamp)
		{
			time2_flag = 1;
		}if(NOW_timestamp != D2_timestamp) time2_flag = 0;
		//��ʱʱ���3��ȡ
		if(time3_flag == 0 && NOW_timestamp == D3_timestamp)
		{
			time3_flag = 1;
		}if(NOW_timestamp != D3_timestamp) time3_flag = 0;
		//��ʱʱ���4��ȡ
		if(time4_flag == 0 && NOW_timestamp == D4_timestamp)
		{
			time4_flag = 1;
		}if(NOW_timestamp != D4_timestamp) time4_flag = 0;
		
		// �ֶ� �Զ� ��ʱ ģʽ����
		if(sys_mode == 0 && Key_Value == 3) toufg_flag = 1;
		if(sys_mode == 1 && yfood < Y_SHUL) toufg_flag = 1;
		if(sys_mode == 2 && (time1_flag == 1 || time2_flag == 1 || time3_flag == 1 || time4_flag == 1)) toufg_flag = 1;
		
		
		//�������ƺͶ�ʱ����Ͷ��ʳ��
		if(toufg_flag == 1)
		{
			yybb_flag = 1;
			toufg_flag = 2;
			//���ö�ʱʱ����־λ
			time1_flag = 2;
			time2_flag = 2;
			time3_flag = 2;
			time4_flag = 2;
			if(Weight_Shiwu < onefood) BUJ_Mode = 1;//����Ͷ��ʳ��
		}
		
		//�ж�Ͷ�������Ƿ���
		if(BUJ_Mode == 1 && Weight_Shiwu >= onefood)
		{
			toufg_flag = 0;
			BUJ_Mode = 0;
		}
		
		if(yybb_flag == 1 && distance >= 10) {
			JR6001_SongControl(1,1);
		}else	yybb_flag = 0;
		
		userHandle();					//�û����ݲɼ�
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//������Э�鴦��
		WIFI_Contection(Key_Value);//WiFi���ӿ���
		if(currentDataPoint.valuemode == 0 && app_mode == 1) sys_mode = 0;
		if(currentDataPoint.valuemode == 1 && app_mode == 0) sys_mode = 3;
		app_mode = currentDataPoint.valuemode;
		
		//Զ��Ͷʳ����
		if(sys_mode == 3 && currentDataPoint.valuetoushi == 1) toufg_flag = 1;
		
		//ϵͳ��ʾ
		display_time(0,0,calendar.sec);
		display2(0,2,Weight_Shiwu);
		display3(0,4,distance);
		
		//ʣ��ʳ��
		yfood = hx711_wg/(onefood*1.0)*100;
		if(yfood >= 100) yfood = 100;
		Moving_progress_bar(yfood,6);
		
		SYS_Set(Key_Value);
	}			
}

void WIFI_Contection(u8 key)//WiFi���ӿ���
{
	if(key==4)
	{
		printf("WIFI����AirLink����ģʽ\r\n");
		gizwitsSetMode(WIFI_AIRLINK_MODE);//Air-linkģʽ����
	}		
}

void display1(u8 x,u8 y,u16 value)
{
	GUI_DrawFont16(x,y,"ʣ��ʳ��",0);
	OLED_ShowString(x+64,y,":",16);
}

void display2(u8 x,u8 y,u16 value)
{
	GUI_DrawFont16(x,y,"���ؼ��",0);
	OLED_ShowString(x+64,y,":",16);
	OLED_ShowNumber(x+72,y,value,4,16);
	OLED_ShowString(x+104,y,"g",16);
}

void display3(u8 x,u8 y,u16 value)
{
	GUI_DrawFont16(x,y,"�������",0);
	OLED_ShowString(x+64,y,":",16);
	sprintf(buff,"%3dcm",value);
	OLED_ShowString(x+72,y,buff,16);
}


