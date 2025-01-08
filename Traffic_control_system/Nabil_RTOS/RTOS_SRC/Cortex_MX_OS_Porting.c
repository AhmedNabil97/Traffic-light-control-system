/*
 * Cortex_MX_OS_Porting.c
 *
 *
 */


#include "Cortex_MX_OS_Porting.h"

void HardFault_Handler(void)
{
	while(1);
}
void	MemManage_Handler(void)
{
	while(1);

}

void	BusFault_Handler(void)
{
	while(1);

}
void	UsageFault_Handler(void)
{
	while(1);

}
__attribute__((naked))   void SVC_Handler()
{

	__asm("TST LR , 0x4 ");
	__asm("ITE EQ");
	__asm("MRSEQ R0 , MSP");
	__asm("MRSNE R0 , PSP");
	__asm("b service_handler");   //higher piriority than pendsvtherefore isr will not get executed till svc isr finish

}



void HW_INIT()
{
	//Initialize clock tree & HW Resources
	// recommend freq.  8 MHZ.
	// 1 TICK = 125 NanoSec
	// X COUNT = 1 MS.
	// X = 8000 COUNTS
	// We will configure our timer to generate interrupt every 1 MS to apply our scheduler algorithm.
	//make pendsv piriority = systick to ensure avoid conflecting
	NVIC_SetPriority(PendSV_IRQn , 15);

	}
void OS_start_ticker()
{
	SysTick_Config(16000);        //no of ticks between each 2 interrupts ------------> 32000 = 4ms.
	}


