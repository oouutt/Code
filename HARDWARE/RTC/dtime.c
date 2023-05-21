#include "rtc.h" 
#include "key.h" 
#include "oled.h" 
#include "usart.h"
#include "stime.h"
#include "dtime.h"

u32  time_stamp = 1577880732;

//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
u8 RTC_Time_Get(u32 times)
{
	static u16 daycnt=0;
	static u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
    timecount=times;	 
 	temp=timecount/86400;   //�õ�����(��������Ӧ��)
	if(daycnt!=temp)//����һ����
	{	  
		daycnt=temp;
		temp1=1970;	//��1970�꿪ʼ
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//������
			{
				if(temp>=366)temp-=366;//�����������
				else {temp1++;break;}  
			}
			else temp-=365;	  //ƽ�� 
			temp1++;  
		}   
		calendar.w_year=temp1;//�õ����
		temp1=0;
		while(temp>=28)//������һ����
		{
			if(Is_Leap_Year(calendar.w_year)&&temp1==1)//�����ǲ�������/2�·�
			{
				if(temp>=29)temp-=29;//�����������
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		calendar.w_month=temp1+1;	//�õ��·�
		calendar.w_date=temp+1;  	//�õ����� 
	}
	temp=timecount%86400;     		//�õ�������   	   
	calendar.hour=temp/3600;     	//Сʱ
	calendar.min=(temp%3600)/60; 	//����	
	calendar.sec=(temp%3600)%60; 	//����
	calendar.week=RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//��ȡ���� 
	//printf("%d-%d-%d %d:%d:%d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
	return 0;
}	

//// TIM4_Int_Init(10000,7199);//1Sϵͳ��ʱ
//// time_stamp = GetTick(YAER,MONTH,DATA,Systime[0],Systime[1],Systime[2]);  //����ʱ��	
////ʹ�ܶ�ʱ��4,ʹ���ж�.(��ʱ��4λϵͳʱ���ṩʱ��)
//void TIM4_Int_Init(u16 arr,u16 psc)
//{
//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //TIM4ʱ��ʹ�� 
// 
//	//TIM4��ʼ������
// 	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
// 
//	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE );//TIM4 ������£������ж�

//	//TIM4�жϷ�������
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�03��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4	

//	//SYS_Timestamp_Init();
//}

////��ʱ��4�жϷ������	 
//void TIM4_IRQHandler(void)
//{ 		    		  			    
//	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)//����ж�
//	{
//		time_stamp++;
//		RTC_Time_Get(time_stamp);
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
//	}				     
//}



