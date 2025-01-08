/*
 * EXTI.c
 *
 *  Created on: 11/12/2023
 *      Author: Ahmed
 */


#include "EXTI.h"


//----------------------------------------------------------------------------------------------------------------------------------------------------------//
		                                      // global variables & Auxiliary functions "

void(*arr_of_ptF[16])(void);
uint8_t routing_value(GPIO_t*x);


/**==========================================================================================================================================================*
 * @Fn			-MCAL_EXTI_Init
 * @brief 		-routing required pin to equivalent interrupt line & assign required interrupt mode & assign required ISR
 * @param[in] 	-EXTI_config_t*
 * @retval		-none
 * Note			-Each pin number will be the same number of interrupt line for which all configurations will be assigned
 */

void MCAL_EXTI_Init(EXTI_config_t*x)

{
	//modify the required pin on which interrupt will occur to be floating input

	GPIO_CONFIG_t ir_config;
	ir_config.Pin_Number = 1<<x->pin_and_line_number;
	ir_config.Pin_Mode = GPIO_MODE_Floating_input;
    MCAL_GPIO_Init(x->port,&ir_config);

    //routing the required pin to the equivalent EXTI line by access AFIO registers

    uint8_t offset,shift_value;
    offset = x->pin_and_line_number / 4;
    volatile uint32_t * Ir_routing_pt = &(AFIO->EXTICR1) + offset;                   //mapping pin to proper register according to interrupt line
    shift_value = ( x->pin_and_line_number % 4 ) * 4;   	                         //each line is configured by 4 pin to be routed to the proper port

  /*
 Bits 15:0 -> EXTICRy EXTIx[15:0]: EXTI x configuration (x= 0 to 15)
These bits are written by software to select the source input for EXTIx external interrupt.
Refer to Section 10.2.5: External interrupt/event line mapping
0000: PA[x] pin
0001: PB[x] pin
0010: PC[x] pin
0011: PD[x] pin
0100: PE[x] pin
0101: PF[x] pin
0110: PG[x] pin

note: each register from EXTICR(1:4) is responsible for routing 4 interrupt lines from EXTI0 : EXTI15

*/

    *Ir_routing_pt &= ~(0b1111<<shift_value);                                   //set configuration bits to 0000 before assign required value
	*Ir_routing_pt |= (routing_value(x->port)<<shift_value);                    //assign the proper value according to the required port
	arr_of_ptF[x->pin_and_line_number] = x->isr_p;

//assign values of EXTI registers to achieve required mode

	switch (x->interrupt_mode)
	{
	case rising_edge :
	EXTI->RTSR |= 1<<(x->pin_and_line_number) ;
	EXTI->FTSR 	&= ~1<<(x->pin_and_line_number) ;

	break;
	case falling_edge :
	EXTI->FTSR 	|= 1<<(x->pin_and_line_number) ;
	EXTI->RTSR &= ~1<<(x->pin_and_line_number) ;

	break ;
	case logical_change :
	EXTI->RTSR |= 1<<(x->pin_and_line_number) ;
	EXTI->FTSR 	|= 1<<(x->pin_and_line_number) ;
	break;
	}


}

/**==========================================================================================================================================================*
 * @Fn			-MCAL_EXTI_DeInit
 * @brief 		-Reset EXTI Peripheral registers to their initial values
 * @param[in] 	-none
 * @retval		-none
 * Note			-EXTI peripheral doesn't reset by RCC so it will be reset manually according to TRM reset values
 */

void MCAL_EXTI_DeInit(void)

{
    EXTI->IMR   =  0x00000000;
    EXTI->EMR   =  0x00000000;
    EXTI->RTSR  =  0x00000000;
    EXTI->FTSR  =  0x00000000;
    EXTI->SWIER =  0x00000000;
    EXTI->PR    =  0x0000FFFF;              //write 1 to clear pending request if exist

	}

/**==========================================================================================================================================================*
 * @Fn			-MCAL_EXTI_Update
 * @brief 		-update interrupt status according to routing pin , interrupt mode , & ISR if updated
 * @param[in] 	-EXTI_config_t*x
 * @retval		-none
 * Note			-interrupt configurations should be updated first before calling the function
 */

