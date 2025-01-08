/*
 * buttons.c
 *
 *  Created on: ٠٦‏/٠١‏/٢٠٢٥
 *      Author: lenovo
 */


#include "buttons.h"

extern void North_Pb_IRQ(void);
extern void South_Pb_IRQ(void);
extern void East_Pb_IRQ(void);
extern void West_Pb_IRQ(void);


void button_init(void)
{


	// GPIOA  CONFIG
	GPIO_CONFIG_t buttons_configuration;
	buttons_configuration.Pin_Number = North_Push_Button ;
	buttons_configuration.Pin_Mode = GPIO_MODE_alternate_function_input;
	MCAL_GPIO_Init(GPIOA, & buttons_configuration);
	buttons_configuration.Pin_Number = South_Push_Button ;
	MCAL_GPIO_Init(GPIOA, & buttons_configuration);
	buttons_configuration.Pin_Number = East_Push_Button ;
	MCAL_GPIO_Init(GPIOB, & buttons_configuration);
	buttons_configuration.Pin_Number = West_Push_Button ;
	MCAL_GPIO_Init(GPIOB, & buttons_configuration);



	//EXTI CONFIG
	EXTI_config_t Push_buttons;
	Push_buttons.interrupt_mode = rising_edge;                  //Generates interrupts when transition to high logic
	Push_buttons.pin_and_line_number = 6;
	Push_buttons.port = GPIOA;
	Push_buttons.isr_p = North_Pb_IRQ;
	MCAL_EXTI_Init(&Push_buttons);

	Push_buttons.pin_and_line_number = 7;
	Push_buttons.isr_p = South_Pb_IRQ;
	MCAL_EXTI_Init(&Push_buttons);

	Push_buttons.pin_and_line_number = 14;
	Push_buttons.port = GPIOB;
	Push_buttons.isr_p = East_Pb_IRQ;
	MCAL_EXTI_Init(&Push_buttons);

	Push_buttons.pin_and_line_number = 15;
	Push_buttons.isr_p = West_Pb_IRQ;
	MCAL_EXTI_Init(&Push_buttons);



	//Enable interrupts
	MCAL_EXTI_Enable(EXTI6);
	MCAL_EXTI_Enable(EXTI7);
	MCAL_EXTI_Enable(EXTI14);
	MCAL_EXTI_Enable(EXTI15);



}
Push_buttons_error button_check(void);
