/*
 * buttons.h
 *
 *  Created on: ٠٦‏/٠١‏/٢٠٢٥
 *      Author: lenovo
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include"EXTI.h"

enum directions
{
	north ,
	south ,
	east ,
	west,
	interrupt_handled        // added to indicate that interrupt request of the direction is already handled
	};
typedef enum
{
	North_pb_faulty,
	South_pb_faulty,
	East_pb_faulty,
	West_pb_faulty
	}Push_buttons_error;

// Push buttons pins ----> access single pins
#define North_Push_Button		GPIO_pin6                     //GPIOA
#define South_Push_Button		GPIO_pin7					  //GPIOA
#define East_Push_Button		GPIO_pin14                    //GPIOB
#define West_Push_Button		GPIO_pin15                    //GPIOB

// Initialize push buttons with prober configuration to map to EXTI
void button_init(void);
Push_buttons_error button_check(void);

#endif /* BUTTONS_H_ */
