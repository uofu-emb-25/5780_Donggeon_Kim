#include "stm32f0xx.h"
#include "hal_exti.h"


void EXTI_Config(void) {
    // Enable system configuration controller clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

    // Map EXTI0 to PA0 (User Button)
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0); // Set to 0000 for PA0

    // Enable EXTI0 interrupt on rising edge
    EXTI->IMR |= EXTI_IMR_IM0;       // Unmask EXTI0
    EXTI->RTSR |= EXTI_RTSR_RT0;     // Enable rising-edge trigger

    NVIC_EnableIRQ(EXTI0_1_IRQn);    // Enable EXTI0_1 interrupt in NVIC
}