//lab5 - done with help with gpt 4o
#include "lab5.h"
#include "stm32f0xx_hal.h"

extern void SystemClock_Config(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    lab5_checkoff_final();
    while (1);
}

