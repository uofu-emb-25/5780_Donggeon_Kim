//hal_exti.c and hal_nvic.c are part of lab2
#include "stm32f072xb.h"
#include "system_setup.h"
#include <stm32f0xx_hal.h>
#include <assert.h>

#include "hal_gpio.h"
#include "hal_exti.h"
#include "hal_nvic.h"

extern void SystemClock_Config(void);
//2.3 
void MY_HAL_SYSCFG_Config_PA0(void);
//2.4
void MY_HAL_EXTI_Enable_PA0(void);

void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void);
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode);
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
volatile uint32_t main_loop_tick = 0;  // Track time in main loop

int lab2_part1_main()  //
{
    SystemClock_Config();

    // Enable Clocks for GPIOC and GPIOA
    MY_HAL_RCC_GPIOC_CLK_ENABLE();
    MY_HAL_RCC_GPIOA_CLK_ENABLE();

    // Initialize LEDs
    MY_HAL_GPIO_Init(GPIOC, 6, 1); // RED LED
    MY_HAL_GPIO_Init(GPIOC, 7, 1); // BLUE LED
    MY_HAL_GPIO_Init(GPIOC, 8, 1); // ORANGE LED
    MY_HAL_GPIO_Init(GPIOC, 9, 1); // GREEN LED

    // Initialize Button (PA0)
    MY_HAL_GPIO_Init(GPIOA, 0, 0); // Input mode with pull-down

    // Set the green LED (PC9) high
    MY_HAL_GPIO_WritePin(GPIOC, 9, 1); // GREEN LED HIGH
    
    //2.3 
     MY_HAL_SYSCFG_Config_PA0();
    
    //2.4
    MY_HAL_EXTI_Enable_PA0();


 
      // Assertions Before and After EXTI Config
    assert((EXTI->IMR & (1 << 0)) == 0);  // Before: Expect EXTI0 to be disabled
    MY_HAL_GPIO_Init_EXTI_PA0();  // Initialize EXTI0
    assert((EXTI->IMR & (1 << 0)) != 0);  // After: Expect EXTI0 to be enabled



    while (1) {
        if (main_loop_tick >= 500) {  // Toggle RED LED every 500ms
            MY_HAL_GPIO_TogglePin(GPIOC, 6);
            main_loop_tick = 0;  // Reset counter
        }
    }
   
    // Configure PA0 Button as EXTI Interrupt   2.2
    MY_HAL_GPIO_Init_EXTI_PA0();
    

    return 0;
}

int lab2_part2_main() {
    SystemClock_Config();
    GPIO_Config();  // Make sure GPIOs are configured properly

    // Enable Clocks for GPIOC and GPIOA
    MY_HAL_RCC_GPIOC_CLK_ENABLE();
    MY_HAL_RCC_GPIOA_CLK_ENABLE();

    // Initialize LEDs
    MY_HAL_GPIO_Init(GPIOC, 6, 1); // Red LED
    MY_HAL_GPIO_Init(GPIOC, 7, 1); // Blue LED
    MY_HAL_GPIO_Init(GPIOC, 8, 1); // Orange LED
    MY_HAL_GPIO_Init(GPIOC, 9, 1); // Green LED

    // Initialize Button (PA0)
    MY_HAL_GPIO_Init(GPIOA, 0, 0); // Input mode for PA0

    // Configure External Interrupt for PA0
    
    MY_HAL_SYSCFG_Config_PA0();  // Configure EXTI source
    MY_HAL_EXTI_Enable_PA0();    // Enable EXTI for PA0

    NVIC_Config();  // Enable NVIC for EXTI0

    // Set initial LED states
    GPIOC->ODR |= (1 << 9); // Set PC9 (Green LED) HIGH initially

    while (1) {
        // Blink Red LED (PC6) in main loop to show system is running
        GPIOC->ODR ^= (1 << 6); // Toggle Red LED (PC6)
        for (volatile int i = 500000; i > 0; i--); // Delay 400-600ms
    }

    return 0;
}








   

