/*
 * scheduler.h
 *
 *  Created on: ١٨‏/٠٥‏/٢٠٢٤
 *      Author: lenovo
 */

#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "Cortex_MX_OS_Porting.h"
#define data_type task_ref*


//======================================================================= OS Errors=========================================================
enum RTOS_ERROR
{
	NO_ERROR,
	OS_fifo_error,
	OS_exceed_stack_size_error,
	OS_mutex_busy
}OS_error;

//======================================================================= Task Struct ======================================================

typedef struct
{
	unsigned int stack_size  ;
	unsigned char piriority  ;
	void(*ptr_to_task)()     ;
	unsigned int _S_PSP_task ;    //not entered by the user
	unsigned int _E_PSP_task ;    //not entered by the user
	unsigned int* current_PSP;    //not entered by the user
	char  task_name[40]      ;
	enum
	{
		suspended,
		waiting,
		ready,
		running
	}task_state;

	struct
	{
		enum
		{
			enable,
			disable
		}blocking;
		unsigned int no_of_ticks;
	}time_waiting;     //blocking based on time yes or no and the period.
	enum
	{
		false,
		true
	}piriority_inheritance ;
	unsigned char piriority_backup ;
}task_ref;

//======================================================================= Mutex Struct ======================================================

typedef struct
{

	unsigned char * payload ;
	unsigned int paylaod_size ;
	task_ref* current_handler ;
	task_ref* next_handler;
	char mutex_name[40];
}Mutex_ref;

//======================================================================= Semaphores Struct ======================================================

typedef struct
{

	unsigned char semaphore_flag ;
	char Semaphore_name[40];
}Semaphore_ref;

//======================================================================= Functions Declarations ========================================================

enum RTOS_ERROR My_RTOS_Init();
enum RTOS_ERROR Nabil_Rtos_Create_Task(task_ref*);
enum RTOS_ERROR Nabil_Rtos_create_task_stack(task_ref*);
enum RTOS_ERROR Nabil_Activate_task(task_ref*);
enum RTOS_ERROR Nabil_Terminate_task(task_ref*);
enum RTOS_ERROR Nabil_Rtos_Aquire_Mutex(Mutex_ref* , task_ref*);
enum RTOS_ERROR Nabil_Rtos_Release_Mutex(Mutex_ref*);
enum RTOS_ERROR Nabil_Rtos_Lock_Semaphore(Semaphore_ref*);
enum RTOS_ERROR Nabil_Rtos_unLock_Semaphore(Semaphore_ref*);
unsigned char	check_Semaphore(Semaphore_ref*);
void Nabil_RTOS_Task_wait(unsigned int no_of_mSecs , task_ref*);
void OS_Start();

//====================================================================================================================================================

#endif /* INC_SCHEDULER_H_ */
