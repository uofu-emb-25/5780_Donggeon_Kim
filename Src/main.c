//done with help of GPT
#include "stm32f072xb.h"
#include "system_setup.h"  // Include header that declares SystemClock_Config
#include <stm32f0xx_hal.h>
#include "main.h"
#include "lab2.h"
#include "lab1.h"
extern void SystemClock_Config(void);  // Prevent multiple definition error

extern int lab1_part1_main(void);
extern int lab1_part2_main(void);
extern int lab1_givenexample_main(void);

extern int lab1_main(void);
extern int lab2_main(void);



// Part1
/* void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
*/

//part2
/*
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void);
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode);
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void delay_ms(uint32_t ms);
*/
int main(void) {

     return lab1_part2_main();
     //return lab1_part1_main();
    
}






//given exmaple
 /*  HAL_Init(); // Reset of all peripherals, init the Flash and Systick
SystemClock_Config(); //Configure the system clock
/* This example uses HAL library calls to control
the GPIOC peripheral. You’ll be redoing this code
with hardware register access. */
/*__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
// Set up a configuration struct to pass to the initialization function
GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
GPIO_MODE_OUTPUT_PP,
GPIO_SPEED_FREQ_LOW,
GPIO_NOPULL};
HAL_GPIO_Init(GPIOC, &initStr); // Initialize pins PC8 & PC9
HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); // Start PC8 high
while (1) {
HAL_Delay(200); // Delay 200ms
// Toggle the output state of both PC8 and PC9
HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
} */

 /*  //part1
  // Configure the system clock
    SystemClock_Config();

    // Enable the GPIOC peripheral clock
    MY_HAL_RCC_GPIOC_CLK_ENABLE();
    
    // Initialize GPIOC for PC6 (Red LED) and PC7 (Blue LED)
    MY_HAL_GPIO_Init(GPIOC, 6);
    MY_HAL_GPIO_Init(GPIOC, 7);

    // Set PC6 high (Red LED on) and PC7 low (Blue LED off)
    MY_HAL_GPIO_WritePin(GPIOC, 6, 1);
    MY_HAL_GPIO_WritePin(GPIOC, 7, 0);

    while (1) {
        // Toggle PC6 and PC7
        MY_HAL_GPIO_TogglePin(GPIOC, 6);
        MY_HAL_GPIO_TogglePin(GPIOC, 7);

        // Delay for visibility
        for (volatile int i = 0; i < 500000; i++); // Simple delay loop
    }

    return 0;
}

// Function to Enable GPIOC Clock
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    ///end of part 1   */


//part2 
/*
    // Configure the system clock
    SystemClock_Config();

    // Enable GPIO clocks for LED and Button
    MY_HAL_RCC_GPIOC_CLK_ENABLE();
    MY_HAL_RCC_GPIOA_CLK_ENABLE();

    // Initialize GPIOC for PC6 (Red LED) and PC7 (Blue LED)
    MY_HAL_GPIO_Init(GPIOC, 6, 1); // Mode 1: Output
    MY_HAL_GPIO_Init(GPIOC, 7, 1); // Mode 1: Output

    // Initialize GPIOA for PA0 (Button with Pull-Down)
    MY_HAL_GPIO_Init(GPIOA, 0, 0); // Mode 0: Input

    // Set initial LED state
    MY_HAL_GPIO_WritePin(GPIOC, 6, 0); // Red LED OFF
    MY_HAL_GPIO_WritePin(GPIOC, 7, 1); // Blue LED ON 

    uint8_t lastButtonState = 0; // Track last button state

    while (1) {
        uint8_t buttonState = MY_HAL_GPIO_ReadPin(GPIOA, 0);

        if (buttonState && !lastButtonState) {  // Button press detected (rising edge)
            MY_HAL_GPIO_TogglePin(GPIOC, 6);  // Toggle Red LED
            MY_HAL_GPIO_TogglePin(GPIOC, 7);  // Toggle Blue LED
            delay_ms(50);  // Debounce delay
        }

        lastButtonState = buttonState;  // Store current button state
    }
*/



//part1
/*
// Function to Enable GPIOC Clock
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

// Function to Enable GPIOA Clock
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}
*/


//part 2 blue and red blinking
/*
// Function to Enable GPIOC Clock
void MY_HAL_RCC_GPIOC_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

// Function to Enable GPIOA Clock
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
}

// Software Delay Function
void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 4000; i++);
}
*/
