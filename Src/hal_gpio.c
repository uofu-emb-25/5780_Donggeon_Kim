#include <stdint.h>
#include "stm32f072xb.h"
#include <assert.h>
#include "stm32f0xx_hal.h" // For HAL_Delay, etc. if you want

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
    } else { // Input w/ pull-down
        GPIOx->MODER &= ~(0x3 << (GPIO_Pin * 2));
        GPIOx->PUPDR &= ~(0x3 << (GPIO_Pin * 2));
        GPIOx->PUPDR |=  (0x2 << (GPIO_Pin * 2));
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

    // Input mode, Pull-down for PA0
    GPIOA->MODER &= ~(3 << (0 * 2));
    GPIOA->PUPDR &= ~(3 << (0 * 2));
    GPIOA->PUPDR |=  (2 << (0 * 2));

    // Enable SYSCFG clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure EXTI0 to be triggered by PA0
    SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4));
    SYSCFG->EXTICR[0] |=  (0x0 << (0 * 4));  // 0 = PA0

    // Rising edge trigger
    EXTI->RTSR |= (1 << 0);
    EXTI->FTSR &= ~(1 << 0);

    // Unmask EXTI line 0
    EXTI->IMR |= (1 << 0);

    // Enable EXTI0_1 IRQ in NVIC
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
 * 3) Lab 1 "Given Example" - PC8 & PC9 blink
 ************************************************/
int lab1_main(void)
{
    HAL_Init(); // for HAL_Delay
    // If you have a system clock config, call it here.

    // Enable GPIOC clock
    MY_HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure PC8 & PC9 as output
    GPIOC->MODER &= ~((3U << (8*2)) | (3U << (9*2)));
    GPIOC->MODER |=  ((1U << (8*2)) | (1U << (9*2)));

    // Start PC8 high, PC9 low
    MY_HAL_GPIO_WritePin(GPIOC, 8, 1);
    MY_HAL_GPIO_WritePin(GPIOC, 9, 0);

    while (1)
    {
        HAL_Delay(200);
        // Toggle both
        MY_HAL_GPIO_TogglePin(GPIOC, 8);
        MY_HAL_GPIO_TogglePin(GPIOC, 9);
    }
    return 0;
}

/************************************************
 * 4) Lab 2 Example - EXTI on PA0, toggle PC6/7
 ************************************************/



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
