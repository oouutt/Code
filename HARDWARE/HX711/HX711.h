#ifndef __HX711_H__ 
#define __HX711_H__ 

extern double hx711_wg;
void HX711_Init(void); //ADÄ£¿é
double Get_Maopi(void);
unsigned long HX711_Read(void);  //¶ÁÈ¡ADÖµ 
double Get_Weight(void);
#endif
