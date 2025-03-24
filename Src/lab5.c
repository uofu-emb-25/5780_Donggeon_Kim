#include "lab5.h"
#include "stm32f072xb.h"
#include "stm32f0xx_hal.h"

#define I2C_SLAVE_ADDRESS 0x69  // Try 0x68 if failing
#define WHO_AM_I_REG  0x0F
#define EXPECTED_ID_1 0xD3
#define EXPECTED_ID_2 0xD4
#define TIMEOUT_LIMIT 1000000

#define LED_RED    (1 << 6)  // PC6
#define LED_BLUE   (1 << 7)  // PC7
#define LED_ORANGE (1 << 8)  // PC8
#define LED_GREEN  (1 << 9)  // PC9

void GPIO_LED_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock

    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2))); 
    GPIOC->MODER |= ((1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));

    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    GPIOC->ODR &= ~(LED_RED | LED_BLUE | LED_ORANGE | LED_GREEN);
}

void GPIO_I2C_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 

    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));
    GPIOB->MODER |= ((2 << (11 * 2)) | (2 << (13 * 2)));  

    GPIOB->OTYPER |= (1 << 11) | (1 << 13); 
    GPIOB->PUPDR |= ((1 << (11 * 2)) | (1 << (13 * 2))); 

    GPIOB->AFR[1] |= (1 << ((11 - 8) * 4)) | (1 << ((13 - 8) * 4)); 

    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (1 << (0 * 2));
    GPIOC->OTYPER &= ~(1 << 0);
    GPIOC->ODR |= (1 << 0);
}

void I2C2_Reset(void) {
    RCC->APB1RSTR |= RCC_APB1RSTR_I2C2RST;  
    RCC->APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;  
}

void I2C2_Init(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    I2C2->TIMINGR = 0x10420F13;  
    I2C2->CR1 |= I2C_CR1_PE;  
}

void Enable_Gyroscope(void) {
    uint8_t powerOn = 0x0F;  
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_START;
    while (!(I2C2->ISR & I2C_ISR_TXIS));
    I2C2->TXDR = 0x20;  
    while (!(I2C2->ISR & I2C_ISR_TC));
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_START | I2C_CR2_RD_WRN;
    while (!(I2C2->ISR & I2C_ISR_RXNE));
    I2C2->RXDR = powerOn;
    I2C2->CR2 |= I2C_CR2_STOP;
    HAL_Delay(100);
}

uint8_t I2C_Read_WHO_AM_I(void) {
    uint8_t id;

    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_START;
    while (!(I2C2->ISR & I2C_ISR_TXIS));
    I2C2->TXDR = WHO_AM_I_REG;
    while (!(I2C2->ISR & I2C_ISR_TC));
    
    I2C2->CR2 = (I2C_SLAVE_ADDRESS << 1) | (1 << 16) | I2C_CR2_START | I2C_CR2_RD_WRN;
    while (!(I2C2->ISR & I2C_ISR_RXNE));
    
    id = I2C2->RXDR;
    I2C2->CR2 |= I2C_CR2_STOP;
    
    return id;
}

void Verify_I2C_Communication(void) {
    GPIOC->ODR |= LED_ORANGE;

    uint8_t receivedID = I2C_Read_WHO_AM_I();

    if (receivedID == EXPECTED_ID_1 || receivedID == EXPECTED_ID_2) {
        GPIOC->ODR &= ~LED_ORANGE;
        GPIOC->ODR |= LED_GREEN;
    } else {
        GPIOC->ODR &= ~LED_ORANGE;
        GPIOC->ODR |= LED_BLUE;
        while (1) {
            GPIOC->ODR ^= LED_BLUE;
            HAL_Delay(500);
        }
    }
}

void Process_Gyro_Data(void) {
    while (1) {
        HAL_Delay(100);
    }
}

void lab5_checkoff_final(void) {
    GPIO_LED_Init();
    GPIO_I2C_Init();
    I2C2_Reset();
    HAL_Delay(10);
    I2C2_Init();
    Enable_Gyroscope();
    Verify_I2C_Communication();
    Process_Gyro_Data();
}
