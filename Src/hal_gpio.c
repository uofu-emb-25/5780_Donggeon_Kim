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
    // Clear MODER bits first
    GPIOx->MODER &= ~(0x3 << (GPIO_Pin * 2));  

    if (Mode == 1) { // Output mode
        GPIOx->MODER |=  (0x1 << (GPIO_Pin * 2)); 
        GPIOx->OTYPER &= ~(0x1 << GPIO_Pin);  // Push-pull mode
        GPIOx->OSPEEDR &= ~(0x3 << (GPIO_Pin * 2));  // Low speed
        GPIOx->PUPDR &= ~(0x3 << (GPIO_Pin * 2)); // No pull-up/down
    } 
    else { // Input with Pull-Down
        GPIOx->PUPDR &= ~(0x3 << (GPIO_Pin * 2));  
        GPIOx->PUPDR |=  (0x2 << (GPIO_Pin * 2));  // Enable pull-down
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
//lab2 parts
// Enable SYSCFG clock and configure EXTI for PA0
void MY_HAL_SYSCFG_Config_PA0(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;  // Enable SYSCFG clock
    SYSCFG->EXTICR[0] &= ~(0xF << (0 * 4)); // Clear EXTI0 config
    SYSCFG->EXTICR[0] |= (0x0 << (0 * 4));  // Map EXTI0 to PA0
}

// Enable EXTI0 for PA0
void MY_HAL_EXTI_Enable_PA0(void) {
    EXTI->IMR |= (1 << 0);  // Unmask EXTI0
    EXTI->RTSR |= (1 << 0); // Enable Rising Edge Trigger
    EXTI->FTSR &= ~(1 << 0); // Disable Falling Edge Trigger
    NVIC_EnableIRQ(EXTI0_1_IRQn); // Enable EXTI0_1 interrupt in NVIC
    NVIC_SetPriority(EXTI0_1_IRQn, 1);
}

//GPIO Pin
void GPIO_Config(void) {
    // Enable GPIOA and GPIOC clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;

    // Configure PA0 as input with pull-down resistor (User Button)
    GPIOA->MODER &= ~(3 << (0 * 2)); // Input mode
    GPIOA->PUPDR |= (2 << (0 * 2));  // Pull-down resistor

    // Configure PC6, PC7, PC8, PC9 as outputs (LEDs)
    GPIOC->MODER |= (1 << (6 * 2)) | (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2));
}

//for lab3 3.6
void MY_HAL_GPIO_Init_AF(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t AF) {
    // 1. Configure the pin to Alternate Function mode
    GPIOx->MODER &= ~(3 << (GPIO_Pin * 2));  
    GPIOx->MODER |=  (2 << (GPIO_Pin * 2));  

    // 2. Output type = Push-Pull
    GPIOx->OTYPER &= ~(1 << GPIO_Pin);

    // 3. Speed = High
    GPIOx->OSPEEDR |= (3 << (GPIO_Pin * 2));

    // 4. Clear old AF bits, then set new AF bits
    if (GPIO_Pin < 8) {
        // Clear bits
        GPIOx->AFR[0] &= ~(0xF << (GPIO_Pin * 4));
        // Set bits
        GPIOx->AFR[0] |= (AF << (GPIO_Pin * 4));
    } else {
        // Clear bits
        GPIOx->AFR[1] &= ~(0xF << ((GPIO_Pin - 8) * 4));
        // Set bits
        GPIOx->AFR[1] |= (AF << ((GPIO_Pin - 8) * 4));
    }
}


/*i moved these back to hal_nvic and hal_exti
void NVIC_Config(void) {
    // Enable EXTI0_1 interrupt in NVIC
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 1); // Priority 1 (High)
}

void Fix_Interrupt_Starvation(void) {
    NVIC_SetPriority(EXTI0_1_IRQn, 3); // Set EXTI to lowest priority
}

void Adjust_SysTick_Priority(void) {
    NVIC_SetPriority(SysTick_IRQn, 2); // Set SysTick to medium priority
}


void EXTI_Config(void) {
    // Enable system configuration controller clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

    // Map EXTI0 to PA0 (User Button)
    SYSCFG->EXTICR[0] &= ~(SYSCFG_EXTICR1_EXTI0); // Set to 0000 for PA0

    // Enable EXTI0 interrupt on rising edge
    EXTI->IMR |= EXTI_IMR_IM0;       // Unmask EXTI0
    EXTI->RTSR |= EXTI_RTSR_RT0;     // Enable rising-edge trigger
}
*/