/**
  ******************************************************************************
  * @file    Templates/Src/stm32f0xx_it.c
  * @author  MCD Application Team
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "main.h"
#include <stm32f0xx_hal.h>
#include <stm32f0xx_it.h>
#include "stm32f072xb.h"

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */

 //it was originally HAL_IncTick()
 //modifiying it to toggle blue led PC7 every 200ms
extern volatile uint32_t main_loop_tick;  //  global variable from lab2.c


void SysTick_Handler(void)
{
    static volatile uint32_t tick_count = 0;
  
    HAL_IncTick();  // Maintain HAL timing functions
    tick_count++;   // 
    main_loop_tick++; //

    if (tick_count >= 200) {  //  200ms
        GPIOC->ODR ^= (1 << 7); // blinking Blue LED (PC7)
        tick_count = 0;  // control variable -  counter
    }
}

//2.5

void EXTI0_1_IRQHandler(void)
{
    // Check if PA0 (EXTI0) triggered the interrupt
    if (EXTI->PR & (1 << 0)) {  
        GPIOC->ODR ^= (1 << 8); // Toggle Orange LED (PC8)
        GPIOC->ODR ^= (1 << 9); // Toggle Green LED (PC9)

        EXTI->PR |= (1 << 0);  // ✅ Clear the EXTI pending flag
    }
}


/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
