/*
#include "stm32f0xx.h"
#include "lab7.h"
#include <stdlib.h>
#include "SEGGER_RTT.h"

#define MOTOR_EN_Pin   GPIO_PIN_8
#define MOTOR_DIRA_Pin GPIO_PIN_9
#define MOTOR_DIRB_Pin GPIO_PIN_10
#define MOTOR_PORT     GPIOC

volatile int16_t motor_speed = 0;
volatile int16_t target_speed = 100;
volatile int16_t error = 0;
volatile int16_t integral = 0;
const int16_t Kp = 2;
const int16_t Ki = 1;

void lab7_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set pins to output
    MOTOR_PORT->MODER |= (1 << (2 * 8)) | (1 << (2 * 9)) | (1 << (2 * 10));

    PWM_Init();
    Encoder_Init();
    Button_Init();

    SysTick_Config(SystemCoreClock / 100); // 10ms tick
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

    static uint8_t last = 1;
    uint8_t current = (GPIOA->IDR & GPIO_IDR_0);

    if (!current && last) { // falling edge
        if (target_speed == 100) target_speed = 50;
        else if (target_speed == 50) target_speed = 0;
        else target_speed = 100;
    }

    last = current;
    SEGGER_RTT_printf(0, "Target: %d Speed: %d Duty: %d\n", target_speed, motor_speed, abs(control));
}

void Button_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER &= ~(3 << (2 * 0)); // Input mode
}
*/


#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx.h" 
//#include "stm32f4xx.h"
#include "motor.h"
#include "SEGGER_RTT.h"

/* -------------------------------------------------------------------------------------------------------------
 *  Global Variable Declarations
 *  -------------------------------------------------------------------------------------------------------------
 */
volatile uint32_t debouncer;

/* -------------------------------------------------------------------------------------------------------------
 *  Miscellaneous Core Functions
 *  -------------------------------------------------------------------------------------------------------------
 */

void LED_init(void) {
    // Initialize PC8 and PC9 for LED's
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;                                          // Enable peripheral clock to GPIOC
    GPIOC->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;                  // Set PC8 & PC9 to outputs
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);                    // Set to push-pull output type
    GPIOC->OSPEEDR &= ~((GPIO_OSPEEDR_OSPEEDR8_0 | GPIO_OSPEEDR_OSPEEDR8_1) |
                        (GPIO_OSPEEDR_OSPEEDR9_0 | GPIO_OSPEEDR_OSPEEDR9_1));   // Set to low speed
    GPIOC->PUPDR &= ~((GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR8_1) |
                      (GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR9_1));             // Set to no pull-up/down
    GPIOC->ODR &= ~(GPIO_ODR_8 | GPIO_ODR_9);                                   // Shut off LED's
}

void  button_init(void) {
    // Initialize PA0 for button input
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;                                          // Enable peripheral clock to GPIOA
    GPIOA->MODER &= ~(GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1);               // Set PA0 to input
    GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEEDR0_0 | GPIO_OSPEEDR_OSPEEDR0_1);     // Set to low speed
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR0_1;                                        // Set to pull-down
}

/* Called by SysTick Interrupt
 * Performs button debouncing, changes wave type on button rising edge
 * Updates frequency output from ADC value
 */
void Lab7_Systick_Callback(void) {
    // Remember that this function is called by the SysTick interrupt
    // You can't call any functions in here that use delay

    debouncer = (debouncer << 1);
    if(GPIOA->IDR & (1 << 0)) {
        debouncer |= 0x1;
    }

    if(debouncer == 0x7FFFFFFF) {
        // Begin critical section
        __disable_irq();
        switch(target_rpm) {
            case 80:
                target_rpm = 50;
                break;
            case 50:
                target_rpm = 81;
                break;
            case 0:
                target_rpm = 80;
                break;
            default:
                target_rpm = 0;
                break;
        }
        __enable_irq();
        // End critical section
    }
}

/* -------------------------------------------------------------------------------------------------------------
 * Main Program Code
 *
 * Starts initialization of peripherals
 * Blinks green LED (PC9) in loop as heartbeat
 * -------------------------------------------------------------------------------------------------------------
 */
volatile uint32_t encoder_count = 0;


/* -------------------------------------------------------------------------------------------------------------
 * Called by SysTick Interrupt
 * Performs button debouncing, changes wave type on button rising edge
 * Updates frequency output from ADC value
 * ------------------------------------------------------------------------------------------------------------- 
 */
void Lab7_Systick_Callback(void) {
    debouncer = (debouncer << 1);
    if (GPIOA->IDR & (1 << 0)) {  // Check if button is pressed (PA0)
        debouncer |= 0x1;
    }

    if (debouncer == 0x7FFFFFFF) {  // If the button press is stable (debounced)
        __disable_irq();  // Begin critical section
        switch (target_rpm) {
            case 0:
                target_rpm = 80;  // Change speed to 80 RPM
                break;
            case 80:
                target_rpm = 50;  // Change speed to 50 RPM
                break;
            case 50:
                target_rpm = 80;  // Change speed to 80 RPM
                break;
            default:
                target_rpm = 0;   // Change speed to 0 RPM
                break;
        }
        __enable_irq();  // End critical section
    }
}

int lab7_main(void) {

    debouncer = 0;                          // Initialize global variables
    HAL_Init();                             // Initialize HAL internals
    LED_init();                             // Initialize LED's
    button_init();                          // Initialize button

    motor_init();                           // Initialize motor code

    while (1) {
        GPIOC->ODR ^= GPIO_ODR_9;           // Toggle green LED (heartbeat)
        encoder_count = TIM2->CNT;
        HAL_Delay(128);                      // Delay 1/8 second
    }
}

// ----------------------------------------------------------------------------
