#include "stm32f10x.h" 
#include "hx711.h" 
#include "usart.h"	

double hx711_wg;
#define ADIO  GPIOC        //ad��  
#define DATA  GPIO_Pin_15	  //���ݿ� 
#define CLK   GPIO_Pin_14	  //ʱ�� 
#define ADCLK RCC_APB2Periph_GPIOC   //����adʱ�ӹ���APB2P.C��

 double hx711_buffer=0;//����hx711_buffer������hx711read��ȡ����Ϣ
 double	weight_maopi=0;//ëƤ����
 double weight_shiwu=0;//ʵ������
 u32 am;//ëƤ������������
 double a;//����ëƤ������ͬ��ëƤ���������п��ޣ�
 double b;//ëƤ����С������
 //const u8 TEXT_Buffer[]={"WarShipSTM32 IIC TEST"};//���п���
#define SIZE sizeof(TEXT_Buffer)	//ֻ��Ϊ�˵õ�SIZE
void HX711_Init(void) //AD��ʼ��
{ 
	GPIO_InitTypeDef GPIO_InitStructure; //�ṹ��

	RCC_APB2PeriphClockCmd(ADCLK,ENABLE); //ʹ��ʱ�ӿ�

	GPIO_InitStructure.GPIO_Pin =   CLK; //PC10Ϊʱ�ӿ�
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; //������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructure); //PC.10ʱ�ӿ�

	GPIO_InitStructure.GPIO_Pin = DATA; //PC11Ϊ���ݿ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructure);	//PC.11���ݿ�
	
	Get_Maopi();
} 

unsigned long HX711_Read(void)  //��ȡ711����������
{ 
	unsigned long val = 0; 
	unsigned char i = 0; 
	float p;
	GPIO_SetBits(ADIO,DATA); 
	GPIO_ResetBits(ADIO,CLK); 
	while(GPIO_ReadInputDataBit(ADIO,DATA)); 
	delay_us(1); 
	for(i=0;i<24;i++) 
	{ 
		GPIO_SetBits(ADIO,CLK); 
		val=val<<1; 
		delay_us(1);  
		GPIO_ResetBits(ADIO,CLK); 
		if(GPIO_ReadInputDataBit(ADIO,DATA)) 
		val++; 
		delay_us(1); 
	} 
	GPIO_SetBits(ADIO,CLK); 
	val = val^0x800000; 
	delay_us(1); 
	GPIO_ResetBits(ADIO,CLK); 
	delay_us(1); 
	p=val;
	return p; 	
}
double Get_Maopi(void)
  {
	  hx711_buffer=HX711_Read();
	  weight_maopi=hx711_buffer/100;
	  weight_maopi=weight_maopi/7.26;//�����ɻ�����ͬ�ĳ���ģ���Ӧ��ͬ�Ĳ��������Լ��궨
	  //a=weight_maopi;
	  //am=(u32)weight_maopi;
	  //b=a-am;//����ëƤ������������С���ֱ����24C02��������ַλ��
//	  if(am-12120<20) //�����Ǽ򵥵ص���洢������ͨ����ȡëƤ�����ݵõ���am��12120���ң�������Կ�
//		{             //ÿ���˵�ëƤ���������趨��ȡ����ƣ����ǵ���洢����ÿ�ζ���洢����Ϊ�߾��ȵ�6kg����ģ��������Ӱ�죬�������Ƕ�̬��
//			AT24CXX_Write(0,(u8*)&am,SIZE);//��������24C02��0��ַ
//			AT24CXX_Write(5,(u8*)&b,SIZE);//��С����24C02��5��ַ
//			weight_maopi=am+b;//�ٽ�������С����Ӹ�ֵ��ëƤ
//		}
//         else
//		{
//		    AT24CXX_Read(0,(u8*)&am,SIZE);//��������24C02��0��ַ
//			AT24CXX_Read(5,(u8*)&b,SIZE);//��С������24C02��0��ַ
//		    weight_maopi=am+b;//�ٽ�������С����Ӹ�ֵ��ëƤ
//		}
     return a;	//����ֵ	
  }
double Get_Weight(void)
{
	double m;
	hx711_buffer=HX711_Read();
	hx711_buffer=hx711_buffer/100;
	hx711_buffer=hx711_buffer/7.26;//����ʵ������
	//printf("%f\r\n",hx711_buffer);
	if(hx711_buffer>weight_maopi)
	{
		weight_shiwu=hx711_buffer;
		weight_shiwu=weight_shiwu-weight_maopi;
	}
	m=weight_shiwu;
	return m;
}


