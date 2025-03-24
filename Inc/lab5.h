#ifndef LAB5_H
#define LAB5_H

#include "stm32f0xx_hal.h"

// LED Definitions
#define LED_RED    (1 << 6)
#define LED_BLUE   (1 << 7)
#define LED_ORANGE (1 << 8)
#define LED_GREEN  (1 << 9)

// Gyroscope I2C Address and Register Definitions
#define GYRO_I2C_ADDR  0x69  
#define WHO_AM_I_REG   0x0F
#define EXPECTED_ID_1  0xD3
#define EXPECTED_ID_2  0xD4

// Function Prototypes
void Init_LEDs(void);
void Init_I2C_GPIO(void);
void Reset_I2C(void);
void Setup_I2C(void);
void Enable_Gyro(void);
uint8_t Get_Gyro_ID(void);
void Validate_Gyro(void);
int Await_I2C_Flag(int flag);
void Process_Gyro(void);
void lab5_checkoff_final(void);
void Transfer_I2C(int address, int numBytes, int *buffer, int isRead, int regAddress);

#endif // LAB5_H
