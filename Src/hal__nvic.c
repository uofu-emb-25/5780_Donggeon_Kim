#include "stm32f0xx.h"
#include "hal_nvic.h"

void NVIC_Config(void) {
    // Enable EXTI0_1 interrupt in NVIC
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 1); // Priority 1 (High)
}