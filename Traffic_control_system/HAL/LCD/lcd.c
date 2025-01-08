
#include <string.h>
#include "lcd.h"


void _delay_ms(unsigned int q)
{
	char j,k;
	for (k=0; k<q; k++)
	{
		for(j=0; j<255; j++);
	}


}


void my_delay(unsigned int i)
{
  volatile  unsigned int k,j;
  for(k=0; k<i; k++)
  {
     for(j=0; j<255; j++);
  }
}
void Lcd_En(LCD_index i)
{
	if(i == lcd1)
	{
	LCD_CTRL1 |= (1<<En);
	my_delay(20);
	LCD_CTRL1 &= ~(1<<En);
	}
	else if(i == lcd2 )
	{
		LCD_CTRL2 |= (1<<En2);
		my_delay(20);
		LCD_CTRL2 &= ~(1<<En2);

	}
}



// Function to check if LCD is busy
void lcd_is_busy(LCD_index i)
{
my_delay(50);
if(i == lcd1)
LCD_CTRL1 &= ~(1<<RS);
else
LCD_CTRL2 &= ~(1<<RS2);

}

void lcd_send_command(unsigned char cmd,LCD_index i)
{

	lcd_is_busy(i);
	if(i == lcd1)
	{
	Access_pointer1->Lcd_Port = ( (cmd & 0xF0) >> 4);                    // Send higher nibble
	LCD_CTRL1 &= ~(1<<RS);
	Lcd_En(lcd1);
	 Access_pointer1->Lcd_Port = (cmd & 0x0F);                            // Send Lower nibble
	Lcd_En(lcd1);
	}
	else
	{
		Access_pointer2->Lcd_Port = ( (cmd & 0xF0) >> 4);                    // Send higher nibble
		LCD_CTRL2 &= ~(1<<RS2);
		Lcd_En(lcd2);
		 Access_pointer2->Lcd_Port = (cmd & 0x0F);                            // Send Lower nibble
		Lcd_En(lcd2);

	}
}


// Function to initialize the LCD
void lcd_init(void)
{
	my_delay(50);

    GPIO_CONFIG_t lcd_pin;
    // Set LCD pins as genral purpose output psh-pull
    //set pins for LCD1
    lcd_pin.Pin_Number = GPIO_pin1;
    lcd_pin.Pin_Mode = GPIO_MODE_General_purpose_output_push_pull;
    lcd_pin.Output_Pin_Speed = medium_speed_10Mhz;
    MCAL_GPIO_Init(GPIOB, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin2;
    MCAL_GPIO_Init(GPIOB, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin3;
    MCAL_GPIO_Init(GPIOB, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin4;
    MCAL_GPIO_Init(GPIOB, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin5;
    MCAL_GPIO_Init(GPIOB, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin6;
    MCAL_GPIO_Init(GPIOB, & lcd_pin);


    //set pins for LCD2
    lcd_pin.Pin_Number = GPIO_pin10;
    lcd_pin.Pin_Mode = GPIO_MODE_General_purpose_output_push_pull;
    lcd_pin.Output_Pin_Speed = medium_speed_10Mhz;
    MCAL_GPIO_Init(GPIOA, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin11;
    MCAL_GPIO_Init(GPIOA, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin12;
    MCAL_GPIO_Init(GPIOA, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin13;
    MCAL_GPIO_Init(GPIOA, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin14;
    MCAL_GPIO_Init(GPIOA, & lcd_pin);
    lcd_pin.Pin_Number = GPIO_pin15;
    MCAL_GPIO_Init(GPIOA, & lcd_pin);



    // Clear control lines
    LCD_CTRL1 &= ~( (1<<RS) | (1<<En));
    LCD_CTRL2 &= ~( (1<<RS) | (1<<En));

    lcd_is_busy(lcd1);
	lcd_send_command(lcd_4_bit_mode_init,lcd1);
    lcd_is_busy(lcd2);
	lcd_send_command(lcd_4_bit_mode_init,lcd2);
	lcd_is_busy(lcd1);
	lcd_send_command(lcd_4_bit_2_line,lcd1);
    lcd_is_busy(lcd2);
	lcd_send_command(lcd_4_bit_2_line,lcd2);
	lcd_is_busy(lcd1);
	lcd_send_command(lcd_clear,lcd1);
    lcd_is_busy(lcd2);
	lcd_send_command(lcd_clear,lcd2);
	lcd_is_busy(lcd1);
	lcd_send_command(lcd_return_home,lcd1);
    lcd_is_busy(lcd2);
	lcd_send_command(lcd_return_home,lcd2);
	lcd_is_busy(lcd1);
    lcd_send_command(lcd_Entery_mode,lcd1);
    lcd_is_busy(lcd2);
    lcd_send_command(lcd_Entery_mode,lcd2);
	lcd_is_busy(lcd1);
	lcd_send_command(lcd_display_crouser_blink_on,lcd1);
    lcd_is_busy(lcd2);
	lcd_send_command(lcd_display_crouser_blink_on,lcd2);
	lcd_is_busy(lcd1);
	lcd_send_command(lcd_begin_at_first_row,lcd1);
    lcd_is_busy(lcd2);
	lcd_send_command(lcd_begin_at_first_row,lcd2);


}

// Function to display a character on the LCD

void lcd_display_char(unsigned char ch,LCD_index i)
{

	if(i == lcd1)
	{
	lcd_is_busy(lcd1);
	LCD_CTRL1 |= (1<<RS);
	Access_pointer1->Lcd_Port = ( (ch & 0xF0) >> 4);                    // Send higher nibble
	Lcd_En(lcd1);
	Access_pointer1->Lcd_Port = (ch & 0x0F);                            // Send Lower nibble
    Lcd_En(lcd1);
	}
	else if (i == lcd2)
	{
		lcd_is_busy(lcd2);
		LCD_CTRL2 |= (1<<RS);
		Access_pointer2->Lcd_Port = ( (ch & 0xF0) >> 4);                    // Send higher nibble
		Lcd_En(lcd2);
		Access_pointer2->Lcd_Port = (ch & 0x0F);                            // Send Lower nibble
	    Lcd_En(lcd2);

	}
}

void lcd_display_sentence(unsigned char*p,LCD_index i)
{
	char index=0;
	while(p[index])
	{
		lcd_is_busy(i);
		if(index == 16)
		{
		lcd_send_command(lcd_begin_at_seconed_row,i);
		}
		if(index == 32)
		{
		lcd_send_command(lcd_begin_at_third_row,i);
		}
		if(index == 48)
		{
		lcd_send_command(lcd_begin_at_fourth_row,i);
		}

	lcd_display_char(p[index],i);
	index++;
	}
}
