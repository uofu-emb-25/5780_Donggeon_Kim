/*
#ifndef LAB7_H
#define LAB7_H

#include <stdint.h>

void lab7_Init(void);
void PWM_Init(void);
void Encoder_Init(void);
void Button_Init(void);
void Set_Motor_Output(int enable, int direction);
void Set_PWM_Duty(uint8_t duty);
int16_t Get_Encoder_Speed(void);
void Lab7_SysTick_Handler(void);

extern volatile int16_t motor_speed;
extern volatile int16_t target_speed;

#endif // LAB7_H
*/
#ifndef LAB7_H
#define LAB7_H
#include "stm32f0xx.h" 
//#include "stm32f4xx.h" // Include the STM32F4xx header file

// -------------------------------------------------------------------------------------------------------------
// Global Variable Declarations
// -------------------------------------------------------------------------------------------------------------
extern volatile uint32_t debouncer;
extern volatile uint32_t encoder_count;

// -------------------------------------------------------------------------------------------------------------
// Function Declarations
// -------------------------------------------------------------------------------------------------------------

// LED Initialization
void LED_init(void);

// Button Initialization
void button_init(void);

// SysTick Callback for Button Debouncing and Frequency Updates
void Lab7_Systick_Callback(void);

// Main Program Function (lab7_main)
void lab7_main(void);

// Motor Initialization (From motor.c, assumed function)
void motor_init(void);

// -------------------------------------------------------------------------------------------------------------
// Constant Definitions
// -------------------------------------------------------------------------------------------------------------
#define TARGET_RPM_80   80
#define TARGET_RPM_50   50
#define TARGET_RPM_81   81
#define TARGET_RPM_0    0

// Add any other constants or macros as needed

#endif // LAB7_H
