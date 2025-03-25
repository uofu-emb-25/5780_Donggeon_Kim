//lab5 - done with help with gpt 4o
#include <stdint.h>
#include <stdlib.h>
#include "lab5.h"
#include "hal_gpio.h"
#include <stm32f0xx_hal.h>

// System clock configuration prototype
void Configure_SystemClock(void);

// Function Prototypes
void Configure_GPIO(void);
void Initialize_I2C2(void);
void Setup_Gyroscope(void);
void Read_Gyro_Data(void);
void Transmit_Gyro_Data(int addr, int count, int *buffer, int mode, int regAddr);
int Check_I2C_Flag(int flag);
void Check_Gyro_Connection(void);

// I2C2 Sensor Data Buffer
int sensorData[4];

// LED Definitions
#define LED_RED    (1 << 6)  // PC6
#define LED_BLUE   (1 << 7)  // PC7
#define LED_ORANGE (1 << 8)  // PC8
#define LED_GREEN  (1 << 9)  // PC9

// Gyro I2C Address
#define GYRO_I2C_ADDR 0x69

// GPIO Configuration
void Configure_GPIO(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    // Configure LEDs (PC6 - PC9) as outputs
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOC->MODER |= ((1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));
    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    GPIOC->ODR &= ~(LED_RED | LED_BLUE | LED_ORANGE | LED_GREEN);

    // Configure PB11 (SDA) and PB13 (SCL) for I2C2 (Alternate Function 1)
    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));
    GPIOB->MODER |= ((2 << (11 * 2)) | (2 << (13 * 2)));
    GPIOB->OTYPER |= (1 << 11) | (1 << 13);
    GPIOB->PUPDR |= ((1 << (11 * 2)) | (1 << (13 * 2))); 
    GPIOB->AFR[1] &= ~((0xF << ((11 - 8) * 4)) | (0xF << ((13 - 8) * 4)));
    GPIOB->AFR[1] |= ((1 << ((11 - 8) * 4)) | (1 << ((13 - 8) * 4)));  
}

// I2C2 Initialization
void Initialize_I2C2(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;  // Enable I2C2 clock

    I2C2->TIMINGR = 0;
    I2C2->TIMINGR |= 0x13 | (0xF << 8) | (0x2 << 16) | (0x4 << 20) | (1 << 28);

    I2C2->CR1 |= (1 << 12); // Enable Analog Filter
    I2C2->CR1 &= ~(1 << 11); // Disable Digital Filter
    I2C2->CR1 |= (1 << 0); // Enable I2C Peripheral
}

// Check if Gyroscope is detected
void Check_Gyro_Connection(void) {
    int who_am_i = 0;
    Transmit_Gyro_Data(GYRO_I2C_ADDR, 1, &who_am_i, 1, WHO_AM_I_REG);

    if (who_am_i == EXPECTED_ID_1 || who_am_i == EXPECTED_ID_2) {
        GPIOC->ODR = LED_GREEN; // Success
    } else {
        GPIOC->ODR = LED_RED; // Failure
        while (1); // Halt execution
    }
}
// Check I2C2 Flag with TimeoutCheck_I2C_Flag
int Check_I2C_Flag(int flag) {
    int timeout = 100000;
    while (!(I2C2->ISR & (1 << flag)) && timeout--) {}

    if (timeout == 0 || (I2C2->ISR & (1 << 4))) { 
        GPIOC->ODR |= LED_RED;  // Turn on red LED for error
        return 0;
    }
    return 1;
}
// Setup Gyroscope
void Setup_Gyroscope(void) {
    int ctrlValue = 0x0F; // Enable X, Y, Z and normal mode
    Transmit_Gyro_Data(GYRO_I2C_ADDR, 1, &ctrlValue, 0, 0x20);
}

// Read Gyro Data
void Read_Gyro_Data(void) {
    Transmit_Gyro_Data(GYRO_I2C_ADDR, 4, sensorData, 1, 0x28);

    int16_t xVal = (sensorData[1] << 8) | sensorData[0];
    int16_t yVal = (sensorData[3] << 8) | sensorData[2];

    if (xVal == 0 && yVal == 0) {
        GPIOC->ODR = LED_RED; // Error condition
    } else {
        if (yVal >= 500) {
            if (xVal >= 500)
                GPIOC->ODR = (xVal > yVal) ? LED_GREEN : LED_RED;
            else if (xVal <= -500)
                GPIOC->ODR = (abs(xVal) > yVal) ? LED_ORANGE : LED_RED;
        } else if (yVal <= -500) {
            if (xVal <= -500)
                GPIOC->ODR = (abs(xVal) > abs(yVal)) ? LED_ORANGE : LED_BLUE;
            else if (xVal >= 500)
                GPIOC->ODR = (xVal > abs(yVal)) ? LED_GREEN : LED_BLUE;
        }
    }
}

// Main Lab Execution
void lab5_checkoff_final(void) {
    Configure_GPIO();
    Initialize_I2C2();
    
    Check_Gyro_Connection();  // Verify Gyroscope
    Setup_Gyroscope();

    while (1) {
        HAL_Delay(100);
        Read_Gyro_Data();
    }
}

// Transmit or Receive Data Over I2C
void Transmit_Gyro_Data(int addr, int count, int *buffer, int mode, int regAddr) {
    const uint32_t NUM_BYTES_POS = 16;
    const uint32_t SLAVE_ADDR_POS = 1;
    const uint32_t INC_FLAG = 0x80;

    I2C2->CR2 &= ~((0xFF << NUM_BYTES_POS) | (0x3FF) | (1 << 10));

    if (mode) {
        I2C2->CR2 |= (1 << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS);
    } else {
        I2C2->CR2 |= ((count + 1) << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS);
    }

    I2C2->CR2 |= (1 << 13);  

    if (Check_I2C_Flag(1)) {
        I2C2->TXDR = regAddr | ((count > 1) ? INC_FLAG : 0);

        if (mode) {
            while (!(I2C2->ISR & (1 << 6)));

            I2C2->CR2 &= ~((0xFF << NUM_BYTES_POS) | (0x3FF));
            I2C2->CR2 |= (count << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS) | (1 << 10);
            I2C2->CR2 |= (1 << 13);

            for (int i = 0; i < count; i++) {
                while (!Check_I2C_Flag(2));
                buffer[i] = I2C2->RXDR;
            }
        } else {
            for (int i = 0; i < count; i++) {
                while (!Check_I2C_Flag(1));
                I2C2->TXDR = buffer[i];
            }
        }
    }

    while (!(I2C2->ISR & (1 << 6)));
    I2C2->CR2 |= (1 << 14);
}
