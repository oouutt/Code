#ifndef __HX711_H__ 
#define __HX711_H__ 

extern double hx711_wg;
void HX711_Init(void); //ADģ��
double Get_Maopi(void);
unsigned long HX711_Read(void);  //��ȡADֵ 
double Get_Weight(void);
#endif
