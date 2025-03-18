#include <stdio.h>
#include "stm32f072xb.h"
#include "system_setup.h"
#include "stm32f0xx_hal.h"

#define I2C_SLAVE_ADDRESS  0x6B  // L3GD20 I2C Address (SA0 high)
#define WHO_AM_I_REG       0x0F  // Register to check device identity
#define EXPECTED_ID        0xD4  // Expected value from WHO_AM_I (try 0xD3 if fails)

extern void SystemClock_Config(void);

void GPIO_LED_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOC->MODER |= (1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2));
    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    GPIOC->PUPDR &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOC->ODR &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
}

void GPIO_I2C_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // PB11 (SDA), PB13 (SCL) - AF mode, open-drain
    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));
    GPIOB->MODER |= (2 << (11 * 2)) | (2 << (13 * 2));
    GPIOB->OTYPER |= (1 << 11) | (1 << 13);
    GPIOB->PUPDR |= (1 << (11 * 2)) | (1 << (13 * 2)); // Internal pull-ups
    GPIOB->AFR[1] |= (5 << ((11 - 8) * 4)) | (5 << ((13 - 8) * 4)); // AF5 for I2C2

    // PB14 (SA0), PC0 (CS) - Output, push-pull, high
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOB->MODER &= ~(3 << (14 * 2));
    GPIOB->MODER |= (1 << (14 * 2));
    GPIOC->MODER &= ~(3 << (0 * 2));
    GPIOC->MODER |= (1 << (0 * 2));
    GPIOB->OTYPER &= ~(1 << 14);
    GPIOC->OTYPER &= ~(1 << 0);
    GPIOB->ODR |= (1 << 14); // SA0 high
    GPIOC->ODR |= (1 << 0);  // CS high
}

void I2C2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    I2C2->TIMINGR = 0x10420F13; // 100 kHz @ 8 MHz
    I2C2->CR1 |= I2C_CR1_PE;
}

void Verify_I2C_Communication(void) {
    uint8_t receivedID;

    // Write phase
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_START;
    while (!(I2C2->ISR & (I2C_ISR_TXIS | I2C_ISR_NACKF)));
    if (I2C2->ISR & I2C_ISR_NACKF) {
        I2C2->ICR |= I2C_ICR_NACKCF;
        I2C2->CR2 |= I2C_CR2_STOP;
        GPIOC->ODR |= (1 << 7); // Blue ON (write NACK)
        while (1);
    }
    I2C2->TXDR = WHO_AM_I_REG;
    while (!(I2C2->ISR & I2C_ISR_TC));

    // Read phase with restart
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_RD_WRN | I2C_CR2_START;
    while (!(I2C2->ISR & (I2C_ISR_RXNE | I2C_ISR_NACKF)));
    if (I2C2->ISR & I2C_ISR_NACKF) {
        I2C2->ICR |= I2C_ICR_NACKCF;
        I2C2->CR2 |= I2C_CR2_STOP;
        GPIOC->ODR |= (1 << 6); // Red ON (read NACK)
        while (1);
    }
    receivedID = I2C2->RXDR;
    while (!(I2C2->ISR & I2C_ISR_TC));
    I2C2->CR2 |= I2C_CR2_STOP;

    // Result
    if (receivedID == EXPECTED_ID) {
        GPIOC->ODR &= ~(1 << 8); // Orange OFF
        GPIOC->ODR |= (1 << 9);  // Green ON (success)
    } else {
        GPIOC->ODR |= (1 << 6);  // Red ON (failure)
    }
    while (1);
}

int lab5_main_part1(void) {
    SystemClock_Config();
    GPIO_LED_Init();
    GPIO_I2C_Init();
    I2C2_Init();

    GPIOC->ODR |= (1 << 8); // Orange ON initially
    Verify_I2C_Communication();

    return 0;
}

int lab5_main_part2(void) {
    return 0;
}