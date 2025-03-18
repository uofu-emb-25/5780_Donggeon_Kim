#ifndef LAB5_H
#define LAB5_H
#include <stdint.h> 
// Lab 5 Main Functions
int lab5_main_part1(void);
int lab5_main_part2(void);

// GPIO and I2C Initialization Functions
void GPIO_I2C_Init(void);             //  I2C GPIO (PB11, PB13)
void GPIO_Control_Signals_Init(void); //  PB14, PC0 for mode select
void GPIO_LED_Init(void);             //  PC6-PC9 for LED status
void I2C2_Init(void);                 //  I2C2 with 100kHz timing
void Lab5_GPIO_Init(void);


// I2C Functions
void I2C2_Init(void);                 // Initialize I2C2 (100kHz timing)
void I2C_Write(uint8_t deviceAddr, uint8_t regAddr);  // Write to I2C device
uint8_t I2C_Read(uint8_t deviceAddr); // Read from I2C device
void Verify_I2C_Communication(void);  // Check I2C response from the sensor
#endif
