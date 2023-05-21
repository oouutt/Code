#include "USART2.h"
void MP3_USART_Init(void)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);		
	
    GPIO_InitStructure.GPIO_Pin = MP3_USART_TXPIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(MP3_USART_GPIO, &GPIO_InitStructure);
   
    
    GPIO_InitStructure.GPIO_Pin = MP3_USART_RXPIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(MP3_USART_GPIO, &GPIO_InitStructure);

	//Busy 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
  
    //USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(MP3_USART, &USART_InitStructure); 
    USART_Cmd(MP3_USART, ENABLE);                   
}

//串口发送数据  
void MP3_Com(u8 com)
{	
	USART_ClearFlag(MP3_USART,USART_FLAG_TC);
	USART_SendData(MP3_USART,com);
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TC));
}

//MP3初始化
void MP3_Init(void)
{
  MP3_USART_Init();
	
  delay_ms(100);
  MP3_Com(0xAA);//选择Flash播放
  MP3_Com(0x0B);
  MP3_Com(0x01);
  MP3_Com(0x02);
  MP3_Com(0xB8);
  delay_ms(100);

  MP3_Com(0xAA);//音量30
  MP3_Com(0x13);
  MP3_Com(0x01);
  MP3_Com(0x01E);
  MP3_Com(0xDC);
  delay_ms(150);
	
  MP3_Com(0xAA);//音量30
  MP3_Com(0x13);
  MP3_Com(0x01);
  MP3_Com(0x01E);
  MP3_Com(0xDC);
  delay_ms(150);
}

u8 MP3_State(void)
{
  return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
}

//选择曲目
void MP3_choose(u32 Number)
{
  u8 check=0;
	
	check=0xB3+Number;	
	MP3_Com(0xAA);
	MP3_Com(0x07);
	MP3_Com(0x02);
	MP3_Com(0x00);
	MP3_Com(Number);	
	MP3_Com(check);
}
