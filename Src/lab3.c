//hal_exti.c and hal_nvic.c are part of lab2
//this is a lab3
//i will keep any function inside of this file that is required to do lab 3

//TIM2 for interrupts
//TIM3

#include "stm32f072xb.h"
#include "system_setup.h"
#include <stm32f0xx_hal.h>
#include <assert.h>

#include "hal_gpio.h"
#include "hal_exti.h"
#include "hal_nvic.h"

extern void SystemClock_Config(void);
//2.3 
void MY_HAL_SYSCFG_Config_PA0(void);
//2.4
void MY_HAL_EXTI_Enable_PA0(void);

void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void);
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode);
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
volatile uint32_t main_loop_tick = 0;  // Track time in main loop

// 3.4 
void TIM2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 
    TIM2 ->PSC = 7999;
    TIM2-> ARR =250;

    TIM2->DIER |= TIM_DIER_UIE; 
    NVIC_EnableIRQ(TIM2_IRQn);   
    TIM2->CR1 |= TIM_CR1_CEN;   
}

//3.5
void TIM3_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 
    TIM3 -> PSC=999;
    TIM3-> ARR =99;
}



int lab3_main() {
    //led output
    //expected output is to have a led lighted up
    return 0;
}








   

