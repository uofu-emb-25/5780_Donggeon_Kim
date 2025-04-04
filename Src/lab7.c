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

void Encoder_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;

    // PA0/PA1 as alternate function (TIM2_CH1/CH2)
    GPIOA->MODER &= ~((3 << (2 * 0)) | (3 << (2 * 1)));
    GPIOA->MODER |= (2 << (2 * 0)) | (2 << (2 * 1));
    GPIOA->AFR[0] |= (1 << (4 * 0)) | (1 << (4 * 1)); // AF1

    TIM2->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1; // Encoder mode 3
    TIM2->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0; // Inputs
    TIM2->ARR = 0xFFFF;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}
volatile int16_t motor_speed = 0;
volatile int16_t target_speed = 100;
volatile int16_t error = 0;
volatile int16_t integral = 0;
const int16_t Kp = 2;
const int16_t Ki = 1;

int16_t Get_Encoder_Speed(void) {
    static int16_t last = 0;
    int16_t current = TIM2->CNT;
    int16_t diff = current - last;
    last = current;
    return diff;
}
void Lab7_SysTick_Handler(void) {
    motor_speed = Get_Encoder_Speed();
    error = target_speed - motor_speed;
    integral += error;

    int16_t control = (Kp * error) + (Ki * integral);

    // Clamp output and direction
    if (control > 255) control = 255;
    if (control < -255) control = -255;

    Set_Motor_Output(1, control);
    Set_PWM_Duty((uint8_t)abs(control));
}
