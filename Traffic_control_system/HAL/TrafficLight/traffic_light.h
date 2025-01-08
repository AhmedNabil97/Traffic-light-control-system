/*
 * traffic_light.h
 *
 *  Created on: ٠٥‏/٠١‏/٢٠٢٥
 *      Author: lenovo
 */

#ifndef TRAFFIC_LIGHT_H_
#define TRAFFIC_LIGHT_H_
#include "GPIO.h"



// used to access all traffic Leds once
typedef struct
{
	unsigned short T_Port :6 ;
}Traffic_port;
#define   Traffic_port_base								(GPIOA_BASE + 0x0C)
#define   Traffic_light_pointer	       ((volatile Traffic_port*)Traffic_port_base)
#define   Traffic_light_port			Traffic_light_pointer->T_Port


// traffic light pins ----> access single pins
#define North_South_Green		GPIO_pin0
#define North_South_Yellow		GPIO_pin1
#define North_South_Red			GPIO_pin2
#define East_West_Green			GPIO_pin3
#define East_West_Yellow		GPIO_pin4
#define East_West_Red	  		GPIO_pin5


// traffic LEDs      ----> testing
#define North_South_Green_led_test		GPIO_pin8
#define North_South_Yellow_led_test		GPIO_pin9
#define North_South_Red_led_test		GPIO_pin10
#define East_West_Green_led_test		GPIO_pin11
#define East_West_Yellow_led_test		GPIO_pin12
#define East_West_Red_led_test	  		GPIO_pin13

// traffic light status
typedef struct
{
	enum active_led
	{
	North_south_green_on ,
	North_south_yellow_on ,
	East_west_green_on ,
	East_west_yellow_on ,
	North_South_East_West_Red_on
	}leds_status;
	unsigned char remaining_time;
	}Traffic_light_status;


// functions
void traffic_light_init(void);							//set Gpio configurations for traffic light pins for operation ---->  output push pull and leds for testing ------> floating input
void traffic_light_update(enum active_led);				//update traffic light with proper status & update current status

#endif /* TRAFFIC_LIGHT_H_ */
