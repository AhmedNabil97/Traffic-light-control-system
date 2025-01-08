/*
 * GPIO.h
 *
 *  Created on: Dec 4, 2023
 *      Author: lenovo
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f103x6.h"

//------------------------------------------------------------------------------------------------------------//

                                     //configuration structure
typedef struct
{
	uint16_t Pin_Number;          		     // specify the number of GPIO pin to be configued
	                             	    // this parameter should be set based on @ref GPIO_PINS_define

	uint8_t Pin_Mode;            	     	// specify the mode of operation of the selected pin
										// this parameter should be set based on @ref GPIO_MODE_define

	uint8_t Output_Pin_Speed;			    // specify the speed of the output selected pin
	                                    // this parameter should be set based on @ref GPIO_SPEED_define
	}GPIO_CONFIG_t;

//------------------------------------------------------------------------------------------------------------//

                                     //macros configuration references
// @ref GPIO_PINS_define

#define GPIO_pin0 						0b0000000000000001U
#define GPIO_pin1 						0b0000000000000010U
#define GPIO_pin2 						0b0000000000000100U
#define GPIO_pin3 						0b0000000000001000U
#define GPIO_pin4 						0b0000000000010000U
#define GPIO_pin5 						0b0000000000100000U
#define GPIO_pin6 						0b0000000001000000U
#define GPIO_pin7 						0b0000000010000000U
#define GPIO_pin8 						0b0000000100000000U
#define GPIO_pin9 						0b0000001000000000U
#define GPIO_pin10 						0b0000010000000000U
#define GPIO_pin11 						0b0000100000000000U
#define GPIO_pin12 						0b0001000000000000U
#define GPIO_pin13 						0b0010000000000000U
#define GPIO_pin14 						0b0100000000000000U
#define GPIO_pin15 						0b1000000000000000U
#define GPIO_port 						0b0000000000000011U           //use this if u want the whole port to have the same configuration

// @ref GPIO_MODE_define
#define GPIO_MODE_Analog_mode 										0b0000
#define GPIO_MODE_Floating_input                				  	0b0100	        // (reset state)
#define GPIO_MODE_Input_with_pull_up								0b1000
#define GPIO_MODE_Input_with_pull_down								0b1001
#define GPIO_MODE_alternate_function_input 							0b0100
#define GPIO_MODE_General_purpose_output_push_pull					0b0010           //Cnf value
#define GPIO_MODE_General_purpose_output_Open_drain					0b0110           //Cnf value
#define GPIO_MODE_Alternate_function_output_push_pull				0b1010           //Cnf value
#define GPIO_MODE_Alternate_function_output_Open_drain				0b1110           //Cnf value

// @ref GPIO_SPEED_define
#define low_speed_2Mhz 												0b10
#define medium_speed_10Mhz 											0b01
#define high_speed_50Mhz											0b11

// @ref GPIO_Lock_Status_define
#define pin_locked       											1
#define locked_error       										    0
#define port_locked       											1
#define locked_error       											0

//------------------------------------------------------------------------------------------------------------//
	                                      // APIs supported by " MCAL GPIO driver "


void MCAL_GPIO_Init(GPIO_t*,GPIO_CONFIG_t*); 					       					 //modify the configuration of GPIO pins or whole port
void MCAL_GPIO_DeInit(GPIO_t*);                   					  					 //reset GPIO registers to it's intial value
uint8_t MCAL_GPIO_Read_Pin(GPIO_t*,uint16_t pin_number);              				     //read the value of specifc pin
uint16_t MCAL_GPIO_Read_Port(GPIO_t*);            									     //read the value of the whole port
void MCAL_GPIO_Write_Pin(GPIO_t*x,uint16_t pin_number,uint8_t pin_value);                 //write the value of specifc pin
void MCAL_GPIO_Write_Port(GPIO_t*x,uint16_t port_value);            				     //write the value of the whole port
void MCAL_GPIO_Toggle_Pin(GPIO_t*x,uint16_t pin_number);                                  //Toggle the value of specifc pin
void MCAL_GPIO_Toggle_Port(GPIO_t*x);            					                     //Toggle the value of the whole port
uint8_t MCAL_GPIO_lock_Pin(GPIO_t*x,uint8_t pin_number);            					 //lock configuration of specific pin
uint8_t MCAL_GPIO_lock_Port(GPIO_t*x);            	                     				 //lock configuration of specific port






#endif /* GPIO_H_ */
