//lab5 - done with help with gpt 40
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "hal_gpio.h"
#include <stm32f0xx_hal.h>

// System clock configuration prototype
void SystemClock_Config(void);

// Function Prototypes
void Configure_GPIO(void);
void Configure_I2C2(void);
void Configure_LEDs(void);
void Configure_I2C_Pins(void);
void Configure_Peripherals(void);
void Initialize_Gyroscope(void);
void Read_Gyroscope(void);
void Transmit_Gyro_Data(int addr, int count, int *buffer, int isRead, int regAddr);
int Check_I2C_Flag(int flag);

// LED definitions for port C (PC6 - PC9)
#define RED_LED    (1 << 6)   // PC6
#define BLUE_LED   (1 << 7)   // PC7
#define ORANGE_LED (1 << 8)   // PC8
#define GREEN_LED  (1 << 9)   // PC9

// Sensor data buffer
int sensorData[4];

/**
 * @brief Inline function to update LED state (only one LED at a time).
 */
static inline void setLED(uint32_t ledPos) {
    GPIOC->ODR = (GPIOC->ODR & ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9))) | (1 << ledPos);
}

/**
 * @brief Configures GPIO peripherals.
 */
void Configure_GPIO(void) {
    // Enable GPIOB and GPIOC clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    Configure_LEDs();
    Configure_I2C_Pins();
}
#define LED_RED    (1 << 6)   // PC6
#define LED_BLUE   (1 << 7)   // PC7
#define LED_ORANGE (1 << 8)   // PC8
#define LED_GREEN  (1 << 9)   // PC9

/**
 * @brief Configures LED pins (PC6 - PC9) as outputs.
 */
void Configure_LEDs(void) {
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOC->MODER |= ((1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));
    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    GPIOC->ODR &= ~(LED_RED | LED_BLUE | LED_ORANGE | LED_GREEN);
}

/**
 * @brief Configures I2C2-related GPIO pins.
 */
void Configure_I2C_Pins(void) {
    // PB11 (SDA)
    GPIOB->MODER &= ~(3 << (11 * 2));
    GPIOB->MODER |= (2 << (11 * 2)); // Alternate function
    GPIOB->OTYPER |= (1 << 11); // Open-drain
    GPIOB->PUPDR |= (1 << (11 * 2)); // Pull-up
    GPIOB->AFR[1] |= (1 << ((11 - 8) * 4)); // AF1

    // PB13 (SCL)
    GPIOB->MODER &= ~(3 << (13 * 2));
    GPIOB->MODER |= (2 << (13 * 2)); // Alternate function
    GPIOB->OTYPER |= (1 << 13); // Open-drain
    GPIOB->PUPDR |= (1 << (13 * 2)); // Pull-up
    GPIOB->AFR[1] |= (1 << ((13 - 8) * 4)); // AF1
}

/**
 * @brief Configures and enables I2C2 peripheral.
 */
void Configure_I2C2(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

    I2C2->TIMINGR = 0x13 | (0xF << 8) | (0x2 << 16) | (0x4 << 20) | (1 << 28);

    I2C2->CR1 |= (1 << 0); // Enable I2C2 peripheral
}

/**
 * @brief Calls all peripheral configurations.
 */
void Configure_Peripher als(void) {
    HAL_Init();
    SystemClock_Config();
    Configure_GPIO();
    Configure_I2C2();
}

/**
 * @brief Initializes the gyroscope.
 */
void Initialize_Gyroscope(void) {
    int ctrlValue = 11;
    Transmit_Gyro_Data(0x69, 1, &ctrlValue, 0, 0x20);
}

/**
 * @brief Reads data from the gyroscope and updates the LEDs accordingly.
 */
void Read_Gyroscope(void) {
    Transmit_Gyro_Data(0x69, 4, sensorData, 1, 0x28);

    int16_t xVal = (sensorData[1] << 8) | sensorData[0];
    int16_t yVal = (sensorData[3] << 8) | sensorData[2];

    if (yVal >= 500) {
        if (xVal >= 500)
            setLED((xVal > yVal) ? 9 : 6);
        else if (xVal <= -500)
            setLED((abs(xVal) > yVal) ? 8 : 6);
    } else if (yVal <= -500) {
        if (xVal <= -500)
            setLED((abs(xVal) > abs(yVal)) ? 8 : 7);
        else if (xVal >= 500)
            setLED((xVal > abs(yVal)) ? 9 : 7);
    }
}

/**
 * @brief Main application entry point.
 */
int lab5_main(void) {
    Configure_Peripherals();
    Initialize_Gyroscope();

    while (1) {
        HAL_Delay(100);
        Read_Gyroscope();
    }
}

/**
 * @brief Waits for a specific I2C2 flag or an error (NACK) flag.
 */
int Check_I2C_Flag(int flag) {
    while (!(I2C2->ISR & (1 << 4)) && !(I2C2->ISR & (1 << flag))) {}

    if (I2C2->ISR & (1 << 4)) {
        GPIOC->ODR |= LED_RED;  // Error: light red LED (PC6)
        return 0;
    }
    return 1;
}

/**
 * @brief Performs an I2C2 transaction with the gyroscope.
 */
void Transmit_Gyro_Data(int addr, int count, int *buffer, int isRead, int regAddr) {
    const uint32_t NUM_BYTES_POS = 16;
    const uint32_t SLAVE_ADDR_POS = 1;
    const uint32_t AUTO_INCREMENT = 128;

    I2C2->CR2 &= ~((0xFF << NUM_BYTES_POS) | (0x3FF) | (1 << 10));

    if (isRead) {
        I2C2->CR2 |= (1 << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS);
    } else {
        I2C2->CR2 |= ((count + 1) << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS);
    }

    I2C2->CR2 |= (1 << 13);

    if (Check_I2C_Flag(1)) {
        I2C2->TXDR = regAddr | ((count > 1) ? AUTO_INCREMENT : 0);

        if (isRead) {
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
