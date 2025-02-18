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
    // 1) Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 

    // 2) Configure TIM3 PSC/ARR for ~800 Hz (or your desired frequency)
    TIM3->PSC = 999;  // Prescaler
    TIM3->ARR = 99;   // Auto-reload

    // 3) Configure CH3 & CH4 in PWM mode (CCMR2 controls channels 3 & 4)
    //    We’ll set them both to PWM mode 1 (OCxM = 110 = 6)
    // Channel 3
    TIM3->CCMR2 &= ~((3 << 0) | (7 << 4)); // Clear CC3S and OC3M bits
    TIM3->CCMR2 |= (6 << 4);              // PWM Mode 1 on CH3 (OC3M = 110)
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE;       // Preload enable for CCR3

    // Channel 4
    TIM3->CCMR2 &= ~((3 << 8) | (7 << 12)); 
    TIM3->CCMR2 |= (6 << 12);             // PWM Mode 1 on CH4 (OC4M = 110)
    TIM3->CCMR2 |= TIM_CCMR2_OC4PE;       // Preload enable for CCR4

    // 4) Enable capture/compare outputs for channels 3 & 4 in CCER
    TIM3->CCER |= TIM_CCER_CC3E | TIM_CCER_CC4E;

    // 5) Optional: Set initial duty cycle, e.g., 20%
    TIM3->CCR3 = (TIM3->ARR + 1) * 0.2f;
    TIM3->CCR4 = (TIM3->ARR + 1) * 0.2f;

    // 6) Finally, enable TIM3
    TIM3->CR1 |= TIM_CR1_CEN;

    // 7) Configure GPIO PC8 & PC9 for Alternate Function 1 (TIM3_CH3, TIM3_CH4)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Make sure GPIOC is enabled
    MY_HAL_GPIO_Init_AF(GPIOC, 8, 1);   // PC8 -> AF1 (TIM3_CH3)
    MY_HAL_GPIO_Init_AF(GPIOC, 9, 1);   // PC9 -> AF1 (TIM3_CH4)
}



int lab3_main() {
    // Initialize timers, GPIO, etc.
    TIM3_Init();


    // Main loop
    while (1) {
        // Cycle through different duty cycles
        for (float duty = 0.1f; duty <= 1.0f; duty += 0.1f) {
            // Update duty cycle on each cycle
            TIM3->CCR1 = (TIM3->ARR + 1) * duty;  // Adjust duty cycle for CH1 (PC6)
            TIM3->CCR2 = (TIM3->ARR + 1) * duty;  // Adjust duty cycle for CH2 (PC7)
            HAL_Delay(500);  // Wait for 500 ms to see change
        }
    }


    return 0;
}








   

