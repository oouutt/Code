#ifndef __MP3_H
#define __MP3_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"

//重要：根据选择的串口需要修改串口时钟 RCC_APB1Periph_USART1
#define MP3_USART 		  USART2
#define MP3_USART_GPIO    GPIOA
#define MP3_USART_TXPIN   GPIO_Pin_2
#define MP3_USART_RXPIN   GPIO_Pin_3

void MP3_Init(void);
void MP3_Star(u32 Number);
u8 MP3_State(void);

#endif  

