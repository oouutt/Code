#ifndef __SYSSET_H
#define __SYSSET_H	 

#include <sys.h>
#include "stdint.h"
#include <stdio.h>

#define KEY1_Value 1
#define KEY2_Value 2
#define KEY3_Value 3
#define KEY4_Value 4

#define SMnum	   4
#define Data_Num   6			
#define Element_MAX Data_Num-1 		

#define Single_Menus_Num  	 5	
#define Multilevel_Menus_Num  0		

extern u8 *sysmode_str[SMnum];
extern u8  Str_buff[20];
extern u16 *Data[Data_Num];
extern u16 Change[Data_Num];
extern u8  *display_str[Data_Num];
extern u16 obj;
extern u16 start;
extern u16 *Data[Data_Num];
void SYS_Set(u8 key);
void display_sysmode(u8 x,u8 y,u8 aim,u8 *srt);


#endif
