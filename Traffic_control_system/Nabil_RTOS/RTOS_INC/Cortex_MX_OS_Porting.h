/*
 * Cortex_MX_OS_Porting.h
 *
 *  Created on: ١٨‏/٠٥‏/٢٠٢٤
 *      Author: lenovo
 */

#ifndef INC_CORTEX_MX_OS_PORTING_H_
#define INC_CORTEX_MX_OS_PORTING_H_

#include "core_cm3.h"


extern int _estack;
extern int _e_heap;


//======================================================================= Macros ===========================================================

#define OS_SeT_PSP(address)		__asm volatile("MOV R0 , %0 \n\t MSR PSP , R0"\
								:                                             \
								: "r"      	(address)                         )


#define OS_get_PSP(address)		        __asm volatile("MRS R0 , PSP \n\t MOV %0 ,R0"  \
        :"=r"            (address)                             )

#define OS_SP_Shadow_MSP		        __asm volatile("MRS   R0 ,    CONTROL "         );\
		                                __asm volatile("AND   R0 , R0 , #0XFFFFFFFD "   );\
	                                	__asm volatile("MSR   CONTROL , R0    "         )

#define OS_SP_Shadow_PSP		        __asm volatile("MRS   R0 ,    CONTROL "   );\
		                                __asm volatile("ORR   R0 , R0 , #0X02 "   );\
		                                __asm volatile("MSR   CONTROL , R0    "   );\
		                                 __asm volatile("ORR R2 , R2 , #0x01"); \
		                                 __asm volatile("MSR CONTROL , R2")



#define Switch_to_previledged_access      __asm volatile("MRS R2 , CONTROL \n\t " \
								              "LSR R2 , R2 , #0x01\n\t" \
									          "LSL R2 , R2 , #0x01\n\t" \
									          "MSR CONTROL , R2")

#define Switch_to_user_access      __asm volatile("MRS R2 , CONTROL \n\t " \
								                  "ORR R2 , R2 , #0x01\n\t" \
									              "MSR CONTROL , R2")

//======================================================================= Functions Declarations ========================================================


void HW_INIT();
void OS_start_ticker();

//============================================================================================================================================

#endif /* INC_CORTEX_MX_OS_PORTING_H_ */
