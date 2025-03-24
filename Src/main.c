#include "stm32f0xx_hal.h"
#include "lab5.h"

int main(void) {
    HAL_Init();
    SystemClock_Config();
    lab5_checkoff_final();
    while (1) {}
}
