#include "lab5.h"
#include "stm32f0xx_hal.h"

/**
 * @brief Entry point for the program.
 */
int main(void) {
    HAL_Init();  
    SystemClock_Config();  
    return lab5_main();  
}
