#include "main.h"
#include <stm32f0xx_hal.h>

void SystemClock_Config(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    MY_HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    MY_HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    uint8_t button_last_state = 0;
    uint8_t button_current_state = 0;

    while (1) {
        button_current_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

        if (button_current_state && !button_last_state) {
            HAL_Delay(50);  // Debounce delay
            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) { 
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
                while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0));
            }
        }

        button_last_state = button_current_state;
    }
}
