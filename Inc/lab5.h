#ifndef LAB5_H
#define LAB5_H

#include "stm32f0xx_hal.h"

// LED Definitions (Port C)
#define LED_RED    (1 << 6)   // PC6 - Error
#define LED_BLUE   (1 << 7)   // PC7 - Default/Idle
#define LED_ORANGE (1 << 8)   // PC8 - X-axis Tilt
#define LED_GREEN  (1 << 9)   // PC9 - Y-axis Tilt

// Gyroscope I2C Address and Register Definitions
#define GYRO_I2C_ADDR   0x69  
#define WHO_AM_I_REG    0x0F
#define CTRL_REG1       0x20  // Control Register 1
#define OUT_X_L         0x28  // X-axis data low byte
#define OUT_X_H         0x29  // X-axis data high byte
#define OUT_Y_L         0x2A  // Y-axis data low byte
#define OUT_Y_H         0x2B  // Y-axis data high byte
#define EXPECTED_ID_1   0xD3  // Valid WHO_AM_I response
#define EXPECTED_ID_2   0xD4  // Another valid response

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
