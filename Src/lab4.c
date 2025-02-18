//hal_exti.c and hal_nvic.c are part of lab2
//this is a lab4 with help of gpt 4o
//i will keep any function inside of this file that is required to do lab 4



#include "stm32f072xb.h"
#include "system_setup.h"
#include <stm32f0xx_hal.h>
#include <assert.h>

#include "hal_gpio.h"
#include "hal_exti.h"
#include "hal_nvic.h"

extern void SystemClock_Config(void);

void MY_HAL_SYSCFG_Config_PA0(void);

void MY_HAL_EXTI_Enable_PA0(void);

void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void);
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode);
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


void USART_SendChar(char c) {
    while (!(USART3->ISR & USART_ISR_TXE)); // wait until TXE bit become 1 
    USART3->TDR = c; // data receiving 
}

void USART_SendString(char *str) {
    while (*str) {
        USART_SendChar(*str++);
    }
}

void lab4_main(void)
{
    
    char received;
    USART_Init();
    LED_Init();

    while (1) {
        received = USART_ReceiveChar();
        if (received == 'r') {
            ToggleRedLED();
        } else if (received == 'b') {
            ToggleBlueLED();
        } else {
            USART_SendString("Error: Unknown command\r\n");
        }
    }
}
    









   

