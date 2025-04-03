#include "stm32f0xx.h"
#include "lab7.h"

#define MOTOR_EN_Pin   GPIO_PIN_8
#define MOTOR_DIRA_Pin GPIO_PIN_9
#define MOTOR_DIRB_Pin GPIO_PIN_10
#define MOTOR_PORT     GPIOC

void lab7_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set pins to output
    MOTOR_PORT->MODER |= (1 << (2 * 8))| (1 << (2 * 9)) | (1 << (2 * 10));
}

void Set_Motor_Output(int enable, int direction) {
    if (enable) {
        MOTOR_PORT->ODR |= MOTOR_EN_Pin;
    } else {
        MOTOR_PORT->ODR &= ~MOTOR_EN_Pin;
    }

    if (direction >= 0) {
        MOTOR_PORT->ODR |= MOTOR_DIRA_Pin;
        MOTOR_PORT->ODR &= ~MOTOR_DIRB_Pin;
    } else {
        MOTOR_PORT->ODR &= ~MOTOR_DIRA_Pin;
        MOTOR_PORT->ODR |= MOTOR_DIRB_Pin;
    }
}

void PWM_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->AHBENR  |= RCC_AHBENR_GPIOCEN;

    // Alternate function for PC8 -> TIM3_CH3
    GPIOC->MODER &= ~(3 << (2 * 8));
    GPIOC->MODER |= (2 << (2 * 8)); // AF
    GPIOC->AFR[1] |= (0x00 << (4 * 0)); // AF0

    TIM3->PSC = 479; // 48 MHz / 480 = 100 kHz
    TIM3->ARR = 255;
    TIM3->CCR3 = 0;
    TIM3->CCMR2 |= (6 << 4); // PWM mode 1
    TIM3->CCER |= TIM_CCER_CC3E;
    TIM3->CR1 |= TIM_CR1_CEN;
}

void Set_PWM_Duty(uint8_t duty) {
    if (duty > 255) duty = 255;
    TIM3->CCR3 = duty;
}