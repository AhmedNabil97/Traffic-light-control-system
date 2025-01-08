/*
 * GPIO.C
 *
 *  Created on: Dec 1, 2023
 *      Author: ahmed nabil
 */

#include "GPIO.h"


//function to return the correct shift value for the equivalent pin needed to be configured
uint8_t get_position(uint16_t pin_number)
{
	switch(pin_number)
	{
	case GPIO_pin0 :
		return 0;
	case GPIO_pin1 :
		return 4;
	case GPIO_pin2 :
		return 8;
	case GPIO_pin3 :
		return 12;
	case GPIO_pin4 :
		return 16;
	case GPIO_pin5 :
		return 20;
	case GPIO_pin6 :
		return 24;
	case GPIO_pin7 :
		return 28;
	case GPIO_pin8 :
		return 0;
	case GPIO_pin9 :
		return 4;
	case GPIO_pin10 :
		return 8;
	case GPIO_pin11 :
		return 12;
	case GPIO_pin12 :
		return 16;
	case GPIO_pin13 :
		return 20;
	case GPIO_pin14 :
		return 24;
	case GPIO_pin15 :
		return 28;
	default :
		return 0;
	}
}

//each pin is configured by 4 pins this function sets the 4 pins to zeros before adjusting the
//proper configuration value for the required mode of the equivalen pin
void reset_mode_cfn(uint32_t*p,uint8_t position)
{
	*p &= ~(0b1111<<position);
}

/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_init
 * @brief 		-routing required pin to equivalent interrupt line & assign required interrupt mode & assign required ISR
 * @param[in] 	-GPIO_t*
 * @param[in] 	-GPIO_CONFIG_t*
 * @retval		-none
 * Note			-
 */

void MCAL_GPIO_Init(GPIO_t *x, GPIO_CONFIG_t *y)
{
	uint8_t config_value;
	// Check if pin is configured as an output
	if (y->Pin_Mode == GPIO_MODE_General_purpose_output_push_pull ||
			y->Pin_Mode == GPIO_MODE_General_purpose_output_Open_drain ||
			y->Pin_Mode == GPIO_MODE_Alternate_function_output_push_pull ||
			y->Pin_Mode == GPIO_MODE_Alternate_function_output_Open_drain)
	{

		// If output, combine Pin_Mode and Output_Pin_Speed
		config_value = y->Pin_Mode ;
		config_value -=2 ;
		config_value += y->Output_Pin_Speed;

	}
	else
	{
		// If not output, use only Pin_Mode
		config_value = y->Pin_Mode;
	}


	if (y->Pin_Mode == GPIO_MODE_Input_with_pull_down)
	{
		//decrement the configuration value which was incremented by 1 in header file to differentiate from in_pullup_mode
		config_value--;
	}

	// Check if configuration is for a single pin or the whole port
	if (y->Pin_Number != GPIO_port)
	{
		// Configure a single pin

		// Check if pin is configured by CRL or CRH
		uint32_t *config_pointer = NULL;
		config_pointer = (y->Pin_Number > (1 << 7)) ? &(x->CRH) : &(x->CRL);

		// Determine the proper shift value based on pin number
		uint8_t shift_value = get_position(y->Pin_Number);

		// Reset mode & cnf bits for the specified pin
		reset_mode_cfn(config_pointer, shift_value);

		// Set the mode and cnf bits for the specified pin
		*config_pointer |= (config_value << shift_value);

		// Activate pull-up resistor if configured
		if (y->Pin_Mode == GPIO_MODE_Input_with_pull_up)
		{
			x->ODR |= y->Pin_Number;
		}
	}
	else
	{
		// Configure the whole port

		unsigned char i, shift = 0;

		// Iterate through pins of the port
		for (i = 0; i < 16; i++)
		{
			if (i <= 7)
			{
				// For pins 0-7, configure using CRL
				reset_mode_cfn(&x->CRL, shift);
				x->CRL |= (config_value << shift);

				// Activate pull-up resistor if configured
				if (y->Pin_Mode == GPIO_MODE_Input_with_pull_up)
				{
					x->ODR |= (1 << i);
				}
			}
			else
			{
				// For pins 8-15, configure using CRH
				if (i == 8)
					shift = 0;

				reset_mode_cfn(&x->CRH, shift);
				x->CRH |= (config_value << shift);

				// Activate pull-up resistor if configured
				if (y->Pin_Mode == GPIO_MODE_Input_with_pull_up)
				{
					x->ODR |= (1 << i);
				}
			}

			// Increment the shift value for the next pin
			shift += 4;
		}
	}
}




/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_DeInit
 * @brief 		-Reset GPIOx to intial value
 * @param[in] 	-GPIO_t*
 * @retval		-none
 * Note			-
 */

