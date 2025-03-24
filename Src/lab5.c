#include "lab5.h"

// Timeout threshold for I2C operations
#define TIMEOUT_LIMIT 1000000

void GPIO_LED_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2)) | (3 << (8 * 2)) | (3 << (9 * 2)));
    GPIOC->MODER |= ((1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2)));

    GPIOC->OTYPER &= ~((1 << 6) | (1 << 7) | (1 << 8) | (1 << 9));
    GPIOC->ODR &= ~(LED_RED | LED_BLUE | LED_ORANGE | LED_GREEN);
}

void GPIO_I2C_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;

    GPIOB->MODER &= ~((3 << (11 * 2)) | (3 << (13 * 2)));
    GPIOB->MODER |= ((2 << (11 * 2)) | (2 << (13 * 2)));

    GPIOB->OTYPER |= (1 << 11) | (1 << 13);  // Open-drain
    GPIOB->PUPDR &= ~((3 << (11 * 2)) | (3 << (13 * 2))); // Clear
    GPIOB->PUPDR |= ((1 << (11 * 2)) | (1 << (13 * 2)));  // Enable pull-up

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

void Activate_Gyro(void) {
    uint8_t activationCommand = 0x0F;
    Communicate_With_Gyro(GYRO_I2C_ADDRESS, 1, (int*)&activationCommand, 0, 0x20);
    HAL_Delay(100);
}

uint8_t Read_Gyro_ID(void) {
    uint8_t id;
    Communicate_With_Gyro(GYRO_I2C_ADDRESS, 1, (int*)&id, 1, GYRO_ID_REGISTER);
    return id;
}

void Verify_Gyro(void) {
    GPIOC->ODR |= LED_ORANGE;
    uint8_t receivedID = Read_Gyro_ID();

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

int WaitFor_I2C_Flag(int flag) {
    uint32_t timeout = TIMEOUT_LIMIT;
    while (!(I2C2->ISR & (1 << flag)) && --timeout);

    if (timeout == 0 || (I2C2->ISR & (1 << 4))) {
        GPIOC->ODR |= LED_RED;
        return 0;
    }
    return 1;
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
    Activate_Gyro();
    Verify_Gyro();
    Process_Gyro_Data();
}

void Communicate_With_Gyro(int address, int numBytes, int *buffer, int mode, int regAddress) {
    const uint32_t BYTE_COUNT_POS = 16;
    const uint32_t DEVICE_ADDR_POS = 1;
    const uint32_t INC_FLAG = 0x80;

    I2C2->CR2 &= ~((0xFF << BYTE_COUNT_POS) | (0x3FF) | (1U << 10));

    if (mode) {
        I2C2->CR2 |= (1U << BYTE_COUNT_POS) | (address << DEVICE_ADDR_POS);
    } else {
        I2C2->CR2 |= ((numBytes + 1) << BYTE_COUNT_POS) | (address << DEVICE_ADDR_POS);
    }

    I2C2->CR2 |= (1U << 13);

    if (WaitFor_I2C_Flag(1)) {
        I2C2->TXDR = regAddress | ((numBytes > 1) ? INC_FLAG : 0);

        if (mode) {
            while (!(I2C2->ISR & (1U << 6)));
            I2C2->CR2 &= ~((0xFF << BYTE_COUNT_POS) | (0x3FF));
            I2C2->CR2 |= (numBytes << BYTE_COUNT_POS) | (address << DEVICE_ADDR_POS) | (1U << 10);
            I2C2->CR2 |= (1U << 13);

            for (int i = 0; i < numBytes; i++) {
                while (!WaitFor_I2C_Flag(2));
                buffer[i] = I2C2->RXDR;
            }
        } else {
            for (int i = 0; i < numBytes; i++) {
                while (!WaitFor_I2C_Flag(1));
                I2C2->TXDR = buffer[i];
            }
        }
    }

    while (!(I2C2->ISR & (1U << 6)));
    I2C2->CR2 |= (1U << 14);
}
