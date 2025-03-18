#ifndef LAB5_H
#define LAB5_H
#include <stdint.h> 
// Lab 5 Main Functions
int lab5_main_part1(void);
int lab5_main_part2(void);



#define WHO_AM_I_REG         0x0F
#define EXPECTED_ID_1        0xD3  // WHO_AM_I expected response 1
#define EXPECTED_ID_2        0xD4  // WHO_AM_I expected response 2
#define TIMEOUT_LIMIT        1000000
#define I2C_SLAVE_ADDRESS    0x69  // Default I2C address

#define LED_RED     (1 << 6)  // PC6
#define LED_BLUE    (1 << 7)  // PC7
#define LED_ORANGE  (1 << 8)  // PC8
#define LED_GREEN   (1 << 9)  // PC9

void GPIO_LED_Init(void);          // Initialize GPIO for LEDs
void GPIO_I2C_Init(void);          // Initialize GPIO for I2C
void I2C2_Reset(void);             // Reset I2C Peripheral
void I2C2_Init(void);              // Initialize I2C Peripheral
uint8_t I2C_Read_WHO_AM_I(void);   // Read WHO_AM_I Register
void Verify_I2C_Communication(void); // Verify I2C Communication
void  Gyro_Reset(void);



#endif
