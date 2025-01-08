/*
 * EXTI.h
 *
 *  Created on: ١٠‏/١٢‏/٢٠٢٣
 *      Author: lenovo
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f103x6.h"
#include "GPIO.h"


//------------------------------------------------------------------------------------------------------------//

                                     //configuration structure

typedef struct
{
	GPIO_t *port;                                     //determine the port on which u want to receive interrupt
	                                                  //this parameter should be set based on @ref port_define

	uint16_t pin_and_line_number;		               //determine both number of pin on which u want to receive interrupt and number of interrupt line
									                  //this parameter should be set based on @ref pin_number_define

	uint8_t interrupt_mode ;                          //determine the mode of interrupt
													  //this parameter should be set based on @ref interrupt_mode_define

    void(*isr_p)(void) ;							 //determine the address of ISR function to deal with interrupt
     												 //this parameter should be set based on your implementation and it's default is reset_handler

	}EXTI_config_t;


//----------------------------------------------------------------------------------------------------------------------------------------------------------//

	                                                  //macros configuration references

// @ref port_define

#define  PORTA						 GPIOA
#define	 PORTB 						 GPIOB
#define  PORTC						 GPIOC
#define  PORTD 						 GPIOD

// @ref pin_number_define

#define	 P0 					  0
#define  P1 					  1
#define  P2 					  2
#define  P3 					  3
#define  P4 					  4
#define  P5 					  5
#define  P6 					  6
#define  P7 					  7
#define  P8 					  8
#define  P9 					  9
#define  P10 			    	  10
#define  P11 				      11
#define  P12 				      12
#define  P13 				      13
#define  P14 				      14
#define  P15 				      15

// @ref interrupt_mode_define

#define falling_edge					 0
#define rising_edge					     1
#define logical_change   				 2                            //interrupt generated for both rising and falling edges

//@ref interrupt_number_define

#define	 EXTI0     							0
#define	 EXTI1     							1
#define	 EXTI2     							2
#define  EXTI3     							3
#define	 EXTI4     							4
#define	 EXTI5     							5
#define	 EXTI6     							6
#define	 EXTI7     							7
#define	 EXTI8     							8
#define	 EXTI9     							9
#define	 EXTI10     						10
#define	 EXTI11     						11
#define	 EXTI12     						12
#define	 EXTI13     						13
#define	 EXTI14     						14
#define	 EXTI15     						15

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
		                                      // APIs supported by " MCAL EXTI driver "



void MCAL_EXTI_Init(EXTI_config_t*);                             //routing required pin to equivalent interrupt line & assign required interrupt mode & assign required ISR
void MCAL_EXTI_DeInit(void);                                     //reset EXTI peripheral to it's initial value
void MCAL_EXTI_Update(EXTI_config_t*);                           //updates interrupt informations [required pin , mode , ISR ]
void MCAL_EXTI_Enable(uint8_t interrupt_number);                 //Enable interrupt..parameter value should be set based on @ref interrupt_number_define
void MCAL_EXTI_Disable(uint8_t interrupt_number);                //Disaple interrupt..parameter value should be set based on @ref interrupt_number_define










#endif /* EXTI_H_ */