//reset required port by access the equivalent bit in APB2RSTR register
void MCAL_GPIO_DeInit(GPIO_t*x)
{
	if (x == GPIOA)
	{
		RCC->APB2RSTR |= (1<<2);
		RCC->APB2RSTR &= ~(1<<2);

	}
	else if (x == GPIOB)
	{
		RCC->APB2RSTR |= (1<<3);
		RCC->APB2RSTR &= ~(1<<3);

	}

	else if (x == GPIOC)
	{
		RCC->APB2RSTR  |= (1<<4);
		RCC->APB2RSTR &= ~(1<<4);
		return;
	}
	else if (x == GPIOD)
	{
		RCC->APB2RSTR |= (1<<5);
		RCC->APB2RSTR &= ~(1<<6);

	}
	else if (x == GPIOE)
	{
		RCC->APB2RSTR |= (1<<6);
		RCC->APB2RSTR &= ~(1<<6);

	}
	else
		return;
}

/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_Read_Pin
 * @brief 		-Read the value of specific pin
 * @param[in] 	-GPIO_t*
 * @param[in] 	-uint8_t                          ----> pin number
 * @retval		-uint_8                           ----> pin value
 * Note			-
 */

//read the value in case the pin is in input mode
uint8_t MCAL_GPIO_Read_Pin(GPIO_t*x,uint16_t pin_number)
{
	if ( (x->IDR & pin_number) )
		return 1 ;

	return 0 ;
}


/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_Read_Port
 * @brief 		-Read the value of specific port
 * @param[in] 	-GPIO_t*
 * @retval		-uint_16
 * Note			-
 */

//read the value in case the port is in input mode
uint16_t MCAL_GPIO_Read_Port(GPIO_t*x)
{
	return (uint16_t)(x->IDR);
}



/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_Write_Pin
 * @brief 		-write the value of specific pin
 * @param[in] 	-GPIO_t*
 * @param[in] 	-uint8_t                          ----> pin number
 * @param[in]	-uint_8                           ----> pin value
 * @retval       -none
 * Note			-
 */


void MCAL_GPIO_Write_Pin(GPIO_t*x,uint16_t pin_number,uint8_t pin_value)
{

	if(pin_value)
	{
		(x->ODR) |=  pin_number;
		return;
	}
	(x->ODR) &= ~ pin_number;
}


/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_Write_Port
 * @brief 		-write the value of specific port
 * @param[in] 	-GPIO_t*
 * @param[in] 	-uint16_t                          ----> port value
 * @retval		-none
 * Note			-
 */

void MCAL_GPIO_Write_Port(GPIO_t*x,uint16_t port_value)
{
	x->ODR = port_value ;
}

/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_toggele_Pin
 * @brief 		-Toggle the value of specific pin
 * @param[in] 	-GPIO_t*
 * @param[in] 	-uint8_t                          ----> pin number
 * @retval		-none
 * Note			-
 */

void MCAL_GPIO_Toggle_Pin(GPIO_t*x,uint16_t pin_number)
{
	x->ODR ^= pin_number;

}
/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_Toggle_Port
 * @brief 		-Toggle the value of specific port
 * @param[in] 	-GPIO_t*
 * @retval		-none
 * Note			-
 */

void MCAL_GPIO_Toggle_Port(GPIO_t*x)
{
	x->ODR ^= 0xffff;
}

/**===============================================================================================================*
 *@Fn			-MCAL_GPIO_lock_Pin
 * @brief 		-lock configurations of specifc pin
 * @param[in] 	-GPIO_t*
 * @param[in] 	-uint8_t                           ----> pin number
 * @retval		-uint8_t                           ----> pin_locked  or lock Error
 * Note			-return values are based on @ref GPIO_Lock_Status_define
 */

uint8_t MCAL_GPIO_lock_Pin(GPIO_t*x,uint8_t pin_number)
{
	unsigned char i;
	x->LCKR |= pin_number;
	x->LCKR |= (1<<16);
	x->LCKR &= ~(1<<16);                                  //locking seq.
	x->LCKR |=  (1<<16);
	for(i=0; i<255; i++);                                 //small delay to allow change of bit 16 from 0 to 1
	if(x->LCKR & 1<<16)
		return pin_locked;
	return locked_error;
}
/**===============================================================================================================*
 * @Fn			-MCAL_GPIO_lock_Port
 * @brief 		-Toggle the value of specific port
 * @param[in] 	-GPIO_t*
 * @retval		-return values are based on @ref GPIO_Lock_Status_define
 * Note			-
 */

uint8_t MCAL_GPIO_lock_Port(GPIO_t*x)
{
	unsigned char i,j ;
	for(i=0; i<16; i++)
	{
		x->LCKR &= ~(1<<16);
		x->LCKR |= (1<<i);
		x->LCKR |= (1<<16);
		x->LCKR &= ~(1<<16);
		x->LCKR |=  (1<<16);
		for(j=0; j<255; j++);
		if(x->LCKR & 1<<16)
			continue;
		else
			return locked_error;
	}

	if(x->LCKR & 1<<16)
		return port_locked;
	else
		return locked_error;

}

















