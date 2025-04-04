#include "lab6.h"
#include "stm32f072xb.h"   // Device-specific CMSIS header
#include "stm32f0xx_hal.h" // HAL library (if using HAL functions)
#include "stm32f0xx.h" 

#include "lab7.h"
int main(void) {

    HAL_Init();           // optional if HAL-based timing is used
    SystemCoreClockUpdate();

    lab7_main();

    while (1) {
        // main loop, all logic is handled by SysTick
    }
    //lab6_checkoff1();  // Run Lab 6 Checkoff 1
    //lab6_checkoff2();  // Run Lab 6 Checkoff 2
    return 0;
}
