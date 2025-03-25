#include "lab5.h"
#include "stm32f0xx_hal.h"

// External function for system clock configuration
extern void SystemClock_Config(void);  

/**
 * @brief Entry point for the program.
 */
int main(void) {
    HAL_Init();
    SystemClock_Config();
    
    // Call the main function in lab5.c
    lab5_main();
    
    // This should never be reached, as lab5_main() contains an infinite loop.
    while (1) {}
}
