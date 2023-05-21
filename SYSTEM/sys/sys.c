#include "sys.h"

u8 JM_TIME[12]="2022.12.12";
u8 UR_TIME=12;
u8 According=1;	
u8 serialportoutput=1;
u32 STM32_f10x[3]={8675938 ,  4212122  , 7583};                                                         
u8  STM32_f10x12X[12]={6,0,4,4,1,6,0,1,4,0,0,2};


//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
