#include <stdint.h>
#include "stm32f072xb.h"

/*part1 
// Custom GPIO Initialization Function
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    // Set pins to general-purpose output mode (MODER register)
    GPIOx->MODER &= ~(0x3 << (GPIO_Pin * 2)); // Clear existing mode
    GPIOx->MODER |= (0x1 << (GPIO_Pin * 2));  // Set as output mode

    // Set push-pull type (OTYPER register)
    GPIOx->OTYPER &= ~(0x1 << GPIO_Pin);

    // Set low speed (OSPEEDR register)
    GPIOx->OSPEEDR &= ~(0x3 << (GPIO_Pin * 2));

    // No pull-up/down resistors (PUPDR register)
    GPIOx->PUPDR &= ~(0x3 << (GPIO_Pin * 2));
}

// Custom function to Write to a GPIO Pin
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState) {
    if (PinState) {
        GPIOx->BSRR = (1 << GPIO_Pin);  // Set pin
    } else {
        GPIOx->BRR = (1 << GPIO_Pin);   // Reset pin
    }
}

// Custom function to Toggle a GPIO Pin
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIOx->ODR ^= (1 << GPIO_Pin);
}

// Custom function to Read GPIO Pin State
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    return (GPIOx->IDR & (1 << GPIO_Pin)) ? 1 : 0;
}
*/

// part2 
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode) {
    if (Mode == 1) { // Output mode
        GPIOx->MODER &= ~(0x3 << (GPIO_Pin * 2)); // Clear existing mode
        GPIOx->MODER |= (0x1 << (GPIO_Pin * 2));  // Set as output mode
        GPIOx->OTYPER &= ~(0x1 << GPIO_Pin);       // Push-pull type
        GPIOx->OSPEEDR &= ~(0x3 << (GPIO_Pin * 2));// Low speed
        GPIOx->PUPDR &= ~(0x3 << (GPIO_Pin * 2));  // No pull-up/down
    } else { // Input mode
        GPIOx->MODER &= ~(0x3 << (GPIO_Pin * 2)); // Set as input mode
        GPIOx->OSPEEDR &= ~(0x3 << (GPIO_Pin * 2)); // Low speed
        GPIOx->PUPDR &= ~(0x3 << (GPIO_Pin * 2)); // Clear pull settings
        GPIOx->PUPDR |= (0x2 << (GPIO_Pin * 2));  // Enable pull-down
    }
}

// Function to Write to a GPIO Pin
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState) {
    if (PinState) {
        GPIOx->BSRR = (1 << GPIO_Pin);  // Set pin
    } else {
        GPIOx->BRR = (1 << GPIO_Pin);   // Reset pin
    }
}

// Function to Toggle a GPIO Pin
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIOx->ODR ^= (1 << GPIO_Pin);
}

// Function to Read GPIO Pin State
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    return (GPIOx->IDR & (1 << GPIO_Pin)) ? 1 : 0;
}