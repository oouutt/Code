#include "JR6001.h"
#include "usart.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 

void JR6001_Volumecontrol(u8 num);

void JR6001_Init(void)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB1PeriphClockCmd(JR6001_USART_CLK, ENABLE);	//使能USART
	RCC_APB2PeriphClockCmd(JR6001_USART_GPIO_CLK, ENABLE);		
  
	//USART_TX 引脚配置  
    GPIO_InitStructure.GPIO_Pin = JR6001_TX_GPIO_PIN; 
    GPIO_InitStructure.GPIO_Speed = JR6001_TX_GPIO_Speed;
    GPIO_InitStructure.GPIO_Mode = JR6001_TX_GPIO_Mode;	//复用推挽输出
    GPIO_Init(JR6001_TX_GPIO_PORT, &GPIO_InitStructure);//初始化
   
    //USART_RX	引脚配置  
    GPIO_InitStructure.GPIO_Pin = JR6001_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = JR6001_RX_GPIO_Mode;//浮空输入
    GPIO_Init(JR6001_RX_GPIO_PORT, &GPIO_InitStructure);//初始化
  
    //USART 初始化设置
	USART_InitStructure.USART_BaudRate = JR6001_USART_Baud;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(JR6001_USART, &USART_InitStructure); //初始化串口
    USART_Cmd(JR6001_USART, ENABLE);                //使能串口  

	#ifdef JR6001_BUSY //查忙引脚配置
	
	RCC_APB2PeriphClockCmd(JR6001_BUSY_GPIO_CLK,ENABLE);//使能POR时钟
	GPIO_InitStructure.GPIO_Pin  = JR6001_BUSY_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = JR6001_BUSY_GPIO_Mode; 
 	GPIO_Init(JR6001_BUSY_GPIO_PORT, &GPIO_InitStructure);
	
	#endif	
	
	JR6001_Volumecontrol(31);//设置音量
	
	JR6001_ms(100);
}

//串口发送数据 
void JR6001_SendCode(u8 *str,u8 busy)
{
	u16 len,i;
	len = strlen(str);
	if(busy == 1 && JR6001_BUSY_IO == 0){
		for(i=0;i<=len;i++)							//循环发送数据
		{ 
			while(USART_GetFlagStatus(JR6001_USART,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕  
			USART_SendData(JR6001_USART,str[i]); 
		}
	}else if(busy == 0){
		for(i=0;i<=len;i++)							//循环发送数据
		{ 
			while(USART_GetFlagStatus(JR6001_USART,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕  
			USART_SendData(JR6001_USART,str[i]); 
		}
	}
	JR6001_ms(10);
}

void JR6001_Instruction(u8 *str,u8 busy)//基础指令发送
{
	u8 JR6001_Buff[20];
	sprintf(JR6001_Buff,"%s\r\n",str);
	JR6001_SendCode(JR6001_Buff,busy);
}	

void JR6001_SongControl(u8 num,u8 busy)//曲目选择
{
	u8 JR6001_Buff[20];
	if(num<10){
		sprintf(JR6001_Buff,"A7:0000%d\r\n",num);
		JR6001_SendCode(JR6001_Buff,busy);
	}else if(num<100){
		sprintf(JR6001_Buff,"A7:000%d\r\n",num);
		JR6001_SendCode(JR6001_Buff,busy);
	}else{
		sprintf(JR6001_Buff,"A7:00%d\r\n",num);
		JR6001_SendCode(JR6001_Buff,busy);
	}
}

void JR6001_Volumecontrol(u8 num)//音量选择
{
	u8 JR6001_Buff[20];
	sprintf(JR6001_Buff,"AF:%d\r\n",num);
	JR6001_SendCode(JR6001_Buff,0);
}

/*
	JR6001_Init();
	JR6001_SongControl(5,0);

*/