void MCAL_EXTI_Update(EXTI_config_t*x)

{
	MCAL_EXTI_Init(x);

	}

/**==========================================================================================================================================================*
 * @Fn			-MCAL_EXTI_Enable
 * @brief 		-Enable interrupt for certain pin or interrupt line
 * @param[in] 	-uint8_t interrupt_number
 * @retval		-none
 * Note			-
 *
 *
 */

void MCAL_EXTI_Enable(uint8_t interrupt_number)
{
	switch(interrupt_number)
	{
	case 0 :
	EXTI->IMR |= (1<<interrupt_number) ;                         //Enable interrupt request line at EXTI peripheral
    NVIC_EXTI0_ENABLE ;                                          //Enable interrupt request at NVIC
    break;
	case 1 :
	EXTI->IMR |= (1<<interrupt_number) ;                         //Enable interrupt request line at EXTI peripheral
    NVIC_EXTI1_ENABLE ;                                          //Enable interrupt request at NVIC
    break;
	case 2 :
	EXTI->IMR |= (1<<interrupt_number) ;                         //Enable interrupt request line at EXTI peripheral
    NVIC_EXTI2_ENABLE ;                                          //Enable interrupt request at NVIC
    break;
	case 3 :
	EXTI->IMR |= (1<<interrupt_number) ;                         //Enable interrupt request line at EXTI peripheral
    NVIC_EXTI3_ENABLE ;                                          //Enable interrupt request at NVIC
    break;
	case 4 :
	EXTI->IMR |= (1<<interrupt_number) ;                         //Enable interrupt request line at EXTI peripheral
    NVIC_EXTI4_ENABLE ;                                          //Enable interrupt request at NVIC
    break;
	case 5 :
	case 6 :
	case 7 :
	case 8 :
	case 9 :
	EXTI->IMR |= (1<<interrupt_number) ;                          //Enable interrupt request line at EXTI peripheral
	NVIC_EXTI5_9_ENABLE ;                                         //Enable interrupt request at NVIC
	break;
	case 10 :
	case 11 :
	case 12 :
	case 13 :
	case 14 :
	case 15 :
	EXTI->IMR |= (1<<interrupt_number) ;                          //Enable interrupt request line at EXTI peripheral
	NVIC_EXTI10_15_ENABLE ;                                       //Enable interrupt request at NVIC
	break;
	}

	}


/**==========================================================================================================================================================*
 * @Fn			-MCAL_EXTI_Disable
 * @brief 		-Disable interrupt for certain pin or interrupt line
 * @param[in] 	-uint8_t interrupt_number
 * @retval		-none
 * Note			-
 *
 *
 */

void MCAL_EXTI_Disable(uint8_t interrupt_number)

{
	switch(interrupt_number)
	{
	case 0 :
	EXTI->IMR &= ~(1<<interrupt_number) ;                         //DISABLE interrupt request line at EXTI peripheral
    NVIC_EXTI0_DISABLE ;                                          //DISABLE interrupt request at NVIC
    break;
	case 1 :
	EXTI->IMR &= ~(1<<interrupt_number) ;                         //DISABLE interrupt request line at EXTI peripheral
    NVIC_EXTI1_DISABLE ;                                          //DISABLE interrupt request at NVIC
    break;
	case 2 :
	EXTI->IMR &= ~(1<<interrupt_number) ;                         //DISABLE interrupt request line at EXTI peripheral
    NVIC_EXTI2_DISABLE ;                                          //DISABLE interrupt request at NVIC
    break;
	case 3 :
	EXTI->IMR &= ~(1<<interrupt_number) ;                         //DISABLE interrupt request line at EXTI peripheral
    NVIC_EXTI3_DISABLE ;                                          //DISABLE interrupt request at NVIC
    break;
	case 4 :
	EXTI->IMR &= ~(1<<interrupt_number) ;                         //DISABLE interrupt request line at EXTI peripheral
    NVIC_EXTI4_DISABLE ;                                          //DISABLE interrupt request at NVIC
    break;
	case 5 :
	case 6 :
	case 7 :
	case 8 :
	case 9 :
	EXTI->IMR &= ~(1<<interrupt_number) ;                          //DISABLE interrupt request line at EXTI peripheral
	NVIC_EXTI5_9_DISABLE ;                                         //DISABLE interrupt request at NVIC
	break;
	case 10 :
	case 11 :
	case 12 :
	case 13 :
	case 14 :
	case 15 :
	EXTI->IMR &= ~(1<<interrupt_number) ;                          //DISABLE interrupt request line at EXTI peripheral
	NVIC_EXTI10_15_DISABLE ;                                       //DISABLE interrupt request at NVIC
	break;
	}

}




