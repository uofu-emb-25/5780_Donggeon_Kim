//lab5 - done with help with gpt 4o
#include <stdint.h>
#include <stdlib.h>
#include "main.h"
#include "hal_gpio.h"
#include <stm32f0xx_hal.h>

void SystemClock_Config(void);
void Transmit_Gyro_Data(int addr, int count, int *buffer, int isRead, int regAddr);
int Check_I2C_Flag(int flag);
void Configure_GPIO(void);
void Configure_I2C2(void);
void Initialize_Gyroscope(void);
#define LED_RED    (1 << 6)
#define LED_BLUE   (1 << 7)
#define LED_ORANGE (1 << 8)
#define LED_GREEN  (1 << 9)
void Read_Gyroscope_Data(void);

static inline void Set_LED(uint32_t ledPos) {
    GPIOC->ODR = (GPIOC->ODR & ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9))) | (1 << ledPos);
}

int lab5_main(void) {
    HAL_Init();
    SystemClock_Config();
    
    Configure_GPIO();
    Configure_I2C2();
    Initialize_Gyroscope();

    while (1) {
        HAL_Delay(100);
        Read_Gyroscope_Data();
    }
}
void Read_Gyroscope_Data(void) {
    int sensorData[4];
    Transmit_Gyro_Data(0x69, 4, sensorData, 1, 0x28);
    
    int16_t xVal = (sensorData[1] << 8) | sensorData[0];
    int16_t yVal = (sensorData[3] << 8) | sensorData[2];

    if (yVal >= 500) {
        if (xVal >= 500)
            Set_LED((xVal > yVal) ? 9 : 6);
        else if (xVal <= -500)
            Set_LED((abs(xVal) > yVal) ? 8 : 6);
    } else if (yVal <= -500) {
        if (xVal <= -500)
            Set_LED((abs(xVal) > abs(yVal)) ? 8 : 7);
        else if (xVal >= 500)
            Set_LED((xVal > abs(yVal)) ? 9 : 7);
    }
}

void Configure_GPIO(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;

    GPIOC->MODER |= (1 << 12);
    GPIOC->MODER |= (1 << 14);
    GPIOC->MODER |= (1 << 16);
    GPIOC->MODER |= (1 << 18);

    GPIOB->MODER |= (1 << 23);
    GPIOB->MODER &= ~(1 << 22);
    GPIOB->OTYPER |= (1 << 11);
    GPIOB->AFR[1] |= (1 << 12);
    
    GPIOB->MODER |= (1 << 27);
    GPIOB->MODER &= ~(1 << 26);
    GPIOB->OTYPER |= (1 << 13);
    GPIOB->AFR[1] |= (1 << 20);
    GPIOB->AFR[1] &= ~(1 << 21);
    GPIOB->AFR[1] |= (1 << 22);
    GPIOB->AFR[1] &= ~(1 << 23);
    
    GPIOB->MODER &= ~(1 << 29);
    GPIOB->MODER |= (1 << 28);
    GPIOB->OTYPER &= ~(1 << 14);
    GPIOB->ODR |= (1 << 14);
    
    GPIOC->MODER |= (1 << 0);
    GPIOC->ODR |= (1 << 0);
}

// I2C2 Initialization
void Configure_I2C2(void) {
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
    
    I2C2->TIMINGR = 0;
    I2C2->TIMINGR |= 0x13;
    I2C2->TIMINGR |= (0xF << 8);
    I2C2->TIMINGR |= (0x2 << 16);
    I2C2->TIMINGR |= (0x4 << 20);
    I2C2->TIMINGR |= (1 << 28);
    
    I2C2->CR1 |= (1 << 0);
}

// Gyroscope Initialization
void Initialize_Gyroscope(void) {
    int ctrlValue = 11;
    Transmit_Gyro_Data(0x69, 1, &ctrlValue, 0, 0x20);
}
int Check_I2C_Flag(int flag) {
    while (!(I2C2->ISR & (1 << 4)) && !(I2C2->ISR & (1 << flag))) {}
    if (I2C2->ISR & (1 << 4)) {
        GPIOC->ODR |= LED_RED;
        return 0;
    }
    return 1;
}

void Transmit_Gyro_Data(int addr, int count, int *buffer, int isRead, int regAddr) {
    const uint32_t NUM_BYTES_POS = 16;
    const uint32_t SLAVE_ADDR_POS = 1;
    const uint32_t AUTO_INCREMENT = 128;
    
    I2C2->CR2 &= ~((0xFF << NUM_BYTES_POS) | (0x3FF) | (1 << 10));
    
    if (isRead) {
        I2C2->CR2 |= (1 << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS);
    } else {
        I2C2->CR2 |= ((count + 1) << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS);
    }
    
    I2C2->CR2 |= (1 << 13);
    
    if (Check_I2C_Flag(1)) {
        I2C2->TXDR = regAddr | ((count > 1) ? AUTO_INCREMENT : 0);
        
        if (isRead) {
            while (!(I2C2->ISR & (1 << 6)));
            
            I2C2->CR2 &= ~((0xFF << NUM_BYTES_POS) | (0x3FF));
            I2C2->CR2 |= (count << NUM_BYTES_POS) | (addr << SLAVE_ADDR_POS) | (1 << 10);
            I2C2->CR2 |= (1 << 13);
            
            for (int i = 0; i < count; i++) {
                while (!Check_I2C_Flag(2));
                buffer[i] = I2C2->RXDR;
            }
        } else {
            for (int i = 0; i < count; i++) {
                while (!Check_I2C_Flag(1));
                I2C2->TXDR = buffer[i];
            }
        }
    }
    
    while (!(I2C2->ISR & (1 << 6)));
    I2C2->CR2 |= (1 << 14);
}
