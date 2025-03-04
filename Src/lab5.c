//Lab5 
#include <stdio.h>
#include "stm32f072xb.h"
#include "system_setup.h"
#include <stdbool.h>

// External Function
extern void SystemClock_Config(void);


#define I2C_SLAVE_ADDRESS  0x6B  // L3GD20 I2C Address
#define WHO_AM_I_REG       0x0F  // Register to check device identity
#define EXPECTED_ID        0xD4  // Expected value from WHO_AM_I


#define SYS_CLOCK 8000000  // 8 MHz Clock


//  GPIO Initialization
void GPIO_Init(void) {
    
}

void I2C2_Init(void) {
    // Enable I2C2 Clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // Configure I2C2 Timing for 100kHz (from STM32 Reference Manual)
    I2C2->TIMINGR = 0x10420F13; 

    // Enable I2C2 Peripheral
    I2C2->CR1 |= I2C_CR1_PE;
}
s





void lab5_main_part1() {
    
    
    return 0;
}

void lab5_main_part2() {
    
    
    return 0;
}