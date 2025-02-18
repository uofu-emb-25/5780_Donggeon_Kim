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
void TIM3_Init(void)
{
    // Enable TIM3 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Set Prescaler (PSC) and Auto-Reload Register (ARR) for ~800 Hz PWM
    TIM3->PSC = 999;  // Prescaler
    TIM3->ARR = 99;   // Auto-reload

    // Configure TIM3 Channel 1 (Red LED, PC6) in PWM mode (PWM Mode 1)
    TIM3->CCMR1 &= ~((3 << 0) | (7 << 4)); // Clear CC1S and OC1M bits
    TIM3->CCMR1 |= (6 << 4);               // PWM Mode 1 on CH1 (OC1M = 110)
    TIM3->CCMR1 |= TIM_CCMR1_OC1PE;        // Preload enable for CCR1

    // Configure TIM3 Channel 2 (Blue LED, PC7) in PWM mode (PWM Mode 1)
    TIM3->CCMR1 &= ~((3 << 8) | (7 << 12)); // Clear CC2S and OC2M bits
    TIM3->CCMR1 |= (6 << 12);              // PWM Mode 1 on CH2 (OC2M = 110)
    TIM3->CCMR1 |= TIM_CCMR1_OC2PE;        // Preload enable for CCR2

    // Enable capture/compare outputs for channels 1 and 2
    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Set initial duty cycle to 20%
    TIM3->CCR1 = (TIM3->ARR + 1) * 0.2f; // 20% duty cycle for CH1 (Red LED)
    TIM3->CCR2 = (TIM3->ARR + 1) * 0.2f; // 20% duty cycle for CH2 (Blue LED)

    // Enable TIM3 counter
    TIM3->CR1 |= TIM_CR1_CEN;
}


void GPIO_Init_TIM3_PWM(void)
{
    // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set PC8, PC9 to Alternate Function mode (AF mode)
    GPIOC->MODER &= ~((3 << (2 * 6)) | (3 << (2 * 7))); // Clear mode bits for PC6 and PC7
    GPIOC->MODER |= ((2 << (2 * 6)) | (2 << (2 * 7)));  // Set to AF mode

    // Set PC8 (TIM3_CH3) and PC9 (TIM3_CH4) to AF1
    GPIOC->AFR[1] &= ~((0xF << (4 * 6)) | (0xF << (4 * 7))); // Clear AF bits for PC6 and PC7
    GPIOC->AFR[1] |= ((1 << (4 * 6)) | (1 << (4 * 7))); // Set AF1 (TIM3_CH3 and TIM3_CH4)
}

void lab3_main(void)
{
    // Initialize HAL and configure the system clock
    HAL_Init();
    SystemClock_Config();

    // Set up GPIO for PWM output
    GPIO_Init_TIM3_PWM();

    // Configure TIM3 for PWM functionality
    TIM3_Init();

    int brightness = 0;  // Initial brightness level
    int step = 5;        // Step size for brightness adjustment

    // Infinite loop for smooth dimming and brightening
    while (1)
    {
        // Update PWM duty cycle for both LEDs
        TIM3->CCR1 = (brightness * TIM3->ARR) / 100;  // CH1 (Red LED, PWM Mode 1)
        TIM3->CCR2 = (brightness * TIM3->ARR) / 100;  // CH2 (Blue LED, PWM Mode 1)

        // Adjust brightness for next cycle
        brightness += step;

        // Reverse direction at brightness limits (0% and 100%)
        if (brightness >= 100 || brightness <= 0)
        {
            step = -step;  // Change direction
        }

        HAL_Delay(50);  // Control speed of dimming/brightening
    }
}








   

