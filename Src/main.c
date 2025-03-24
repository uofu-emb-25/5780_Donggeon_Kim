#include "stm32f072xb.h"
#include "system_setup.h"  
#include <stm32f0xx_hal.h>
#include "main.h"
#include "lab5.h"

extern void SystemClock_Config(void);  // System Clock Configuration

// Function prototype
int lab5_main(void);  


int main(void) {
    GPIO_LED_Init();
    while(1);
}
