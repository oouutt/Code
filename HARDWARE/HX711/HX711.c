#include "stm32f10x.h" 
#include "hx711.h" 
#include "usart.h"	

double hx711_wg;
#define ADIO  GPIOC        //ad口  
#define DATA  GPIO_Pin_15	  //数据口 
#define CLK   GPIO_Pin_14	  //时钟 
#define ADCLK RCC_APB2Periph_GPIOC   //生命ad时钟挂在APB2P.C上

 double hx711_buffer=0;//定义hx711_buffer来接受hx711read读取的信息
 double	weight_maopi=0;//毛皮重量
 double weight_shiwu=0;//实物重量
 u32 am;//毛皮重量整数部分
 double a;//处理毛皮重量等同于毛皮重量（可有可无）
 double b;//毛皮重量小数部分
 //const u8 TEXT_Buffer[]={"WarShipSTM32 IIC TEST"};//可有可无
#define SIZE sizeof(TEXT_Buffer)	//只是为了得到SIZE
void HX711_Init(void) //AD初始化
{ 
	GPIO_InitTypeDef GPIO_InitStructure; //结构体

	RCC_APB2PeriphClockCmd(ADCLK,ENABLE); //使能时钟口

	GPIO_InitStructure.GPIO_Pin =   CLK; //PC10为时钟口
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; //推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructure); //PC.10时钟口

	GPIO_InitStructure.GPIO_Pin = DATA; //PC11为数据口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(ADIO,&GPIO_InitStructure);	//PC.11数据口
	
	Get_Maopi();
} 

unsigned long HX711_Read(void)  //读取711读到的数据
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
	  weight_maopi=weight_maopi/7.26;//参数可换，不同的称重模块对应不同的参数，需自己标定
	  //a=weight_maopi;
	  //am=(u32)weight_maopi;
	  //b=a-am;//处理毛皮重量，分整数小数分别存入24C02的两个地址位。
//	  if(am-12120<20) //并不是简单地掉电存储，而是通过获取毛皮的数据得到的am在12120左右，这个可以靠
//		{             //每个人的毛皮重量进行设定，取巧设计，不是掉电存储，是每次都会存储。因为高精度的6kg称重模块会受外界影响，其数据是动态的
//			AT24CXX_Write(0,(u8*)&am,SIZE);//存整数到24C02的0地址
//			AT24CXX_Write(5,(u8*)&b,SIZE);//存小数到24C02的5地址
//			weight_maopi=am+b;//再将整数与小数相加赋值给毛皮
//		}
//         else
//		{
//		    AT24CXX_Read(0,(u8*)&am,SIZE);//读整数从24C02的0地址
//			AT24CXX_Read(5,(u8*)&b,SIZE);//读小数数从24C02的0地址
//		    weight_maopi=am+b;//再将整数与小数相加赋值给毛皮
//		}
     return a;	//返回值	
  }
double Get_Weight(void)
{
	double m;
	hx711_buffer=HX711_Read();
	hx711_buffer=hx711_buffer/100;
	hx711_buffer=hx711_buffer/7.26;//处理实物重量
	//printf("%f\r\n",hx711_buffer);
	if(hx711_buffer>weight_maopi)
	{
		weight_shiwu=hx711_buffer;
		weight_shiwu=weight_shiwu-weight_maopi;
	}
	m=weight_shiwu;
	return m;
}


