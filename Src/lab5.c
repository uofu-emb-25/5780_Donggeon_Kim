#include <stdio.h>
#include "stm32f072xb.h"
#include "system_setup.h"
#include "stm32f0xx_hal.h"

#define WHO_AM_I_REG         0x0F
#define EXPECTED_ID_1        0xD3  // Some sensors return 0xD3
#define EXPECTED_ID_2        0xD4  // Some sensors return 0xD4
#define TIMEOUT_LIMIT        1000000
#define I2C_SLAVE_ADDRESS 0x69

#define LED_RED    (1 << 6)  // PC6
#define LED_BLUE   (1 << 7)  // PC7
#define LED_ORANGE (1 << 8)  // PC8
#define LED_GREEN  (1 << 9)  // PC9

extern void SystemClock_Config(void);

// automoatedt I2C Address (PB14 controls SA0)
uint8_t I2C_Get_Address(void) {
    return (GPIOB->IDR & (1 << 14)) ? 0x6B : 0x69;  // high => 0x6B,  low=> 0x69 SA0
}

void GPIO_LED_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // able clock for GPIOC

    //  PC6~PC9 as output for LED
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); 
    GPIOC->MODER |= ((1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));

    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9)); // output push-pull
    GPIOC->ODR &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9)); //all LED off first
}

// GPIO initialization for I2C
void GPIO_I2C_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // Enable GPIOB clock

    // *** Set PB11 (SDA) and PB13 (SCL) as alternate function ***
    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));  
    GPIOB->MODER |= (2 << (11 * 2)) | (2 << (13 * 2));

    GPIOB->OTYPER |= (1 << 11) | (1 << 13);  // Open-drain enable
    GPIOB->PUPDR |= (1 << (11 * 2)) | (1 << (13 * 2));  // Pull-up resistor on SDA, SCL
    GPIOB->AFR[1] |= (5 << ((11 - 8) * 4)) | (5 << ((13 - 8) * 4)); // I2C2 AF5

    // *** Setup PB14 as output for SA0 ***
    GPIOB->MODER |= (1 << (14 * 2));
    GPIOB->OTYPER &= ~(1 << 14);
    GPIOB->ODR |= (1 << 14);  // SA0 HIGH (Use I2C addr 0x6B)

    // *** Set PC0 as output for SPI/I2C mode selection ***
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (1 << (0 * 2));
    GPIOC->ODR |= (1 << 0);
}

// Reset I2C Peripheral to Avoid Bus Lock
void I2C2_Reset(void) {
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST; // Reset I2C2
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST; // Release Reset
}

uint8_t I2C_Read_WHO_AM_I(void) {
    uint8_t id;
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_START | I2C_CR2_RD_WRN;
    while (!(I2C2->ISR & I2C_ISR_RXNE));  // Wait data come
    id = I2C2->RXDR;
    I2C2->CR2 |= I2C_CR2_STOP;
    return id;
}

void I2C2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    // Try a different TIMINGR value
    I2C2->TIMINGR = (1 << 28) | (0x17 << 0) | (0x9 << 8) | (0x2 << 16) | (0x4 << 20);
    
    I2C2->CR1 |= I2C_CR1_PE;
}
void Gyro_Reset(void) {
    // Reset gyroscope using CTRL_REG2 (soft reset bit)
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (2 << 16) | I2C_CR2_START;
    while (!(I2C2->ISR & I2C_ISR_TXIS));

    I2C2->TXDR = 0x21;  // CTRL_REG2 (Reset register)
    while (!(I2C2->ISR & I2C_ISR_TXIS));

    I2C2->TXDR = 0x04;  // Reset command (bit 2 = 1)
    while (!(I2C2->ISR & I2C_ISR_TC));

    I2C2->CR2 |= I2C_CR2_STOP;
    HAL_Delay(10);  // Wait for gyro to reset
}

// Verify I2C Communication with WHO_AM_I
void Verify_I2C_Communication(void) {
    uint8_t testWrite = 0x00;
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (2 << 16) | I2C_CR2_START;
    while (!(I2C2->ISR & I2C_ISR_TXIS));
    
    I2C2->TXDR = 0x20;  // Write to CTRL_REG1
    while (!(I2C2->ISR & I2C_ISR_TXIS));

    I2C2->TXDR = testWrite;  // Write zero (no power)
    while (!(I2C2->ISR & I2C_ISR_TC));
    I2C2->CR2 |= I2C_CR2_STOP;

    HAL_Delay(5);  // Short delay

    uint8_t receivedID = I2C_Read_WHO_AM_I();

    if (receivedID == EXPECTED_ID_1 || receivedID == EXPECTED_ID_2) {
        GPIOC->ODR |= LED_GREEN;  // Green ON (Success)
    } else {
        GPIOC->ODR |= LED_BLUE;  // Blue ON (Read Error)
        while (1) {
            GPIOC->ODR ^= LED_BLUE;  // Blue BLINKS
            HAL_Delay(500);
        }
    }
}



// Main function
int lab5_main_part1(void) {
    SystemClock_Config();
    GPIO_LED_Init();
    GPIO_I2C_Init();
    I2C2_Reset();
    I2C2_Init();
    GPIOC->ODR |= (1 << 8); // Orange LED ON (Processing)
    Verify_I2C_Communication();
    return 0;
}

int lab5_main_part2(void) {
    return 0;
}
