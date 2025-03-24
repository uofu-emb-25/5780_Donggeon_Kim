#include "stm32f072xb.h"
#include "system_setup.h"
#include "stm32f0xx_hal.h"
#include "lab5.h"

extern void SystemClock_Config(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();

    // Run the complete Lab 5 checkoff sequence
    lab5_checkoff_final();

    while (1) {
        // Infinite loop to keep the program running
    }

    return 0;
}
