#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f0xx_hal.h"

/**
 * @brief Enables GPIOC clock
 */
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);

/**
 * @brief Enables GPIOA clock
 */
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void);

/**
 * @brief Initialize pin as output (Mode=1) or input pull-down (Mode=0).
 * @param  GPIOx  Pointer to GPIO port
 * @param  GPIO_Pin Pin number [0..15]
 * @param  Mode  1 = output, 0 = input pull-down
 */
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode);

// Convenience functions
void MY_HAL_GPIO_Init_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void MY_HAL_GPIO_Init_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

// GPIO write, toggle, read
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

// EXTI initialization for PA0
void MY_HAL_GPIO_Init_EXTI_PA0(void);
void GPIO_Config(void);

// Simple blocking delay
void delay_ms(uint32_t ms);

/**
 * @brief  Configure a GPIO pin to an alternate function.
 *         This sets the pin's MODER to AF mode, configures push-pull & high speed,
 *         and clears + sets the AFR bits for the specified AF number.
 * @param  GPIOx: Pointer to GPIO port (e.g., GPIOA, GPIOB, GPIOC).
 * @param  GPIO_Pin: Pin number (0..15).
 * @param  AF: Alternate function number (0..15).
 * @retval None
 */
void MY_HAL_GPIO_Init_AF(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t AF);

#ifdef __cplusplus
}
#endif

#endif // HAL_GPIO_H
