#ifndef LAB5_H
#define LAB5_H

#include <stdint.h>

// Lab 5 Main Functions
int lab5_main_part1(void);

// I2C Addresses and Constants
#define WHO_AM_I_REG         0x0F
#define EXPECTED_ID_1        0xD3  
#define EXPECTED_ID_2        0xD4  
#define TIMEOUT_LIMIT        1000000
#define I2C_SLAVE_ADDRESS    0x6B  

// LED Definitions
#define LED_RED     (1 << 6)  
#define LED_BLUE    (1 << 7)  
#define LED_ORANGE  (1 << 8)  
#define LED_GREEN   (1 << 9)  

// Function Prototypes
void GPIO_LED_Init(void);
void GPIO_I2C_Init(void);
void I2C2_Reset(void);
void I2C2_Init(void);
uint8_t I2C_Read_WHO_AM_I(void);
void Verify_I2C_Communication(void);

#endif
