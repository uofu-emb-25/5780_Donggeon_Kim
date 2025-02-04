//done by help of gpt
#include <stdint.h>
#include "stm32f072xb.h"
#include <assert.h>

#include <assert.h>


//for given code oragne and green
#include <stm32f0xx.h>
/*
void My_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock
    assert(RCC->AHBENR & RCC_AHBENR_GPIOCEN); // Verify clock is enabled
}
//given example
void My_HAL_GPIO_Init(void) {
    // Configure PC8 and PC9 as General Purpose Output Mode
    GPIOC->MODER &= ~((3U << (8 * 2)) | (3U << (9 * 2))); // Clear bits
    GPIOC->MODER |= ((1U << (8 * 2)) | (1U << (9 * 2)));  // Set as output mode
    assert((GPIOC->MODER & (3U << (8 * 2))) == (1U << (8 * 2)));
    assert((GPIOC->MODER & (3U << (9 * 2))) == (1U << (9 * 2)));

    // Configure Push-Pull Output Type
    GPIOC->OTYPER &= ~((1U << 8) | (1U << 9));
    assert(!(GPIOC->OTYPER & (1U << 8)));
    assert(!(GPIOC->OTYPER & (1U << 9)));

    // Configure Low Speed Output
    GPIOC->OSPEEDR &= ~((3U << (8 * 2)) | (3U << (9 * 2))); // Clear bits
    GPIOC->OSPEEDR |= ((1U << (8 * 2)) | (1U << (9 * 2)));  // Set low speed
    assert((GPIOC->OSPEEDR & (3U << (8 * 2))) == (1U << (8 * 2)));
    assert((GPIOC->OSPEEDR & (3U << (9 * 2))) == (1U << (9 * 2)));

    // No Pull-up / Pull-down
    GPIOC->PUPDR &= ~((3U << (8 * 2)) | (3U << (9 * 2)));
    assert(!(GPIOC->PUPDR & (3U << (8 * 2))));
    assert(!(GPIOC->PUPDR & (3U << (9 * 2))));
}

void My_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState) {
    if (PinState) {
        GPIOx->BSRR = GPIO_Pin;  // Set pin high
    } else {
        GPIOx->BRR = GPIO_Pin;   // Set pin low
    }
    assert((PinState && (GPIOx->ODR & GPIO_Pin)) || (!PinState && !(GPIOx->ODR & GPIO_Pin)));
}

void My_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
    GPIOx->ODR ^= GPIO_Pin;  // Toggle pin
    assert(GPIOx->ODR & GPIO_Pin);  // Ensure the pin toggled
}

int lab1_main(void) {
    My_HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock
    My_HAL_GPIO_Init();  // Initialize GPIOC pins 8 and 9

    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1); // Set PC8 high initially
    My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0); // Set PC9 low initially

    while (1) {
        HAL_Delay(200); // Delay 200ms
        My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9); // Toggle PC8 & PC9
    }
}
////
/*
part1 bymyself
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

// part2 bymyself

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




//lab2_ part 1
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // Enable GPIOC clock
}

void MY_HAL_RCC_GPIOA_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // Enable GPIOA clock
}


//2.2 Configuring the EXTI.
void MY_HAL_GPIO_Init_EXTI_PA0(void) {
    // Enable GPIOA Clock (If not already enabled)
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;

    // Configure PA0 as Input (00)
    GPIOA->MODER &= ~(3 << (0 * 2));  // Clear bits (00 = Input mode)

    // Configure Pull-down Resistor (PD: 10)
    GPIOA->PUPDR &= ~(3 << (0 * 2));  // Clear bits
    GPIOA->PUPDR |= (2 << (0 * 2));   // Set to 10 (Pull-down)

    // Enable EXTI for PA0
    SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4)); // Clear EXTI0 bits (Ensure PA0 is mapped)
    SYSCFG->EXTICR[0] |= (0x0 << (0 * 4));  // Map PA0 to EXTI0

    // Unmask EXTI Line 0
    EXTI->IMR |= (1 << 0);

    // Set EXTI to Rising Edge Trigger
    EXTI->RTSR |= (1 << 0);
    EXTI->FTSR &= ~(1 << 0);  // Ensure Falling trigger is disabled

    // Enable EXTI0 Interrupt in NVIC
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 1);  // Set priority (higher than SysTick but not highest)
}


//2.3 Setting the SYSCFG Pin Multiplexer
void MY_HAL_SYSCFG_Config_PA0(void) {
    // Enable SYSCFG Clock (If not already enabled)
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // before Assert 
    //Add an assertion before and after your setup function to verify the register values before and
     // after invocation.
    assert((SYSCFG->EXTICR[0] & (0xF << (0 * 4))) == 0);

    // Configure SYSCFG to route PA0 to EXTI0
    SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4)); // Clear bits for EXTI0
    SYSCFG->EXTICR[0] |= (0x0 << (0 * 4));  // Set PA0 to EXTI0 (0x0 = PA0)

    // after asset
    assert((SYSCFG->EXTICR[0] & (0xF << (0 * 4))) == 0x0);
}
//2.4
void MY_HAL_EXTI_Enable_PA0(void) {
    // Enable NVIC for EXTI0_1
    NVIC_EnableIRQ(EXTI0_1_IRQn);

    // Set priority to 1 (High Priority)
    NVIC_SetPriority(EXTI0_1_IRQn, 1);
}