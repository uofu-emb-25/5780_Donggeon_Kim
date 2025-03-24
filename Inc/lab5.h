#ifndef LAB5_H
#define LAB5_H

#include <stdint.h>
#include "stm32f0xx_hal.h"

// LED Definitions
#define LED_RED    (1 << 6)  // PC6
#define LED_BLUE   (1 << 7)  // PC7
#define LED_ORANGE (1 << 8)  // PC8
#define LED_GREEN  (1 << 9)  // PC9

// I2C Gyroscope Definitions
#define I2C_GYRO_ADDR  0x69  // Change to 0x68 if needed
#define WHO_AM_I_REG   0x0F
#define EXPECTED_ID_1  0xD3
#define EXPECTED_ID_2  0xD4

// Function Prototypes
void GPIO_LED_Init(void);
void GPIO_I2C_Init(void);
void I2C2_Reset(void);
void I2C2_Init(void);
void Enable_Gyroscope(void);
uint8_t I2C_Read_WHO_AM_I(void);
void Verify_I2C_Communication(void);
void Process_Gyro_Data(void);
void lab5_checkoff_final(void);
void gyroTransfer(int address, int numBytes, int *dataBuffer, int readMode, int registerAddress);
int waitForI2CFlag(int flag);

#endif // LAB5_H
