#ifndef LAB5_H
#define LAB5_H

#include "stm32f0xx_hal.h"

// LED Definitions
#define LED_RED    (1 << 6)
#define LED_BLUE   (1 << 7)
#define LED_ORANGE (1 << 8)
#define LED_GREEN  (1 << 9)

// Gyroscope Register Definitions
#define GYRO_I2C_ADDR   0x69  
#define WHO_AM_I_REG    0x0F
#define CTRL_REG1       0x20
#define OUT_X_L         0x28
#define EXPECTED_ID_1   0xD3
#define EXPECTED_ID_2   0xD4

// Function Prototypes
void Configure_GPIO(void);
void Initialize_I2C2(void);
void Setup_Gyroscope(void);
void Read_Gyro_Data(void);
void Transmit_Gyro_Data(int address, int numBytes, int *buffer, int mode, int regAddress);
int Check_I2C_Flag(int flag);
void Check_Gyro_Connection(void);
void lab5_checkoff_final(void);

#endif // LAB5_H
