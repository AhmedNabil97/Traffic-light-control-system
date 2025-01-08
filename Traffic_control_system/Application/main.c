/*
 *
 * Created: 12/12/2023 08:47:28 م
 * Author : Ahmed Nabil
 */

#include "GPIO.h"
#include "EXTI.h"
#include "ARMCM3.h"
#include "core_cm3.h"
#include "system_ARMCM3.h"
#include "Nabil_RTOS_FIFO.h"
#include <string.h>
#include "traffic_light.h"
#include "lcd.h"
#include "buttons.h"






/////////////////////////////////////////////////// " Initialization functions " ///////////////////////////////////////////////////////////////////////////


void clock_init(void)
{

	HW_INIT()      ;    //adjust clock for system which is left as default 8 Mhz
	GPIOA_CLK_EN() ;    //Enable clock for GPIOA
	GPIOB_CLK_EN() ;    //Enable clock for GPIOB
	AFIO_CLK_EN()  ;    //Enable clock for AFIO


}

void Hard_Ware_init(void)
{
	lcd_init();
	traffic_light_init();
	button_init();
}

/////////////////////////////////////////////////// " Global Variables " ///////////////////////////////////////////////////////////////////////////
task_ref t1 , t2 ,t3 , t4;
volatile Mutex_ref m1;
Traffic_light_status all_possible_status[4] = {
		{North_south_green_on , 20} ,
		{North_south_yellow_on , 5},
		{East_west_green_on , 20} ,
		{East_west_yellow_on , 5}
};

volatile Semaphore_ref Signaling_traffic = {1 , "lights semaphore"} // signaling traffic semaphore will be used to notify task1 to check mailbox & operate with required
, Signaling_lcd1 = {0 , "LCD semaphore"},
		Signaling_lcd2 = {0 , "LCD semaphore"}	;				     // Signaling LCD semaphore will be used to notify task2 that the request is done and it can notify pedestrian to cross

volatile int  mail_box[2] = {-1,-1} ;
volatile enum directions push_button_interrupt_soure ;
volatile unsigned int req1_delay =0 , req2_delay = 0 ;
volatile unsigned char t2_status = 0;
volatile unsigned int north_south_IR_disable_time = 0 ;
volatile unsigned int east_west_IR_disable_time = 0;
volatile unsigned char number_of_active_requests = 0 ;
/////////////////////////////////////////////////////////// /////////// ///////////////////////////////////////////////////////////////////////////

//task 1 is the main task that conrols the traffic lights periodically
// task opertes green led for 40 sec then yellow for 10 and then red for 50
// task communicates with T2 VIA shared mail box and synchronization occur through semaphores and mutex
// task1 is used to activate t2 when interrupt occur

volatile int index1 = 0;
void task1_fun()

{
	volatile int i  ;
	while(1)
	{
       // activating t2 to handle the request
 		if(number_of_active_requests  && !t2_status)
		{
			t2_status = 1;
			Nabil_Activate_task(&t2);

		}
 		// acquire mutex before accessing the shared resoure which is mail  box
		Nabil_Rtos_Aquire_Mutex(&m1, &t1);
		// check if there's a msg in the mail
		if(!Signaling_traffic.semaphore_flag && ( mail_box[0] != -1 || mail_box[1] != -1))
		{
			// when there is a msg check number and msg location and then execute it
			if (mail_box[0] != -1 && mail_box[1] != -1)
			{
				traffic_light_update(North_South_East_West_Red_on);
				// unlcoking semaphores of lcds to notify t2
				Nabil_Rtos_unLock_Semaphore(&Signaling_lcd1);
				Nabil_Rtos_unLock_Semaphore(&Signaling_lcd2);

			}
			else if(mail_box[0] != -1 && mail_box[1] == -1)
			{
				traffic_light_update(mail_box[0]);
				if(mail_box[0] == East_west_green_on)
					Nabil_Rtos_unLock_Semaphore(&Signaling_lcd1);
				else
					Nabil_Rtos_unLock_Semaphore(&Signaling_lcd2);


			}
			else if(mail_box[0] == -1 && mail_box[1] != -1)
			{
				traffic_light_update(mail_box[1]);
				if(mail_box[1] == East_west_green_on)
					Nabil_Rtos_unLock_Semaphore(&Signaling_lcd1);
				else
					Nabil_Rtos_unLock_Semaphore(&Signaling_lcd2);

			}


		}
		// release mutex
		Nabil_Rtos_Release_Mutex(&m1);
		// normal operation
		for(i = index1 ; i < 4 ; i++)
		{
			if(Signaling_traffic.semaphore_flag)
			{
				// this line was added to make task1 complete execution from where it stopped.
				index1 = i;
				if(index1 == 3)
				{
					index1 = 0 ;
				}
				traffic_light_update(all_possible_status[i].leds_status);
				Nabil_RTOS_Task_wait(all_possible_status[i].remaining_time*1000 , &t1);
			}
			else
				break;
		}

	}

}


