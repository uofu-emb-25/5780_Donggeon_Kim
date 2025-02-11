#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"  // or stm32f0xx.h
void delay_ms(uint32_t ms);


// Clock enable functions
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void);

// Basic GPIO initialization with mode (1 = Output, 0 = Input with pull-down)
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode);

// Convenience functions for output and input initialization:
void MY_HAL_GPIO_Init_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void MY_HAL_GPIO_Init_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

// GPIO write, toggle, and read
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

// EXTI initialization for PA0
void MY_HAL_GPIO_Init_EXTI_PA0(void);

// Simple blocking delay
void delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif // HAL_GPIO_H
