#include "lab5.h"

// Timeout threshold for I2C operations
#define TIMEOUT_LIMIT 1000000

void GPIO_LED_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock

    // Set PC6 - PC9 as output
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
    gyroTransfer(I2C_GYRO_ADDR, 1, (int*)&powerOn, 0, 0x20);
    HAL_Delay(100);
}

uint8_t I2C_Read_WHO_AM_I(void) {
    uint8_t id;
    gyroTransfer(I2C_GYRO_ADDR, 1, (int*)&id, 1, WHO_AM_I_REG);
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
int waitForI2CFlag(int flag) {
    while (!(I2C2->ISR & (1 << 4)) && !(I2C2->ISR & (1 << flag))) {
        // Busy wait for either the desired flag or an error (NACK)
    }

    if (I2C2->ISR & (1 << 4)) {
        GPIOC->ODR |= LED_RED;  // Error detected, turn on Red LED
        return 0;  // Indicate failure
    }

    return 1;  // Indicate success
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

void gyroTransfer(int address, int numBytes, int *dataBuffer, int readMode, int registerAddress) {
    const uint32_t BYTE_COUNT_POS = 16;
    const uint32_t DEVICE_ADDR_POS = 1;
    const uint32_t INC_FLAG = 0x80;

    I2C2->CR2 &= ~((0xFF << BYTE_COUNT_POS) | (0x3FF) | (1U << 10));

    if (readMode) {
        I2C2->CR2 |= (1U << BYTE_COUNT_POS) | (address << DEVICE_ADDR_POS);
    } else {
        I2C2->CR2 |= ((numBytes + 1) << BYTE_COUNT_POS) | (address << DEVICE_ADDR_POS);
    }

    I2C2->CR2 |= (1U << 13);

    if (waitForI2CFlag(1)) {
        I2C2->TXDR = registerAddress | ((numBytes > 1) ? INC_FLAG : 0);

        if (readMode) {
            while (!(I2C2->ISR & (1U << 6)));
            I2C2->CR2 &= ~((0xFF << BYTE_COUNT_POS) | (0x3FF));
            I2C2->CR2 |= (numBytes << BYTE_COUNT_POS) | (address << DEVICE_ADDR_POS) | (1U << 10);
            I2C2->CR2 |= (1U << 13);

            for (int i = 0; i < numBytes; i++) {
                while (!waitForI2CFlag(2));
                dataBuffer[i] = I2C2->RXDR;
            }
        } else {
            for (int i = 0; i < numBytes; i++) {
                while (!waitForI2CFlag(1));
                I2C2->TXDR = dataBuffer[i];
            }
        }
    }

    while (!(I2C2->ISR & (1U << 6)));
    I2C2->CR2 |= (1U << 14);
}
