//hal_exti.c and hal_nvic.c are part of lab2
//this is a lab4 with help of gpt 4o
//lab 4



#include "stm32f072xb.h"
#include "system_setup.h"
#include <stdbool.h> //need for true false boolean

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
    // 1. Turn on power for USART3 (so it works)
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;  // Give USART3 power
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;  // Also turn on power for GPIOB (used for TX/RX)

    // 2. Set PB10 (TX) and PB11 (RX) to "special mode" (Alternate Function)
    GPIOB->MODER &= ~((3 << (10 * 2)) | (3 << (11 * 2)));  // Clear old settings
    GPIOB->MODER |= (2 << (10 * 2)) | (2 << (11 * 2));  // Set to AF mode (needed for USART)

    // 3. Tell PB10 and PB11 to act like USART3 (AF4)
    GPIOB->AFR[1] |= (4 << ((10 - 8) * 4)) | (4 << ((11 - 8) * 4));  // "AF4" is USART3's setting

    // 4. Set how fast USART should talk (baud rate)
    USART3->BRR = SYS_CLOCK / BAUD_RATE;  // Example: 8,000,000 / 115200 = ~69

    // 5. Turn on TX (send) and RX (receive) so it can talk & listen
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;

    // 6. Tell USART3 to shout when it gets data (enable RX interrupt)
    USART3->CR1 |= USART_CR1_RXNEIE;  // "RXNEIE" = "Receive Data Not Empty Interrupt"

    // 7. Tell the CPU to listen for USART3 (turn on interrupt in NVIC)
    NVIC_EnableIRQ(USART3_4_IRQn);  // Let USART3 interrupts happen
    NVIC_SetPriority(USART3_4_IRQn, 1);  // Set priority (1 means "not super important")

    // 8. Finally, turn USART3 ON!
    USART3->CR1 |= USART_CR1_UE;  // "UE" = "USART Enable"
}

void USART3_4_IRQHandler(void) {
    // Check if USART3 got a new letter
    if (USART3->ISR & USART_ISR_RXNE) {  
        char received = USART3->RDR;  // Grab the letter (clears the flag)

        // Send back the letter (echo)
        USART_SendChar(received);
        
        // Do something if it's 'r' or 'b' (turn LEDs on/off)
        if (received == 'r') {
            ToggleRedLED();  // Flip Red LED on/off
            USART_SendString("Red LED toggled.");  // Tell the user
        } else if (received == 'b') {
            ToggleBlueLED();  // Flip Blue LED on/off
            USART_SendString("Blue LED toggled.");  // Tell the user
        } else {
            USART_SendString("Unknown command.");  // Confused? Tell the user!
        }
    }
}
//To make it recognize lower==upper
char to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (c + 32) : c;
}


char USART3_ReceiveChar(void) {
    while (!(USART3->ISR & USART_ISR_RXNE));  // Wait for RX buffer to be full
    return USART3->RDR;  // Read received character
}


bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout) {
    while (!(USART3->ISR & USART_ISR_RXNE) && timeout--);
    if (timeout == 0) return false;
    *c = USART3->RDR;
    return true;
}



void USART_SendChar(char c) {
    while (!(USART3->ISR & USART_ISR_TXE)); // wait until TXE bit become 1 
    USART3->TDR = c; // data receiving 
}

void USART_SendString(const char *str) {
    if (!str) return;
    while (*str) {
        USART_SendChar(*str++);
    }
    USART_SendChar('\r');
    USART_SendChar('\n');
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


void LED_Control(char led, char command) {
    uint16_t pin = (led == 'r') ? (1 << 6) : (led == 'b') ? (1 << 7) : 0;
    if (pin) {
        if (command == '0') GPIOC->BSRR = pin << 16;  // Turn Off
        else if (command == '1') GPIOC->BSRR = pin;   // Turn On
        else if (command == '2') GPIOC->ODR ^= pin;   // Toggle
    }
}


void lab4_main(void)
{
    
    while (1) {
        USART_SendString("CMD?");
        char received = USART3_ReceiveChar();
        LED_Control(received, USART3_ReceiveChar());
    }
}
    









   

