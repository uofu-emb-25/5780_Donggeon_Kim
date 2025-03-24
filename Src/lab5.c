//lab5 - done with help with gpt 4o
#include <stdint.h>
#include <stdlib.h>
#include "stm32f072xb.h"
#include "system_setup.h"
#include "stm32f0xx_hal.h"
#include "lab5.h"

// I2C Slave Address (Gyroscope)
#define I2C_SLAVE_ADDRESS 0x69

// WHO_AM_I Register Address and Expected Response
#define WHO_AM_I_REG  0x0F
#define EXPECTED_ID_1 0xD3
#define EXPECTED_ID_2 0xD4

// Timeout Limit
#define TIMEOUT_LIMIT 1000000

// LED Definitions for Port C
#define LED_RED    (1 << 6)  // PC6
#define LED_BLUE   (1 << 7)  // PC7
#define LED_ORANGE (1 << 8)  // PC8
#define LED_GREEN  (1 << 9)  // PC9

void GPIO_LED_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock

    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); 
    GPIOC->MODER |= ((1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));

    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    GPIOC->ODR &= ~(LED_RED | LED_BLUE | LED_ORANGE | LED_GREEN);
}

// Configure GPIO for I2C
void GPIO_I2C_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Enable GPIOB clock

    // Configure PB11 (SDA) and PB13 (SCL) as alternate function Open-Drain
    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));
    GPIOB->MODER |= ((2 << (11 * 2)) | (2 << (13 * 2)));  // Alternate function mode

    GPIOB->OTYPER |= (1 << 11) | (1 << 13); // Open-drain mode
    GPIOB->PUPDR |= ((1 << (11 * 2)) | (1 << (13 * 2))); // Enable pull-ups

    GPIOB->AFR[1] |= (1 << ((11 - 8) * 4)) | (1 << ((13 - 8) * 4)); // Set I2C2 Alternate Function AF1

    // Set PC0 for SPI/I2C mode selection (push-pull)
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (1 << (0 * 2));
    GPIOC->OTYPER &= ~(1 << 0);
    GPIOC->ODR |= (1 << 0);
}

// Reset I2C
void I2C2_Reset(void) {
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;  
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;  
}

// Initialize I2C
void I2C2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;  // Enable I2C2 Clock
    
    // Configure I2C Timing for 100kHz Standard Mode
    I2C2->TIMINGR = 0x10420F13;  

    I2C2->CR1 |= I2C_CR1_PE;  // Enable I2C2
}

// Read WHO_AM_I Register from Gyroscope
uint8_t I2C_Read_WHO_AM_I(void) {
    uint8_t id;

    // Configure I2C2 for reading the WHO_AM_I register
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_START | I2C_CR2_RD_WRN;

    uint32_t timeout = TIMEOUT_LIMIT;
    while (!(I2C2->ISR & I2C_ISR_RXNE) && --timeout);
    if (timeout == 0) return 0xFF;  // Timeout Error

    id = I2C2->RXDR;
    I2C2->CR2 |= I2C_CR2_STOP;  // Stop the transaction
    return id;
}

// Verify I2C Communication (Check WHO_AM_I)
void Verify_I2C_Communication(void) {
    GPIOC->ODR |= LED_ORANGE;  // Indicate Processing

    uint8_t receivedID = I2C_Read_WHO_AM_I();

    if (receivedID == EXPECTED_ID_1 || receivedID == EXPECTED_ID_2) {
        GPIOC->ODR &= ~LED_ORANGE;  // Turn Off Orange LED
        GPIOC->ODR |= LED_GREEN;    // Success: Turn ON Green LED
    } else {
        GPIOC->ODR &= ~LED_ORANGE;
        GPIOC->ODR |= LED_BLUE;  // Failure: Turn ON Blue LED

        while (1) {
            GPIOC->ODR ^= LED_BLUE;  // Blink Blue LED
            HAL_Delay(500);
        }
    }
}

// Process Gyroscope Data
void Process_Gyro_Data(void) {
    uint8_t data[4];
    int16_t x_value, y_value;
    int prev_led = -1;

    while (1) {
        // Read X and Y data
        I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (4 << 16) | I2C_CR2_START | I2C_CR2_RD_WRN;
        
        while (!(I2C2->ISR & I2C_ISR_RXNE));
        data[0] = I2C2->RXDR;
        while (!(I2C2->ISR & I2C_ISR_RXNE));
        data[1] = I2C2->RXDR;
        while (!(I2C2->ISR & I2C_ISR_RXNE));
        data[2] = I2C2->RXDR;
        while (!(I2C2->ISR & I2C_ISR_RXNE));
        data[3] = I2C2->RXDR;
        
        I2C2->CR2 |= I2C_CR2_STOP;

        x_value = (int16_t)((data[1] << 8) | data[0]);
        y_value = (int16_t)((data[3] << 8) | data[2]);

        int new_led = -1;
        if (abs(x_value) > 400 || abs(y_value) > 400) {
            new_led = (x_value > y_value) ? LED_GREEN : LED_RED;
        }

        if (new_led != prev_led) {
            GPIOC->ODR &= ~(LED_RED | LED_BLUE | LED_ORANGE | LED_GREEN);
            if (new_led != -1) {
                GPIOC->ODR |= new_led;
            }
            prev_led = new_led;
        }

        HAL_Delay(100);
    }
}

// Run the full Lab 5 Checkoff
void lab5_checkoff_final(void) {
    GPIO_LED_Init();
    GPIO_I2C_Init();
    I2C2_Reset();
    I2C2_Init();
    Verify_I2C_Communication();
    Process_Gyro_Data();
}
