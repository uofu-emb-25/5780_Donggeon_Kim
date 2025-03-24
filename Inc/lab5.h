#ifndef LAB5_H
#define LAB5_H

#include <stdint.h>
#include "stm32f072xb.h"

// Function Prototypes
void GPIO_LED_Init(void);
void GPIO_I2C_Init(void);
void I2C2_Reset(void);
void I2C2_Init(void);
uint8_t I2C_Read_WHO_AM_I(void);
void Verify_I2C_Communication(void);
void Process_Gyro_Data(void);
void lab5_checkoff_final(void);

#endif // LAB5_H
