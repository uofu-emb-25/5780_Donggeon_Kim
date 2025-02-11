#include "stm32f0xx_hal.h"
#include "system_setup.h"
#include "hal_gpio.h"


/** 
 *  Uncomment ONE of these defines to choose which "main" function you want.
 *  Or comment them all out and call the desired function manually from another file.
// #define LAB_PART_1
// #define LAB_PART_2
// #define LAB_GIVEN_EXAMPLE
 */

/**
 * PART 1 main function (Blink two LEDs on PC6, PC7 using direct register access)
 */
int lab1_part1_main(void)
{
    // Optional: Initialize HAL (for SysTick, etc.)
    HAL_Init();

    // Configure the system clock
    SystemClock_Config();

    // Enable GPIOC clock
    MY_HAL_RCC_GPIOC_CLK_ENABLE();

    // Configure PC6 & PC7 as output
 
    MY_HAL_GPIO_Init(GPIOC, 6,1);
    MY_HAL_GPIO_Init_Output(GPIOC, 7);

    // Initial LED states
    MY_HAL_GPIO_WritePin(GPIOC, 6, 1); // LED on PC6 high
    MY_HAL_GPIO_WritePin(GPIOC, 7, 0); // LED on PC7 low

    // Blink loop
    while (1)
    {
        MY_HAL_GPIO_TogglePin(GPIOC, 6);
        MY_HAL_GPIO_TogglePin(GPIOC, 7);

        // Simple delay
        for (volatile int i = 0; i < 500000; i++); 
    }

    // Unreachable
    return 0;
}


// Software Delay Function
void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 4000; i++);
}

int lab1_part2_main(void)
{
    HAL_Init();
    SystemClock_Config();

    MY_HAL_RCC_GPIOC_CLK_ENABLE();
    MY_HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure PC6 & PC7 as outputs (red and blue LEDs)
    MY_HAL_GPIO_Init_Output(GPIOC, 6); // Red LED
    MY_HAL_GPIO_Init_Output(GPIOC, 7); // Blue LED

    // Configure PA0 as input (button)
    MY_HAL_GPIO_Init_Input(GPIOA, 0);

    // Set initial LED states: red OFF, blue ON (for example)
    MY_HAL_GPIO_WritePin(GPIOC, 6, 0);
    MY_HAL_GPIO_WritePin(GPIOC, 7, 1);

    uint8_t lastButtonState = 0;
    while (1)
    {
        uint8_t buttonState = MY_HAL_GPIO_ReadPin(GPIOA, 0);
    
        if (buttonState != lastButtonState) 
        {
            delay_ms(50);  // Debounce: wait 50ms and check again
            if (MY_HAL_GPIO_ReadPin(GPIOA, 0) == buttonState)  // Confirm it's not bouncing
            {
                if (buttonState == 1)  // If the button was pressed (rising edge)
                {
                    MY_HAL_GPIO_TogglePin(GPIOC, 6);
                    MY_HAL_GPIO_TogglePin(GPIOC, 7);
                }
            }
        }
    
        lastButtonState = buttonState;
    }
    return 0;
}


/**
 * GIVEN EXAMPLE main function:
 *  Uses standard HAL calls to blink PC8 and PC9 at 200ms intervals
 *  (As originally provided in the comment block)
 */
int lab1_givenexample_main(void)
{
    HAL_Init(); // Reset of all peripherals, init the Flash and SysTick
    SystemClock_Config(); // Configure the system clock

    __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC

    // Set up a configuration struct to pass to the initialization function
    GPIO_InitTypeDef initStr = {
        GPIO_PIN_8 | GPIO_PIN_9,    // Pins to initialize
        GPIO_MODE_OUTPUT_PP,        // Push-pull output
        GPIO_SPEED_FREQ_LOW,        // Low speed
        GPIO_NOPULL                 // No pull-up or pull-down
    };

    // Initialize pins PC8 & PC9
    HAL_GPIO_Init(GPIOC, &initStr);

    // Start PC8 high
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

    while (1) 
    {
        // Delay 200ms
        HAL_Delay(200);
        
        // Toggle the output state of both PC8 and PC9
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
    }
    
    // Unreachable
    return 0;
}

/**
 * If you want to compile and run directly from this file without 
 * a separate main.c, you can define LAB_PART_1, LAB_PART_2, or LAB_GIVEN_EXAMPLE,
 * and we provide one "master" main() here that calls the appropriate function.
 */
#if defined(LAB_PART_1) || defined(LAB_PART_2) || defined(LAB_GIVEN_EXAMPLE)
int main(void)
{
#ifdef LAB_PART_1
    return lab1_part1_main();
#elif defined(LAB_PART_2)
    return lab1_part2_main();
#elif defined(LAB_GIVEN_EXAMPLE)
    return lab1_givenexample_main();
#endif
    // Should never get here
    return 0;
}
#endif
