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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设
							 
}

u16 times=0;
/*中断标志必须清除，否则程序卡死*/
//定时器2中断服务程序
void TIM4_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源
		
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
   
//步进电机IO初始化
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
	 
	 TIM4_Int_Init(19,7199);		//2MS系统定时 步进脉冲
}
