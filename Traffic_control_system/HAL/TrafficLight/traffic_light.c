#include "traffic_light.h"
Traffic_light_status current_status;

void traffic_light_init(void)
{


	// set GPIO portA pins from p0 - p5 as output pushpull to control traffic lights
	GPIO_CONFIG_t traffic_light_pins;
	traffic_light_pins.Pin_Number = North_South_Green ;
	traffic_light_pins.Output_Pin_Speed = low_speed_2Mhz ;
	traffic_light_pins.Pin_Mode = GPIO_MODE_General_purpose_output_push_pull;
	MCAL_GPIO_Init(GPIOA, &traffic_light_pins);
	traffic_light_pins.Pin_Number = North_South_Yellow ;
	MCAL_GPIO_Init(GPIOA, &traffic_light_pins);
	traffic_light_pins.Pin_Number = North_South_Red ;
	MCAL_GPIO_Init(GPIOA, &traffic_light_pins);
	traffic_light_pins.Pin_Number = East_West_Green ;
	MCAL_GPIO_Init(GPIOA, &traffic_light_pins);
	traffic_light_pins.Pin_Number = East_West_Yellow ;
	MCAL_GPIO_Init(GPIOA, &traffic_light_pins);
	traffic_light_pins.Pin_Number = East_West_Red ;
	MCAL_GPIO_Init(GPIOA, &traffic_light_pins);


	//set all LEDs to zero initially
	Traffic_light_port = 0b000000;


	// set GPIO portB pins from p8 - p13 as floating input to test the leds
	traffic_light_pins.Pin_Number = North_South_Green_led_test;
	traffic_light_pins.Pin_Mode = GPIO_MODE_Floating_input;
	MCAL_GPIO_Init(GPIOB, &traffic_light_pins);
	traffic_light_pins.Pin_Number = North_South_Yellow_led_test;
	MCAL_GPIO_Init(GPIOB, &traffic_light_pins);
	traffic_light_pins.Pin_Number = North_South_Red_led_test;
	MCAL_GPIO_Init(GPIOB, &traffic_light_pins);
	traffic_light_pins.Pin_Number = East_West_Green_led_test;
	MCAL_GPIO_Init(GPIOB, &traffic_light_pins);
	traffic_light_pins.Pin_Number = East_West_Yellow_led_test;
	MCAL_GPIO_Init(GPIOB, &traffic_light_pins);
	traffic_light_pins.Pin_Number = East_West_Red_led_test;
	MCAL_GPIO_Init(GPIOB, &traffic_light_pins);

}
void traffic_light_update(enum active_led x)
{
	switch(x)
	{
	case North_south_green_on :
		Traffic_light_port = 0b100001;
		current_status.leds_status = North_south_green_on;
		current_status.remaining_time = 20;                         //set 20 seconds for green light
		break ;
	case North_south_yellow_on :
		Traffic_light_port = 0b100010;
		current_status.leds_status = North_south_yellow_on;
		current_status.remaining_time = 5;                         //set 5 seconds for yellow light
		break ;

	case East_west_green_on :
		Traffic_light_port = 0b001100;
		current_status.leds_status = East_west_green_on;
		current_status.remaining_time = 20;                         //set 20 seconds for green light
		break ;

	case East_west_yellow_on :
		Traffic_light_port = 0b010100;
		current_status.leds_status = East_west_yellow_on;
		current_status.remaining_time = 5;                         //set 5 seconds for yellow light
		break ;
	case North_South_East_West_Red_on :
		Traffic_light_port = 0b100100;
				current_status.leds_status = East_west_yellow_on;
				current_status.remaining_time = 5;                         //set 5 seconds for yellow light
				break ;
	}
}
