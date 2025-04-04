#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx.h"
#include "motor.h"
#include "SEGGER_RTT.h"

/* -------------------------------------------------------------------------------------------------------------
 *  Global Variable Declarations
 * ------------------------------------------------------------------------------------------------------------- */
volatile uint32_t debouncer;  // For button press debouncing

/* -------------------------------------------------------------------------------------------------------------
 *  Miscellaneous Core Functions
 * ------------------------------------------------------------------------------------------------------------- */

void LED_init(void) {
    // Initialize PC8 and PC9 for LED's (Green LED on PC9, Blue LED on PC8)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;                                          // Enable peripheral clock to GPIOC
    GPIOC->MODER |= GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;                  // Set PC8 & PC9 to outputs
    GPIOC->OTYPER &= ~(GPIO_OTYPER_OT_8 | GPIO_OTYPER_OT_9);                    // Set to push-pull output type
    GPIOC->OSPEEDR &= ~((GPIO_OSPEEDR_OSPEEDR8_Msk) | (GPIO_OSPEEDR_OSPEEDR9_Msk));   // Set to low speed
    GPIOC->PUPDR &= ~((GPIO_PUPDR_PUPDR8_Msk) | (GPIO_PUPDR_PUPDR9_Msk));             // Set to no pull-up/down
    GPIOC->ODR &= ~(GPIO_ODR_8 | GPIO_ODR_9);                                   // Shut off LED's initially
}

void button_init(void) {
    // Initialize PA0 for button input (to control motor speed changes)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;                                          // Enable peripheral clock to GPIOA
    GPIOA->MODER &= ~(GPIO_MODER_MODER0_Msk);                                   // Set PA0 as input
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_1;                                        // Set PA0 to pull-down (for button press detection)
}

/* Called by SysTick Interrupt
 * Performs button debouncing, changes motor speed on button press (falling edge)
 */
void Lab7_Systick_Callback(void) {
    // Remember that this function is called by the SysTick interrupt
    // Debounce the button press and update target RPM

    debouncer = (debouncer << 1);  // Shift previous state to debounce
    if (GPIOA->IDR & (1 << 0)) {  // Read the button state (PA0)
        debouncer |= 0x1;
    }

    if (debouncer == 0x7FFFFFFF) {  // If the debouncer indicates a valid button press (falling edge)
        __disable_irq();  // Begin critical section (disable interrupts)
        // Toggle target speed between 0, 50, and 80 RPM
        switch (target_rpm) {
            case 80:
                target_rpm = 50;
                break;
            case 50:
                target_rpm = 0;
                break;
            case 0:
                target_rpm = 80;
                break;
            default:
                target_rpm = 0;  // Default to 0 if no matching case
                break;
        }
        __enable_irq();  // End critical section (enable interrupts)
    }
}

/* -------------------------------------------------------------------------------------------------------------
 * Main Application Entry Point
 * ------------------------------------------------------------------------------------------------------------- */
volatile uint32_t encoder_count = 0;

int lab7_main(void) {
    debouncer = 0;                          // Initialize global variables
    HAL_Init();                             // Initialize HAL internals (for hardware abstraction)
    LED_init();                             // Initialize LEDs
    button_init();                          // Initialize button

    motor_init();                           // Initialize motor control (PWM, encoder, etc.)

    SysTick_Config(SystemCoreClock / 100);  // 10ms tick for SysTick

    while (1) {
        GPIOC->ODR ^= GPIO_ODR_9;           // Toggle green LED (PC9) as heartbeat for program running
        encoder_count = TIM3->CNT;          // Get encoder count (optional for debugging)
        HAL_Delay(10);                     // ~125ms delay
    }
}
