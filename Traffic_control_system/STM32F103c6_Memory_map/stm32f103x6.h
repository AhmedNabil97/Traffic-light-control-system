/*
 * stm32f103x6.h
 *
 *  Created on: march 24, 2024
 *      Author: ahmed
 */

#ifndef STM32F103X6_H_
#define STM32F103X6_H_

//Includes

#include <stdlib.h>
#include <stdint.h>

/*-------------------------------------------------------------------------------------------------------------*/
//                                 base addresses for memories

#define Flash_Memory_Base								0x08000000UL
#define System_Memory_Base								0x1FFFF000UL
#define Sram_Memory_Base								0x20000000UL
#define Peripherals_Base								0x40000000UL
#define Cortex_M3_Internal_Peripherals_Base				0xE0000000UL
#define NVIC_BASE 										0xE000E100UL

/*-------------------------------------------------------------------------------------------------------------*/
//                                 base addresses for AHB Peripherals
//1. RCC
#define RCC_BASE 									    0x40021000UL

/*-------------------------------------------------------------------------------------------------------------*/
//                                 base addresses for APB2 Peripherals
// 1. GPIO
//FOR Pacakge_LQFP48      --->       [ A,B FULY INCLUDED] --- C,D PARTIALLY INCLUDED --- E DOESNOT INCLUDED ]
#define GPIOA_BASE										0x40010800UL
#define GPIOB_BASE										0x40010C00UL
#define GPIOC_BASE										0x40011000UL
#define GPIOD_BASE										0x40011400UL
#define GPIOE_BASE										0x40011800UL
// 2. EXTI
#define EXTI_BASE										0x40010400UL
// 3. AFIO
#define AFIO_BASE										0x40010000UL
/*-------------------------------------------------------------------------------------------------------------*/
//                                                  NVIC     REGISTERS

#define NVIC_ISER0                                      *((volatile uint32_t *)(NVIC_BASE + 0x000))
#define NVIC_ISER1                                      *((volatile uint32_t *)(NVIC_BASE + 0x004))
#define NVIC_ICER0                                      *((volatile uint32_t *)(NVIC_BASE + 0x080))
#define NVIC_ICER1                                      *((volatile uint32_t *)(NVIC_BASE + 0x084))
#define NVIC_ICPR0 										*((volatile uint32_t *)(NVIC_BASE + 0x180))
#define NVIC_ICPR1 										*((volatile uint32_t *)(NVIC_BASE + 0x184))

/*-------------------------------------------------------------------------------------------------------------*/
//                                    perphiral       registers
//1. GPIO
typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;

}GPIO_t;

//2. RCC
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;

}RCC_t;

//3. EXTI
typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_t;

//4.AFIO
typedef struct
{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	volatile uint32_t RESERVED;
	volatile uint32_t MAPR2;
}AFIO_t;

/*-------------------------------------------------------------------------------------------------------------*/
//                                    perphiral       instants

#define GPIOA										((GPIO_t*)GPIOA_BASE)
#define GPIOB										((GPIO_t*)GPIOB_BASE)
#define GPIOC										((GPIO_t*)GPIOC_BASE)
#define GPIOD										((GPIO_t*)GPIOD_BASE)
#define GPIOE										((GPIO_t*)GPIOE_BASE)
#define RCC									     	((RCC_t*)RCC_BASE)
#define EXTI										((EXTI_t*)EXTI_BASE)
#define AFIO										((AFIO_t*)AFIO_BASE)

/*-------------------------------------------------------------------------------------------------------------*/
//                                    CLOCK       ENABLE


#define GPIOA_CLK_EN()								(RCC->APB2ENR |= (1<<2))
#define GPIOB_CLK_EN()								(RCC->APB2ENR |= (1<<3))
#define GPIOC_CLK_EN()								(RCC->APB2ENR |= (1<<4))
#define GPIOD_CLK_EN()								(RCC->APB2ENR |= (1<<5))
#define GPIOE_CLK_EN()								(RCC->APB2ENR |= (1<<6))
#define AFIO_CLK_EN()								(RCC->APB2ENR |= (1<<0))


/*-------------------------------------------------------------------------------------------------------------*/
//                                            NVIC IRQ ENABLE


#define NVIC_EXTI0_ENABLE 							   NVIC_ISER0 |= (1<<6)
#define NVIC_EXTI1_ENABLE 							   NVIC_ISER0 |= (1<<7)
#define NVIC_EXTI2_ENABLE 							   NVIC_ISER0 |= (1<<8)
#define NVIC_EXTI3_ENABLE 							   NVIC_ISER0 |= (1<<9)
#define NVIC_EXTI4_ENABLE 							   NVIC_ISER0 |= (1<<10)
#define NVIC_EXTI5_9_ENABLE 						   NVIC_ISER0 |= (1<<23)
#define NVIC_EXTI10_15_ENABLE 						   NVIC_ISER1 |= (1<<8)

#define NVIC_EXTI0_DISABLE							   NVIC_ICER0 |= (1<<6)
#define NVIC_EXTI1_DISABLE							   NVIC_ICER0 |= (1<<7)
#define NVIC_EXTI2_DISABLE							   NVIC_ICER0 |= (1<<8)
#define NVIC_EXTI3_DISABLE							   NVIC_ICER0 |= (1<<9)
#define NVIC_EXTI4_DISABLE							   NVIC_ICER0 |= (1<<10)
#define NVIC_EXTI5_9_DISABLE						   NVIC_ICER0 |= (1<<23)
#define NVIC_EXTI10_15_DISABLE						   NVIC_ICER1 |= (1<<8)




/*-------------------------------------------------------------------------------------------------------------*/
//                                           INTERRUPT VECTOR TABLE


#define	 EXTI0_IRQ										6
#define  EXTI1_IRQ										7
#define  EXTI2_IRQ										8
#define  EXTI3_IRQ										9
#define  EXTI4_IRQ										10
#define  EXTI5_9_IRQ									23
#define  EXTI10_15_IRQ									40











#endif /* STM32F103X6_H_ */
