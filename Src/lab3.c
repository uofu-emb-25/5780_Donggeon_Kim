//hal_exti.c and hal_nvic.c are part of lab2
//this is a lab3 with help of gpt 4o
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
    
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos);  // PWM Mode 1 for both channels
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC2PE;  // Enable preload

    // Enable Capture/Compare outputs
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Enable TIM3 Counter
    TIM3->CR1 |= TIM_CR1_CEN;


       // Set duty cycle to 20% (CCR = ARR * 0.2)
    TIM3->CCR1 = (TIM3->ARR + 1) * 0.2;
    TIM3->CCR2 = (TIM3->ARR + 1) * 0.2;


    //calling hal_gpio for pc6 and pc7
    
// Enable GPIOC clock
RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

// Set PC6 & PC7 to AF1 (TIM3_CH1 and TIM3_CH2)
MY_HAL_GPIO_Init_AF(GPIOC, 6, 1);
MY_HAL_GPIO_Init_AF(GPIOC, 7, 1);

}



int lab3_main() {
    //led output
    //expected output is to have a led lighted up
    return 0;
}








   

