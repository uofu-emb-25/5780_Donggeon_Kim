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