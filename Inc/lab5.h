#ifndef LAB5_H
#define LAB5_H

#include "stm32f0xx_hal.h"

// LED Definitions
#define LED_RED    (1 << 6)
#define LED_BLUE   (1 << 7)
#define LED_ORANGE (1 << 8)
#define LED_GREEN  (1 << 9)

// Gyroscope I2C Address and Register Definitions
#define GYRO_I2C_ADDRESS  0x68  // Try this instead of 0x69

#define GYRO_ID_REGISTER  0x0F
#define EXPECTED_ID_1     0xD3
#define EXPECTED_ID_2     0xD4

// Function Prototypes
void GPIO_LED_Init(void);
void GPIO_I2C_Init(void);
void I2C2_Reset(void);
void I2C2_Init(void);
void Activate_Gyro(void);
uint8_t Read_Gyro_ID(void);
void Verify_Gyro(void);
int WaitFor_I2C_Flag(int flag);
void Process_Gyro_Data(void);
void lab5_checkoff_final(void);
void Communicate_With_Gyro(int address, int numBytes, int *buffer, int mode, int regAddress);

#endif // LAB5_H
