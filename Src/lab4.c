//hal_exti.c and hal_nvic.c are part of lab2
//this is a lab4 with help of gpt 4o
//lab 4



#include "stm32f072xb.h"
#include "system_setup.h"

#include <assert.h>


extern void SystemClock_Config(void);

void MY_HAL_SYSCFG_Config_PA0(void);

void MY_HAL_EXTI_Enable_PA0(void);

void MY_HAL_RCC_GPIOC_CLK_ENABLE(void);
void MY_HAL_RCC_GPIOA_CLK_ENABLE(void);
void MY_HAL_GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t Mode);
void MY_HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
void MY_HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
uint8_t MY_HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);


#include "stm32f0xx.h"

// Define baud rate
#define BAUD_RATE 115200
#define SYS_CLOCK 8000000  // Assuming an 8 MHz clock

void USART3_Init(void) {
    // 1. Enable USART3 Clock (APB1)
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    
    // 2. Enable GPIOB Clock
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    
    // 3. Configure PB10 (TX) & PB11 (RX) to Alternate Function mode
    GPIOB->MODER &= ~((3 << (10 * 2)) | (3 << (11 * 2))); // Clear mode bits
    GPIOB->MODER |= (2 << (10 * 2)) | (2 << (11 * 2));    // Set to AF mode

    // 4. Set PB10 and PB11 to AF4 (USART3)
    GPIOB->AFR[1] |= (4 << ((10 - 8) * 4)) | (4 << ((11 - 8) * 4));

    // 5. Configure USART3 Baud Rate (BRR)
    USART3->BRR = SYS_CLOCK / BAUD_RATE;  // 8000000 / 115200 ≈ 69

    // 6. Enable USART3 transmitter and receiver
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE; 

    // 7. Enable USART3
    USART3->CR1 |= USART_CR1_UE; 
}

char USART3_ReceiveChar(void) {
    while (!(USART3->ISR & USART_ISR_RXNE));  // Wait for RX buffer to be full
    return USART3->RDR;  // Read received character
}



void USART_SendChar(char c) {
    while (!(USART3->ISR & USART_ISR_TXE)); // wait until TXE bit become 1 
    USART3->TDR = c; // data receiving 
}

void USART_SendString(char *str) {
    while (*str) {
        USART_SendChar(*str++);
    }
}


/* Toggle Red LED (PC6) */
void ToggleRedLED(void) {
    GPIOC->ODR ^= (1 << 6);
}

/* Toggle Blue LED (PC7) */
void ToggleBlueLED(void) {
    GPIOC->ODR ^= (1 << 7);
}

// GPIOC clock enable part
void GPIO_Init(void) {
    // Enable GPIOC clock
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
}

void lab4_main(void)
{
    
    char received;
    USART3_Init();
    HAL_Init();

    while (1) {
        received = USART3_ReceiveChar();
        if (received == 'r') {
            ToggleRedLED();
        } else if (received == 'b') {
            ToggleBlueLED();
        } else {
            USART_SendString("Error: Unknown command\r\n");
        }
    }
}
    









   

