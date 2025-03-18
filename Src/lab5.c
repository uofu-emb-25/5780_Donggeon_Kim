#include <stdio.h>
#include "stm32f072xb.h"
#include "system_setup.h"
#include "stm32f0xx_hal.h"

#define WHO_AM_I_REG         0x0F
#define EXPECTED_ID_1        0xD3  // Some sensors return 0xD3
#define EXPECTED_ID_2        0xD4  // Some sensors return 0xD4
#define TIMEOUT_LIMIT        1000000

extern void SystemClock_Config(void);

// Auto-detect I2C Address (PB14 controls SA0)
uint8_t I2C_Get_Address(void) {
    return (GPIOB->IDR & (1 << 14)) ? 0x6B : 0x69;  // SA0 HIGH → 0x6B, SA0 LOW → 0x69
}

// GPIO Initialization for LEDs
void GPIO_LED_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOC->MODER |= ((1 << (6 * 2)) | (0 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2))); // PC7 input
    GPIOC->OTYPER &= ~((1 << 6) | (1 << 8) | (1 << 9));
    GPIOC->PUPDR &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOC->ODR &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
}

// GPIO Initialization for I2C
void GPIO_I2C_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    
    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));
    GPIOB->MODER |= (2 << (11 * 2)) | (2 << (13 * 2)); // PB11, PB13 alternate function
    GPIOB->OTYPER |= (1 << 11) | (1 << 13); // Open-drain
    GPIOB->PUPDR |= (1 << (11 * 2)) | (1 << (13 * 2)); // Pull-ups enabled
    GPIOB->AFR[1] |= (5 << ((11 - 8) * 4)) | (5 << ((13 - 8) * 4)); // AF5 for I2C2

    GPIOB->MODER |= (1 << (14 * 2)); // PB14 output (for SA0)
    GPIOB->OTYPER &= ~(1 << 14);
    GPIOB->ODR &= ~(1 << 14); // SA0 LOW → I2C Address 0x69 (Set HIGH for 0x6B)

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (1 << (0 * 2)); // PC0 output
    GPIOC->OTYPER &= ~(1 << 0);
    GPIOC->ODR |= (1 << 0); // CS high
}

// Reset I2C Peripheral to Avoid Bus Lock
void I2C2_Reset(void) {
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST; // Reset I2C2
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST; // Release Reset
}

// I2C Initialization
void I2C2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    I2C2->TIMINGR = (1 << I2C_TIMINGR_PRESC_Pos) |
                (0x13 << I2C_TIMINGR_SCLL_Pos) |
                (0xF << I2C_TIMINGR_SCLH_Pos) |
                (0x2 << I2C_TIMINGR_SDADEL_Pos) |
                (0x4 << I2C_TIMINGR_SCLDEL_Pos);
    I2C2->CR1 |= I2C_CR1_PE;
}

// Verify I2C Communication with WHO_AM_I
void Verify_I2C_Communication(void) {
    uint32_t timeout;
    uint8_t receivedID;
    uint8_t deviceAddress = I2C_Get_Address(); // Auto-detect address

    // Indicate start of communication
    GPIOC->ODR |= (1 << 9); // Green ON
    HAL_Delay(2000);
    GPIOC->ODR &= ~(1 << 9);

    // Write phase
    I2C2->CR2 = (deviceAddress << 1) | (1 << 16) | I2C_CR2_START;
    timeout = TIMEOUT_LIMIT;
    while (!(I2C2->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)) && --timeout);
    if (I2C2->ISR & I2C_ISR_NACKF || timeout == 0) {
        I2C2->ICR |= I2C_ICR_NACKCF;
        while (1) {
            GPIOC->ODR ^= (1 << 6);  // Red BLINKS (Write Fail)
            HAL_Delay(500);
        }
    }
    I2C2->TXDR = WHO_AM_I_REG;
    timeout = TIMEOUT_LIMIT;
    while (!(I2C2->ISR & I2C_ISR_TC) && --timeout);
    if (timeout == 0) {
        while (1) {
            GPIOC->ODR ^= (1 << 6);  // Red BLINKS (Timeout)
            HAL_Delay(500);
        }
    }
    I2C2->CR2 |= I2C_CR2_STOP;

    // Read phase
    I2C2->CR2 = (deviceAddress << 1) | (1 << 16) | I2C_CR2_RD_WRN | I2C_CR2_START;
    timeout = TIMEOUT_LIMIT;
    while (!(I2C2->ISR & (I2C_ISR_RXNE | I2C_ISR_NACKF)) && --timeout);
    if (I2C2->ISR & I2C_ISR_NACKF || timeout == 0) {
        while (1) {
            GPIOC->ODR ^= (1 << 8);  // Orange BLINKS (Read Fail)
            HAL_Delay(500);
        }
    }
    receivedID = I2C2->RXDR;
    timeout = TIMEOUT_LIMIT;
    while (!(I2C2->ISR & I2C_ISR_TC) && --timeout);
    if (timeout == 0) {
        while (1) {
            GPIOC->ODR ^= (1 << 8);  // Orange BLINKS (TC Timeout)
            HAL_Delay(500);
        }
    }
    I2C2->CR2 |= I2C_CR2_STOP;

    // Check received WHO_AM_I value
    if (receivedID == EXPECTED_ID_1 || receivedID == EXPECTED_ID_2) {
        GPIOC->ODR |= (1 << 9);  // Green ON (Success)
    } else {
        while (1) {
            GPIOC->ODR ^= (1 << 6);  // Red BLINKS (Wrong ID)
            HAL_Delay(500);
        }
    }

    while (1);
}

// Main function
int lab5_main_part1(void) {
    SystemClock_Config();
    GPIO_LED_Init();
    GPIO_I2C_Init();
    I2C2_Reset();  // Reset I2C before initializing
    I2C2_Init();
    GPIOC->ODR |= (1 << 8); // Orange LED ON initially
    Verify_I2C_Communication();
    return 0;
}

int lab5_main_part2(void) {
    return 0;
}