/////////////////////////////////////////////////////////////task2 function//////////////////////////////////////////////////////////////////////////////////////

void task2_fun()
{

// this task is respoinsible for handling pedestrian requests
// task is only activated as a response for interrupt request for one of the 4 push buttons exist in the 4 directions
// task communicate with the main task through mail box and synchronization depending on semaphores and mutex
// task2 is also controlling the 2 LCDs to notify the pedestrian to cross the road


// systic timer responsible for delays
//1 tick = 2 ms
//so any number is set for delays when multiplied by 2ms will generate required delay
//ex req delay 1 is set for 15000
// 15000 * 2ms  = 30 sec. which is the required delay


	while(1)
	{


		if(t1.task_state == suspended && number_of_active_requests)
		{
			Nabil_Activate_task(&t1);
		}

		switch(push_button_interrupt_soure)
		{
		case north :
		case south :
			Nabil_Rtos_Aquire_Mutex(&m1, &t2);
			if(mail_box[0] == -1)
			{
				mail_box[0] = East_west_green_on;
				Nabil_Rtos_Release_Mutex(&m1);
				// locking that semaphore notify task1 to check the mail box
				Nabil_Rtos_Lock_Semaphore(&Signaling_traffic);
				// waiting task1 to unlock LCD semaphore to notify pedestrian they can wa;l
				while(! check_Semaphore(&Signaling_lcd1));
				// lock the semaphore again for next requests
				Nabil_Rtos_Lock_Semaphore(&Signaling_lcd1);
				// delay for 30 sec to allow crossing the road
				req1_delay = 15000 ;
				// disable north_south interrupt request
				OS_SVC_Set(6);
				north_south_IR_disable_time = 22500;           // disable interrupts for 45 sec. to allow smooth traffic in all directions
				push_button_interrupt_soure = interrupt_handled;
				lcd_display_sentence("pedestrian can cross NORTH / SOUTH way ", lcd1);
			}
			else if(mail_box[1] == -1)
			{
				mail_box[1] = East_west_green_on;
				Nabil_Rtos_Release_Mutex(&m1);
				Nabil_Rtos_Lock_Semaphore(&Signaling_traffic);
				while(! check_Semaphore(&Signaling_lcd1));
				Nabil_Rtos_Lock_Semaphore(&Signaling_lcd1);
				req2_delay = 15000;
				OS_SVC_Set(6);
				north_south_IR_disable_time = 22500;           // disable interrupts for 45 sec. to allow smooth traffic in all directions
				push_button_interrupt_soure = interrupt_handled;
				lcd_display_sentence("pedestrian can cross NORTH / SOUTH way ", lcd1);
			}

			break;
		case east :
		case west :
			if(mail_box[0] == -1)
			{
				mail_box[0] = North_south_green_on;
				Nabil_Rtos_Lock_Semaphore(&Signaling_traffic);
				while(! check_Semaphore(&Signaling_lcd2));
				Nabil_Rtos_Lock_Semaphore(&Signaling_lcd2);
				req1_delay = 15000 ;                           // delay for 30 sec to allow crossing the road
				OS_SVC_Set(8);
				east_west_IR_disable_time = 22500;           // disable interrupts for 45 sec. to allow smooth traffic in all directions
				push_button_interrupt_soure = interrupt_handled;
				lcd_display_sentence("pedestrian can cross East / West way ", lcd2);
			}
			else if(mail_box[1] == -1)
			{
				mail_box[1] = North_south_green_on;
				Nabil_Rtos_Lock_Semaphore(&Signaling_traffic);
				while(! check_Semaphore(&Signaling_lcd2));
				Nabil_Rtos_Lock_Semaphore(&Signaling_lcd2);
				req2_delay = 15000;
				OS_SVC_Set(8);
				east_west_IR_disable_time = 22500;           // disable interrupts for 45 sec. to allow smooth traffic in all directions
				push_button_interrupt_soure = interrupt_handled;
				lcd_display_sentence("pedestrian can cross East / West way ", lcd2);
			}

			break;
		case interrupt_handled :
		default :
			break;
		}

		Nabil_Rtos_Aquire_Mutex(&m1, &t2);
		if(!req1_delay && mail_box[0] != -1)
		{

			if(mail_box[0] == North_south_green_on)
			{
				lcd_send_command(lcd_clear, lcd2);
			}
			else
			{
				lcd_send_command(lcd_clear, lcd1);
			}
			mail_box[0] = -1;
			number_of_active_requests --;

		}
		if(!req2_delay && mail_box[1] != -1)
		{

			if(mail_box[1] == North_south_green_on)
			{
				lcd_send_command(lcd_clear, lcd2);
			}
			else
			{
				lcd_send_command(lcd_clear, lcd1);
			}
			mail_box[1] = -1;

			number_of_active_requests --;
		}
		if(!req1_delay && !req2_delay )
		{
			mail_box[0] = -1;
			mail_box[1] = -1;
			number_of_active_requests = 0;
			Nabil_Rtos_unLock_Semaphore(&Signaling_traffic);
			t2_status = 0;
		}
		Nabil_Rtos_Release_Mutex(&m1);
		if(! north_south_IR_disable_time && !east_west_IR_disable_time && check_Semaphore(&Signaling_traffic) )                       // allow interrupts again after the delay time finished
		{
			OS_SVC_Set(5);
			OS_SVC_Set(7);
			Nabil_Terminate_task(&t2);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(void)

{

//==================================================== intilization========================================================================//
	clock_init();
	Hard_Ware_init();
	My_RTOS_Init() ;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//==================================================== adjusting tasks========================================================================//


	t1.piriority = 3 ;
	t1.ptr_to_task = task1_fun;
	t1.stack_size = 1024 ;
	strcpy(t1.task_name , "task 1 ");
	Nabil_Rtos_Create_Task(&t1);
	t2.piriority = 3 ;
	t2.ptr_to_task = task2_fun;
	t2.stack_size = 1024 ;
	strcpy(t2.task_name , "task 2 ");
	Nabil_Rtos_Create_Task(&t2);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//==================================================== adjusting Mutex========================================================================//

	m1.current_handler = NULL;
	m1.next_handler = Null;
	m1.payload = mail_box;
	m1.paylaod_size = 8;
	strcpy(m1.mutex_name , "mutex shared between t1 - t2");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//==================================================== starting OS========================================================================//

	Nabil_Activate_task(&t1);
	OS_Start();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}



void North_Pb_IRQ(void)
{
	push_button_interrupt_soure = north;
	number_of_active_requests++;

}
void South_Pb_IRQ(void)
{
	push_button_interrupt_soure = south;
	number_of_active_requests++;

}
void East_Pb_IRQ(void)
{
	push_button_interrupt_soure = east;
	number_of_active_requests++;

}
void West_Pb_IRQ(void)
{
	push_button_interrupt_soure = west;
	number_of_active_requests++;

}
