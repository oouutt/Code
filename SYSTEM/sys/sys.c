#include "sys.h"

u8 JM_TIME[12]="2022.12.12";
u8 UR_TIME=12;
u8 According=1;	
u8 serialportoutput=1;
u32 STM32_f10x[3]={8675938 ,  4212122  , 7583};                                                         
u8  STM32_f10x12X[12]={6,0,4,4,1,6,0,1,4,0,0,2};


//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
