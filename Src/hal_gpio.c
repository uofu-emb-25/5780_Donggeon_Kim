#include <stdint.h>
#include "stm32f072xb.h"
#include <assert.h>
#include "stm32f0xx_hal.h" // For HAL_Delay, etc. if you want
#include "hal_gpio.h"

/************************************************
 * 1) Common GPIO Functions
 ************************************************/
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 
}

void MY_HAL_RCC_GPIOA_CLK_ENABLE(void)
{
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 
}

void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode)
{
    if (Mode == 1) { // Output
        GPIOx->MODER &= ~(0x3 << (GPIO_Pin * 2)); 
        GPIOx->MODER |=  (0x1 << (GPIO_Pin * 2)); 
        GPIOx->OTYPER &= ~(0x1 << GPIO_Pin);
        GPIOx->OSPEEDR &= ~(0x3 << (GPIO_Pin * 2));
        GPIOx->PUPDR   &= ~(0x3 << (GPIO_Pin * 2));
    } else { // Input with pull-down
        GPIOx->MODER &= ~(0x3 << (GPIO_Pin * 2));
        GPIOx->PUPDR &= ~(0x3 << (GPIO_Pin * 2));
        GPIOx->PUPDR |=  (0x2 << (GPIO_Pin * 2));  // Pull-down enabled
    }
}


void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState)
{
    if (PinState)
        GPIOx->BSRR = (1 << GPIO_Pin);
    else
        GPIOx->BRR = (1 << GPIO_Pin);
}

void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= (1 << GPIO_Pin);
}

uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    return (GPIOx->IDR & (1 << GPIO_Pin)) ? 1 : 0;
}

/************************************************
 * 2) EXTI Functions
 ************************************************/

 void MY_HAL_GPIO_Init_EXTI_PA0(void)
 {
     // Enable GPIOA clock
     MY_HAL_RCC_GPIOA_CLK_ENABLE();
 
     // Configure PA0 as input with pull-down
     GPIOA->MODER &= ~(3 << (0 * 2));  
     GPIOA->PUPDR &= ~(3 << (0 * 2));
     GPIOA->PUPDR |=  (2 << (0 * 2));
 
     // Enable SYSCFG clock for EXTI configuration
     RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
 
     // Map EXTI0 to PA0
     SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4));
     SYSCFG->EXTICR[0] |=  (0x0 << (0 * 4));
 
     // Configure EXTI for rising edge trigger only
     EXTI->RTSR |= (1 << 0);
     EXTI->FTSR &= ~(1 << 0);
 
     // Unmask EXTI line 0
     EXTI->IMR |= (1 << 0);
 
     // Enable the EXTI0_1 IRQ in NVIC and set its priority
     NVIC_EnableIRQ(EXTI0_1_IRQn);
     NVIC_SetPriority(EXTI0_1_IRQn, 1);
 }

void MY_HAL_GPIO_Init_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    MY_HAL_GPIO_Init(GPIOx, GPIO_Pin, 0);
}


void MY_HAL_GPIO_Init_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    MY_HAL_GPIO_Init(GPIOx, GPIO_Pin, 1);
}




/************************************************
 *  A "master" main if you want
 ************************************************/
// #define LAB1
// #define LAB2
#ifdef LAB1
int main(void)
{
    return lab1_main();
}
#elif defined(LAB2)
int main(void)
{
    return lab2_main();
}
#endif
