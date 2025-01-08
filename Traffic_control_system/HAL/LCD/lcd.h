
#ifndef LCD_H_
#define LCD_H_



#include "GPIO.h"





//lcd port 

typedef struct
{
	unsigned short reseved  :1 ;
	unsigned short ctrl	    :2 ;
	unsigned short Lcd_Port :4 ;
}PA_t;

typedef struct
{
	unsigned short reseved    :10 ;
	unsigned short ctrl	      :2 ;
	unsigned short Lcd_Port   :4 ;
}PA_t2;

typedef enum
{
	lcd1,lcd2
}LCD_index;

#define   PORTB_base								    (GPIOB_BASE + 0x0C)
#define   Access_pointer1 								((volatile PA_t*)PORTB_base)
#define   LCD_CTRL1										Access_pointer1->ctrl
#define   En											0
#define   RS											1

#define   PORTA_base								    (GPIOA_BASE + 0x0C)
#define   Access_pointer2 								((volatile PA_t2*)PORTA_base)
#define   LCD_CTRL2										Access_pointer2->ctrl
#define   En2											0
#define   RS2											1

// functions

void lcd_is_busy(LCD_index);
void lcd_init(void);
void lcd_send_command(unsigned char cmd,LCD_index );
void Lcd_En(LCD_index);
void lcd_display_char(unsigned char ch,LCD_index);
void lcd_display_sentence(unsigned char* p,LCD_index);
void my_delay(unsigned int i);
void _delay_ms(unsigned int );


// commands

#define lcd_clear 0x1
#define lcd_return_home 0x2
#define lcd_Entery_mode 0x6
#define lcd_display_crouser_blink_on 0x0F
#define lcd_crouser_shift_right 0x14
#define lcd_crouser_shift_left 0x10
#define lcd_display_shift_right 0x1C
#define lcd_display_shift_left 0x18
#define lcd_Eight_bit_1_line 0x34
#define lcd_8_bit_2_line 0x38 
#define lcd_4_bit_2_line 0x2C
#define lcd_begin_at_first_row 0x80
#define lcd_begin_at_seconed_row 0xC0
#define lcd_4_bit_mode_init 0x32
#define lcd_begin_at_third_row  (0x90)
#define lcd_begin_at_fourth_row	(0xD0)
#endif /* LCD_H_ */
