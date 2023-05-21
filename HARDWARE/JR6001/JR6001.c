#include "JR6001.h"
#include "usart.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 

void JR6001_Volumecontrol(u8 num);

void JR6001_Init(void)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB1PeriphClockCmd(JR6001_USART_CLK, ENABLE);	//ʹ��USART
	RCC_APB2PeriphClockCmd(JR6001_USART_GPIO_CLK, ENABLE);		
  
	//USART_TX ��������  
    GPIO_InitStructure.GPIO_Pin = JR6001_TX_GPIO_PIN; 
    GPIO_InitStructure.GPIO_Speed = JR6001_TX_GPIO_Speed;
    GPIO_InitStructure.GPIO_Mode = JR6001_TX_GPIO_Mode;	//�����������
    GPIO_Init(JR6001_TX_GPIO_PORT, &GPIO_InitStructure);//��ʼ��
   
    //USART_RX	��������  
    GPIO_InitStructure.GPIO_Pin = JR6001_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = JR6001_RX_GPIO_Mode;//��������
    GPIO_Init(JR6001_RX_GPIO_PORT, &GPIO_InitStructure);//��ʼ��
  
    //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = JR6001_USART_Baud;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(JR6001_USART, &USART_InitStructure); //��ʼ������
    USART_Cmd(JR6001_USART, ENABLE);                //ʹ�ܴ���  

	#ifdef JR6001_BUSY //��æ��������
	
	RCC_APB2PeriphClockCmd(JR6001_BUSY_GPIO_CLK,ENABLE);//ʹ��PORʱ��
	GPIO_InitStructure.GPIO_Pin  = JR6001_BUSY_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = JR6001_BUSY_GPIO_Mode; 
 	GPIO_Init(JR6001_BUSY_GPIO_PORT, &GPIO_InitStructure);
	
	#endif	
	
	JR6001_Volumecontrol(31);//��������
	
	JR6001_ms(100);
}

//���ڷ������� 
void JR6001_SendCode(u8 *str,u8 busy)
{
	u16 len,i;
	len = strlen(str);
	if(busy == 1 && JR6001_BUSY_IO == 0){
		for(i=0;i<=len;i++)							//ѭ����������
		{ 
			while(USART_GetFlagStatus(JR6001_USART,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������  
			USART_SendData(JR6001_USART,str[i]); 
		}
	}else if(busy == 0){
		for(i=0;i<=len;i++)							//ѭ����������
		{ 
			while(USART_GetFlagStatus(JR6001_USART,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������  
			USART_SendData(JR6001_USART,str[i]); 
		}
	}
	JR6001_ms(10);
}

void JR6001_Instruction(u8 *str,u8 busy)//����ָ���
{
	u8 JR6001_Buff[20];
	sprintf(JR6001_Buff,"%s\r\n",str);
	JR6001_SendCode(JR6001_Buff,busy);
}	

void JR6001_SongControl(u8 num,u8 busy)//��Ŀѡ��
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

void JR6001_Volumecontrol(u8 num)//����ѡ��
{
	u8 JR6001_Buff[20];
	sprintf(JR6001_Buff,"AF:%d\r\n",num);
	JR6001_SendCode(JR6001_Buff,0);
}

/*
	JR6001_Init();
	JR6001_SongControl(5,0);

*/
