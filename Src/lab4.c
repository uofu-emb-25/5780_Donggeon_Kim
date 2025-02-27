#include <stdio.h>
#include "stm32f072xb.h"
#include "system_setup.h"
#include <stdbool.h>

// External Function
extern void SystemClock_Config(void);

// USART Function Prototypes
void USART3_Init(int part);
void USART3_4_IRQHandler(void);
char USART3_ReceiveChar(void);
bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout);
void USART_SendChar(char c);
void USART_SendString(const char *str);

// LED Control Functions
void ToggleRedLED(void);
void ToggleBlueLED(void);
void GPIO_Init(void);
void LED_Control(char led, char command);

// Global Variables
volatile uint8_t usart_mode = 1;  // 1 for part1, 2 for part2
volatile char received_char = 0;
volatile bool new_data_available = false;
volatile char uart_buffer[2];
volatile uint8_t uart_rx_index = 0;

#define BAUD_RATE 115200
#define SYS_CLOCK 8000000  // 8 MHz Clock

//  Initialize USART3
void USART3_Init(int part) {
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    GPIOC->MODER &= ~((3 << (4 * 2)) | (3 << (5 * 2)));  
    GPIOC->MODER |= (2 << (4 * 2)) | (2 << (5 * 2));  

    GPIOC->AFR[0] &= ~((0xF << (4 * 4)) | (0xF << (5 * 4)));  
    GPIOC->AFR[0] |= (1 << (4 * 4)) | (1 << (5 * 4));  

    USART3->BRR = SYS_CLOCK / BAUD_RATE;
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;

    NVIC_EnableIRQ(USART3_4_IRQn);
    NVIC_SetPriority(USART3_4_IRQn, 1);

    usart_mode = part;  // Set mode for Part 1 or Part 2
    USART3->CR1 |= USART_CR1_UE;
}

//  Single IRQ Handler for Both Checkoff 1 & 2
void USART3_4_IRQHandler(void) {
    if (USART3->ISR & USART_ISR_RXNE) {
        char temp_char = USART3->RDR;
        if (temp_char == '\r' || temp_char == '\n') return;

        if (usart_mode == 1) {  
            // Checkoff 1: Read One Char at a Time
            if (!new_data_available) {
                received_char = temp_char;
                new_data_available = true;
            }
        } else {  
            // Checkoff 2: Read Two Inputs at Once
            uart_buffer[uart_rx_index] = temp_char;
            uart_rx_index++;
            if (uart_rx_index == 2) {  
                new_data_available = true;
                uart_rx_index = 0;
            }
        }

        // Clear UART Errors
        USART3->ICR |= USART_ICR_ORECF | USART_ICR_FECF | USART_ICR_PECF;
    }
}

bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout) {
    while (timeout--) {
        if (new_data_available) { // Check if data is ready from IRQ handler
            new_data_available = false;  // Reset flag
            *c = received_char;  // Store received character
            return true; // Successfully received character
        }
        __NOP(); // Small delay to avoid CPU overuse
    }
    return false; // Timeout occurred
}

//  USART Functions
void USART_SendChar(char c) {
    while (!(USART3->ISR & USART_ISR_TXE));
    USART3->TDR = c;
}

void USART_SendString(const char *str) {
    if (!str) return;
    while (*str) {
        USART_SendChar(*str++);
    }
    USART_SendChar('\r');  
    USART_SendChar('\n');  
}

//  LED Control Functions
void ToggleRedLED(void) { GPIOC->ODR ^= (1 << 6); }
void ToggleBlueLED(void) { GPIOC->ODR ^= (1 << 7); }

//  GPIO Initialization
void GPIO_Init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2))); 
    GPIOC->MODER |= (1 << (6 * 2)) | (1 << (7 * 2));   
    GPIOC->BSRR = (1 << (6 + 16)) | (1 << (7 + 16));
}

//  LED Control Based on Input
void LED_Control(char led, char command) {
    uint16_t pin = (led == 'r') ? (1 << 6) : (led == 'b') ? (1 << 7) : 0;
    
    if (pin) {  // Ensure 'r' or 'b' was provided
        if (command == '0') {
            GPIOC->BSRR = pin << 16;  // Turn Off
            USART_SendString((led == 'r') ? "Red LED OFF!" : "Blue LED OFF!");
        } 
        else if (command == '1') {
            GPIOC->BSRR = pin;   // Turn On
            USART_SendString((led == 'r') ? "Red LED ON!" : "Blue LED ON!");
        } 
        else if (command == '2') {
            GPIOC->ODR ^= pin;   //  TOGGLE LED using XOR
            USART_SendString((led == 'r') ? "Red LED Toggled!" : "Blue LED Toggled!");
        } 
        else {
            USART_SendString("Error: Invalid command! Use 0, 1, or 2.");
        }
    } else {
        USART_SendString("Error: Use 'r' or 'b' for LED control.");
    }
}


// Checkoff 1 - Read LED Color First, Then Command
void lab4_main_part1() {
    USART3_Init(1);
    GPIO_Init();
    USART_SendString("Enter LED commands (e.g., r1 for Red ON, b0 for Blue OFF):");

    while (1) {
        if (new_data_available) {
            new_data_available = false; // Reset flag
            char led = received_char;   // Read first input
            received_char = 0;          // **Clear received_char to avoid carry-over**
           
            // Debugging: Print first received character
            char debug_msg[40];
            sprintf(debug_msg, "First char received: %c", led);
            USART_SendString(debug_msg);

            // Ensure first char is 'r' or 'b'
            if (led != 'r' && led != 'b') {
                USART_SendString("Error: First input must be 'r' or 'b'.");
                continue; // Skip and wait for new input
            }

            // Now wait for the second character (0 or 1)
            char command;
            bool received = USART3_ReceiveCharTimeout(&command, 2000000); // Wait for 2 seconds

            // **Clear UART buffer to prevent carry-over**
            received_char = 0;
            new_data_available = false;

            // Debugging: Print second received character
            sprintf(debug_msg, "Second char received: %c", command);
            USART_SendString(debug_msg);

            if (!received) {
                USART_SendString("Error: Timeout waiting for second input.");
                continue;
            }

            // Ensure second char is '0' or '1'
            if (command != '0' && command != '1') {
                USART_SendString("Error: Second input must be '0' or '1'.");
                continue;
            }

            // Send confirmation message
            char response[30];
            sprintf(response, "Received: %c%c", led, command);
            USART_SendString(response);

            // Control LED
            LED_Control(led, command);

            // **Clear any possible leftover input**
            received_char = 0;
            new_data_available = false;
        }
    }
}
// Checkoff 2 - Read Both Inputs at Once
void lab4_main_part2() {
    USART3_Init(2);  // Use IRQ mode for Part 2
    GPIO_Init();

    USART_SendString("USART Ready. Enter two-character LED command (e.g., r1, b0, r2):");

    while (1) {
        if (new_data_available) {  // When both characters are received
            new_data_available = false;
            char led = uart_buffer[0];  
            char command = uart_buffer[1];

            //  Validate LED input
            if (led != 'r' && led != 'b') {
                USART_SendString(" Error: First input must be 'r' or 'b'.");
                continue;
            }

            //  Validate Command (0, 1, or 2)
            if (command != '0' && command != '1' && command != '2') {
                USART_SendString(" Error: Second input must be '0', '1', or '2'.");
                continue;
            }

            //  Show received input
            char response[30];
            sprintf(response, " Received: %c%c", led, command);
            USART_SendString(response);

            //  Perform LED action (ON, OFF, TOGGLE)
            LED_Control(led, command);
        }
    }
}