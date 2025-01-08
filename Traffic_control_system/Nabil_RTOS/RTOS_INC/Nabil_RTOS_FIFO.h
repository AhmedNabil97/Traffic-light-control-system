/*
 * Nabil_RTOS_FIFO.h
 *
 *  Created on: ١٨‏/٠٥‏/٢٠٢٤
 *      Author: lenovo
 */

#ifndef INC_NABIL_RTOS_FIFO_H_
#define INC_NABIL_RTOS_FIFO_H_
#include "scheduler.h"
#include "traffic_light.h"

#define Null (void*)0
#define null (void*)0


//======================================================================= Fifo Errors===========================================================

typedef enum

{
	Fifo_no_error
	, Fifo_full
	, Fifo_empty
	, Fifo_null
	, Fifo_error
}Fifo_status_t;


//======================================================================= Fifo struct ===========================================================

typedef struct
{
	void*base;
	void*head;
	void*tail;
	unsigned int count;
	unsigned int length;
}Fifo_Buffer;

//======================================================================= Functions Declarations ===============================================


Fifo_status_t Fifo_init(Fifo_Buffer*,void*,int);
data_type Fifo_get_item(Fifo_Buffer*);
Fifo_status_t Fifo_status(Fifo_Buffer*);
Fifo_status_t Fifo_add_item(Fifo_Buffer*,data_type );

//============================================================================================================================================

#endif /* INC_NABIL_RTOS_FIFO_H_ */
