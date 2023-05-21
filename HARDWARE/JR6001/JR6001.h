#ifndef __JR6001_H
#define __JR6001_H

#include "sys.h"

//��Ҫ������ѡ��Ĵ�����Ҫ�޸Ĵ���ʱ��
#define  JR6001_USART			USART2
#define	 JR6001_USART_Baud		9600
#define  JR6001_USART_CLK		RCC_APB1Periph_USART2

//JR6001��������ʱ��
#define  JR6001_USART_GPIO_CLK   RCC_APB2Periph_GPIOA

//JR6001���ڷ�����������
#define  JR6001_TX_GPIO_PORT    GPIOA		   
#define  JR6001_TX_GPIO_PIN		GPIO_Pin_2
#define  JR6001_TX_GPIO_Speed	GPIO_Speed_50MHz
#define  JR6001_TX_GPIO_Mode	GPIO_Mode_AF_PP

//JR6001���ڽ����������� 
#define  JR6001_RX_GPIO_PORT    GPIOA			   
#define  JR6001_RX_GPIO_PIN		GPIO_Pin_3
#define  JR6001_RX_GPIO_Speed	GPIO_Speed_50MHz
#define  JR6001_RX_GPIO_Mode	GPIO_Mode_IN_FLOATING

//��æʹ��
#define  JR6001_BUSY 1 		 	//0.ʧ�� 1.ʹ�� 

//JR6001��æ�������� �ߵ�ƽæ
#define  JR6001_BUSY_GPIO_CLK   RCC_APB2Periph_GPIOC
#define  JR6001_BUSY_GPIO_PORT  GPIOC	   
#define  JR6001_BUSY_GPIO_PIN	GPIO_Pin_13
#define  JR6001_BUSY_GPIO_Mode	GPIO_Mode_IPD

#define  JR6001_BUSY_IO    		GPIO_ReadInputDataBit(JR6001_BUSY_GPIO_PORT,JR6001_BUSY_GPIO_PIN)//��ȡ��æ����

//��ʱ�����ض��� JR6001��ʱ�������壬�û����и���Ϊ�Լ�����ʱ����
#define  JR6001_ms(ms)    		delay_ms(ms)         
#define  JR6001_us(us)   		delay_us(us)

//JR6001��������[���� ��ͣ ��һ�� ��һ�� ������ ������]
#define  Play					"A2"	
#define  Suspend				"A3"
#define  On_Song				"A5"
#define  Next_Song				"A6"
#define  Volumeadd				"B0"
#define  Volumesub				"B1"

#endif  

