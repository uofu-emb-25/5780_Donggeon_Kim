#ifndef LAB5_H
#define LAB5_H

#include <stdint.h>
#include "stm32f0xx_hal.h"

void Configure_SystemClock(void);
int lab5_main(void);
void Transmit_Gyro_Data(int addr, int count, int *buffer, int isRead, int regAddr);
int Check_I2C_Flag(int flag);

#define LED_RED    (1 << 6)
#define LED_BLUE   (1 << 7)
#define LED_ORANGE (1 << 8)
#define LED_GREEN  (1 << 9)

static inline void Set_LED(uint32_t ledPos) {
    GPIOC->ODR = (GPIOC->ODR & ~(LED_RED | LED_BLUE | LED_ORANGE | LED_GREEN)) | (1 << ledPos);
}

#endif  // LAB5_H
