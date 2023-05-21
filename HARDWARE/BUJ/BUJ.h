#ifndef __BUJ_H
#define __BUJ_H	 
#include "sys.h"

#define BUJ1_GPIOX    GPIOA
#define BUJ1_GPIOPin  GPIO_Pin_11

#define BUJ2_GPIOX    GPIOA
#define BUJ2_GPIOPin  GPIO_Pin_12

#define BUJ3_GPIOX    GPIOA
#define BUJ3_GPIOPin  GPIO_Pin_15

#define BUJ4_GPIOX    GPIOB
#define BUJ4_GPIOPin  GPIO_Pin_3

#define BUJ1 PAout(11)
#define BUJ2 PAout(12)	
#define BUJ3 PAout(15)		
#define BUJ4 PBout(3)

extern u8     BUJ_Mode;
extern u8     BUJ_CONTROL;

void BUJ_Init(void);//≥ı ºªØ
void TIM2_Int_Init(u16 arr,u16 psc);
void BUJING_Cotrol(u8 mode,u16 time,u16 count);
		 				    
#endif
