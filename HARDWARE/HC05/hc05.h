#ifndef __HC05_H
#define __HC05_H
#include "sys.h"

extern float hc_05_distance;
extern u8  TIM2CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ 
 
void TIM1_PWM_Init(u16 arr,u16 psc);
void TIM2_Cap_Init(u16 arr,u16 psc);
void Wave_SRD_Strat(void);

#endif
