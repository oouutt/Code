#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

extern u8  Key_Value;//����ֵ

//���尴������
#define WK_UP_GPIOX    GPIOB
#define WK_UP_GPIOPin  GPIO_Pin_4

#define KEY0_GPIOX     GPIOB
#define KEY0_GPIOPin   GPIO_Pin_5

#define KEY1_GPIOX     GPIOB
#define KEY1_GPIOPin   GPIO_Pin_6

#define KEY2_GPIOX     GPIOB
#define KEY2_GPIOPin   GPIO_Pin_7

#define HW1_GPIOX     GPIOB
#define HW1_GPIOPin   GPIO_Pin_15

#define HW2_GPIOX     GPIOA
#define HW2_GPIOPin   GPIO_Pin_8

#define KEY0    GPIO_ReadInputDataBit(KEY0_GPIOX,KEY0_GPIOPin)//��ȡ����0
#define KEY1    GPIO_ReadInputDataBit(KEY1_GPIOX,KEY1_GPIOPin)//��ȡ����1
#define KEY2    GPIO_ReadInputDataBit(KEY2_GPIOX,KEY2_GPIOPin)//��ȡ����2 
#define WK_UP   GPIO_ReadInputDataBit(WK_UP_GPIOX,WK_UP_GPIOPin)//��ȡ����3(WK_UP) 

#define HW1     GPIO_ReadInputDataBit(HW1_GPIOX,HW1_GPIOPin)
#define HW2     GPIO_ReadInputDataBit(HW2_GPIOX,HW2_GPIOPin)

#define KEY0_PRES 	2	//KEY0����
#define KEY1_PRES	3	//KEY1����
#define KEY2_PRES	4	//KEY2����
#define WKUP_PRES   1	//KEY_UP����(��WK_UP/KEY_UP)

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