//function return the proper value of 4 bits configured by EXTIC registers of AFIO according to the required port
uint8_t routing_value(GPIO_t*x)
{
	if( x==GPIOA )
		return 0b0000;
	else if( x== GPIOB)
		return 0b0001;
	else if( x== GPIOC)
		return 0b0010;
	else if( x== GPIOD)
		return 0b0011;
	else
		return 0b000;
}

/**=========================================================================================================================================================*/
                                                      //INTERRUPT SERVICE ROUTINE


void EXTI0_IRQHandler(void)
{
	arr_of_ptF[0]();                                                           //call function
    EXTI->PR |= (1<<0);                                                        //clear pending status in EXTI
    NVIC_ICPR0 |= (1<<EXTI0_IRQ);                                              //clear pending status in NVIC
}

void EXTI1_IRQHandler(void)
{
	arr_of_ptF[1]();
    EXTI->PR |= (1<<1);
    NVIC_ICPR0 |= (1<<EXTI1_IRQ);
}
void EXTI2_IRQHandler(void)
{
	arr_of_ptF[2]();
    EXTI->PR |= (1<<2);
    NVIC_ICPR0 |= (1<<EXTI2_IRQ);

}
void EXTI3_IRQHandler(void)
{
	arr_of_ptF[3]();
    EXTI->PR |= (1<<3);
    NVIC_ICPR0 |= (1<<EXTI3_IRQ);

}
void EXTI4_IRQHandler(void)
{
	arr_of_ptF[4]();
    EXTI->PR |= (1<<4);
    NVIC_ICPR0 |= (1<<EXTI1_IRQ);

}

void EXTI9_5_IRQHandler(void)
{
     if ( EXTI->PR & (1<<5) )    		 { EXTI->PR |= (1<<5);   	 arr_of_ptF[5](); }
     if ( EXTI->PR & (1<<6) ) 			 { EXTI->PR |= (1<<6);   	 arr_of_ptF[6](); }
     if ( EXTI->PR & (1<<7) ) 			 { EXTI->PR |= (1<<7);   	 arr_of_ptF[7](); }
     if ( EXTI->PR & (1<<8) ) 			 { EXTI->PR |= (1<<8);   	 arr_of_ptF[8](); }
     if ( EXTI->PR & (1<<9) ) 		   	 { EXTI->PR |= (1<<9);   	 arr_of_ptF[9](); }

    NVIC_ICPR0 |= (1<<EXTI5_9_IRQ);

	}


void EXTI15_10_IRQHandler(void)
{
    if ( EXTI->PR & (1<<10) )    		 { EXTI->PR |= (1<<10);   	 arr_of_ptF[10](); }
    if ( EXTI->PR & (1<<11) ) 			 { EXTI->PR |= (1<<11);   	 arr_of_ptF[11](); }
    if ( EXTI->PR & (1<<12) ) 			 { EXTI->PR |= (1<<12);   	 arr_of_ptF[12](); }
    if ( EXTI->PR & (1<<13) ) 			 { EXTI->PR |= (1<<13);   	 arr_of_ptF[13](); }
    if ( EXTI->PR & (1<<14) ) 		   	 { EXTI->PR |= (1<<14);   	 arr_of_ptF[14](); }
    if ( EXTI->PR & (1<<15) ) 		   	 { EXTI->PR |= (1<<15);   	 arr_of_ptF[15](); }

   NVIC_ICPR1 |= (1<<(EXTI10_15_IRQ - 32));

	}
















