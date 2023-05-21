#include "MP3.h"

void MP3_USART_Init(void)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);		
	
    GPIO_InitStructure.GPIO_Pin = MP3_USART_TXPIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(MP3_USART_GPIO, &GPIO_InitStructure);
   
    
    GPIO_InitStructure.GPIO_Pin = MP3_USART_RXPIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(MP3_USART_GPIO, &GPIO_InitStructure);

	//Busy 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
  
    //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(MP3_USART, &USART_InitStructure); 
    USART_Cmd(MP3_USART, ENABLE);                   
}

//���ڷ�������  
void MP3_Com(u8 com)
{	
	USART_ClearFlag(MP3_USART,USART_FLAG_TC);
	USART_SendData(MP3_USART,com);
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TC));
}

//MP3��ʼ��
void MP3_Init(void)
{
	MP3_USART_Init();
	
  delay_ms(1000);
  MP3_Com(0xAA);//ѡ��Flash����
  MP3_Com(0x0B);
  MP3_Com(0x01);
  MP3_Com(0x02);
  MP3_Com(0xB8);
  delay_ms(1000);

  MP3_Com(0xAA);//����30
  MP3_Com(0x13);
  MP3_Com(0x01);
  MP3_Com(0x01E);
  MP3_Com(0xDC);
  delay_ms(1500);
}

u8 MP3_State(void)
{
  return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
}

//ѡ����Ŀ
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
