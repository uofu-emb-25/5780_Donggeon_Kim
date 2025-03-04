//Lab5  -- done with help with GPT 4o
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


void GPIO_Init(void) {
    // enable GPIOB and GPIOC clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    // pb11 (SDA), pb13 (SCL) - Alternate Function mode, Open-Drain
    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));  // Clear mode
    GPIOB->MODER |= (2 << (11 * 2)) | (2 << (13 * 2));     // Set AF mode

    GPIOB->OTYPER |= (1 << 11) | (1 << 13);  // Open-Drain mode
    GPIOB->PUPDR |= (1 << (11 * 2)) | (1 << (13 * 2));  // Pull-up resistors

    // set Alternate Function (AF) for I2C2
    GPIOB->AFR[1] |= (5 << ((11 - 8) * 4)) | (5 << ((13 - 8) * 4));

    // PB14 and PC0 as push-pull outputs (for selecting I2C mode)
    GPIOB->MODER |= (1 << (14 * 2));  // Output mode
    GPIOC->MODER |= (1 << (0 * 2));

    GPIOB->OTYPER &= ~(1 << 14);  // Push-Pull mode
    GPIOC->OTYPER &= ~(1 << 0);

    GPIOB->ODR |= (1 << 14);  // Set PB14 HIGH
    GPIOC->ODR |= (1 << 0);   // Set PC0 HIGH
}

void I2C2_Init(void) {
  
    // Enable I2C2 Clock
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // Configure I2C2 Timing for 100kHz (from STM32 Reference Manual)
    I2C2->TIMINGR = 0x10420F13; 

    // Enable I2C2 Peripheral
    I2C2->CR1 |= I2C_CR1_PE;
}

void I2C_Write(uint8_t deviceAddr, uint8_t regAddr) {

    // make slave address and write mode (W = 0)
    I2C2->CR2 = (deviceAddr << 1) | (1 << 16);  // 1 byte transfer

    // start condition
    I2C2->CR2 |= I2C_CR2_START;

    // wait for TXIS or NACKF
    while (!(I2C2->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));

    if (I2C2->ISR & I2C_ISR_NACKF) {
        I2C2->ICR |= I2C_ICR_NACKCF;  // Clear NACK flag
        return;
    }

    // send register address
    I2C2->TXDR = regAddr;

    // wait for transfer complete (TC) flag
    while (!(I2C2->ISR & I2C_ISR_TC));

    // stop condition
    I2C2->CR2 |= I2C_CR2_STOP;
}

uint8_t I2C_Read(uint8_t deviceAddr) {
    uint8_t data;

    // Set slave address and read mode (R = 1)
    I2C2->CR2 = (deviceAddr << 1) | (1 << 16) | I2C_CR2_RD_WRN;

    // Start Condition
    I2C2->CR2 |= I2C_CR2_START;

    // Wait for RXNE or NACKF
    while (!(I2C2->ISR & (I2C_ISR_RXNE | I2C_ISR_NACKF)));

    if (I2C2->ISR & I2C_ISR_NACKF) {
        I2C2->ICR |= I2C_ICR_NACKCF;  // Clear NACK flag
        return 0;
    }

    // Read Data
    data = I2C2->RXDR;

    // Wait for Transfer Complete (TC)
    while (!(I2C2->ISR & I2C_ISR_TC));

    // Stop Condition
    I2C2->CR2 |= I2C_CR2_STOP;

    return data;
}


void lab5_main_part1() {
    
    
    return 0;
}

void lab5_main_part2() {
    
    
    return 0;
}