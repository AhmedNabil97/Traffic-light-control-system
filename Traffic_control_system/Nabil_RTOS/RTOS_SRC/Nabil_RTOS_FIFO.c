/*
 * Nabil_RTOS_FIFO.c
 *
 *  Created on: ١٨‏/٠٥‏/٢٠٢٤
 *      Author: lenovo
 */


#include "Nabil_RTOS_FIFO.h"

Fifo_status_t Fifo_init(Fifo_Buffer*p,void*address,int length)
{
	if(address == null)
		return Fifo_null;
	p->base = address;
	p->head = address;
	p->tail = address;
	p->count =0;
	p->length = length;
	return Fifo_no_error;
}
Fifo_status_t Fifo_add_item(Fifo_Buffer*p,data_type x)
{
	if(p->base == null)
	{
		return Fifo_null;
	}
	if(p->count == p->length)
	{
		return Fifo_full;

	}
	  if((data_type*)p->tail - (data_type*)p->base == p->length)                    // apply circular principle
		     {
		    	 p->tail = p->base;
		     }
            *((data_type*)(p->tail)) = x;
			p->tail = (data_type*)p->tail +1;
			p->count ++;
	return Fifo_no_error;
}
data_type Fifo_get_item(Fifo_Buffer*p)
{
	data_type val;
	if(p->base == null)
		return Null;
	if(p->count == 0)
		return Null;
	if( ((data_type*)p->head) - ((data_type*)p->base) == p->length-1 )              //read last element
			{
		        val = *((data_type*)(p->head));
				p->head = p->base;
				p->count--;
				return val;
			}
			p->count--;
			 val = *((data_type*)(p->head));
			p->head = (data_type*)p->head +1;
	return val;
}
Fifo_status_t Fifo_status(Fifo_Buffer*p)
{
	if(p->count == 0)
		return Fifo_empty;
	if(p->base == null)
		return Fifo_null;
	if(p->count == p->length)
		return Fifo_full;
	return Fifo_no_error;
}

