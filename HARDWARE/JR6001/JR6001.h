#ifndef __JR6001_H
#define __JR6001_H

#include "sys.h"

//重要：根据选择的串口需要修改串口时钟
#define  JR6001_USART			USART2
#define	 JR6001_USART_Baud		9600
#define  JR6001_USART_CLK		RCC_APB1Periph_USART2

//JR6001串口引脚时钟
#define  JR6001_USART_GPIO_CLK   RCC_APB2Periph_GPIOA

//JR6001串口发送引脚配置
#define  JR6001_TX_GPIO_PORT    GPIOA		   
#define  JR6001_TX_GPIO_PIN		GPIO_Pin_2
#define  JR6001_TX_GPIO_Speed	GPIO_Speed_50MHz
#define  JR6001_TX_GPIO_Mode	GPIO_Mode_AF_PP

//JR6001串口接收引脚配置 
#define  JR6001_RX_GPIO_PORT    GPIOA			   
#define  JR6001_RX_GPIO_PIN		GPIO_Pin_3
#define  JR6001_RX_GPIO_Speed	GPIO_Speed_50MHz
#define  JR6001_RX_GPIO_Mode	GPIO_Mode_IN_FLOATING

//查忙使能
#define  JR6001_BUSY 1 		 	//0.失能 1.使能 

//JR6001查忙引脚配置 高电平忙
#define  JR6001_BUSY_GPIO_CLK   RCC_APB2Periph_GPIOC
#define  JR6001_BUSY_GPIO_PORT  GPIOC	   
#define  JR6001_BUSY_GPIO_PIN	GPIO_Pin_13
#define  JR6001_BUSY_GPIO_Mode	GPIO_Mode_IPD

#define  JR6001_BUSY_IO    		GPIO_ReadInputDataBit(JR6001_BUSY_GPIO_PORT,JR6001_BUSY_GPIO_PIN)//读取查忙引脚

//延时函数重定义 JR6001延时函数定义，用户自行更改为自己的延时函数
#define  JR6001_ms(ms)    		delay_ms(ms)         
#define  JR6001_us(us)   		delay_us(us)

//JR6001串口命令[播放 暂停 上一曲 下一曲 音量加 音量减]
#define  Play					"A2"	
#define  Suspend				"A3"
#define  On_Song				"A5"
#define  Next_Song				"A6"
#define  Volumeadd				"B0"
#define  Volumesub				"B1"

#endif  

