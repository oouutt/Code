#include "BUJ.h"
#include "led.h"
#include "rtc.h" 
#include "stime.h"
#include "dtime.h"

u8   BUJ_Mode=0;
u8   BUJ_CONTROL;
u32  BUJ_COUNT;
void TIM4_Int_Init(u16 arr,u16 psc)
{
   	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����
							 
}

u16 times=0;
/*�жϱ�־������������������*/
//��ʱ��2�жϷ������
void TIM4_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ
		
		LIB_BUJ_MCOUNT(&BUJ_CONTROL,BUJ_Mode,&BUJ_COUNT);
		
		if(BUJ_CONTROL == 0) {BUJ1=1; BUJ2=0; BUJ3=0;BUJ4=0;}
			
		if(BUJ_CONTROL == 1) {BUJ1=0; BUJ2=1; BUJ3=0;BUJ4=0;}
			
		if(BUJ_CONTROL == 2) {BUJ1=0; BUJ2=0; BUJ3=1;BUJ4=0;}
			
		if(BUJ_CONTROL == 3) {BUJ1=0; BUJ2=0; BUJ3=0;BUJ4=1;}
		
		if(++times>=500)
		{
			times = 0;
			time_stamp++;
			RTC_Time_Get(time_stamp);
		}
		
	}
}
   
//�������IO��ʼ��
void BUJ_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 
		
	 GPIO_InitStructure.GPIO_Pin   =  BUJ1_GPIOPin;				
	 GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 		 
	 GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;		
	 GPIO_Init(BUJ1_GPIOX, &GPIO_InitStructure);
		
	 GPIO_InitStructure.GPIO_Pin   =  BUJ2_GPIOPin;				
	 GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 		 
	 GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;		
	 GPIO_Init(BUJ2_GPIOX, &GPIO_InitStructure);

	 GPIO_InitStructure.GPIO_Pin   =  BUJ3_GPIOPin;				
	 GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 		 
	 GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;		
	 GPIO_Init(BUJ3_GPIOX, &GPIO_InitStructure);	

	 GPIO_InitStructure.GPIO_Pin   =  BUJ4_GPIOPin;				
	 GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 		 
	 GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;		
	 GPIO_Init(BUJ4_GPIOX, &GPIO_InitStructure);
	 
	 TIM4_Int_Init(19,7199);		//2MSϵͳ��ʱ ��������
}
