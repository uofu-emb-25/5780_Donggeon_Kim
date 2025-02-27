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

void USART3_Init(void);
void USART3_4_IRQHandler(void);
char to_lower(char c);
void USART_SendChar(char c);
void USART_SendString(const char *str);
void LED_Control(char led, char command);
char USART3_ReceiveChar(void);
bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout);
void ToggleRedLED(void);
void ToggleBlueLED(void);
void GPIO_Init(void);



#include "stm32f0xx.h"

// Define baud rate
#define BAUD_RATE 115200
#define SYS_CLOCK 8000000  // Assuming an 8 MHz clock

void USART3_Init(void) {
    // Enable clocks for USART3 and GPIOC
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;  
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  

    // Set PC4 (TX) and PC5 (RX) to "Alternate Function"
    GPIOC->MODER &= ~((3 << (4 * 2)) | (3 << (5 * 2)));  
    GPIOC->MODER |= (2 << (4 * 2)) | (2 << (5 * 2));  

    // Set PC4 and PC5 to AF1 (USART3)
    GPIOC->AFR[0] &= ~((0xF << (4 * 4)) | (0xF << (5 * 4)));  
    GPIOC->AFR[0] |= (1 << (4 * 4)) | (1 << (5 * 4));  

    // Configure USART3 baud rate
    USART3->BRR = SYS_CLOCK / BAUD_RATE;  

    // Enable TX, RX, and RX interrupt
    USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE;  

    // Enable USART3 in NVIC (Interrupt Controller)
    NVIC_EnableIRQ(USART3_4_IRQn);
    NVIC_SetPriority(USART3_4_IRQn, 1);

    // Enable USART3
    USART3->CR1 |= USART_CR1_UE;
}




// Function to make uppercase letters into lowercase
char to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? (c + 32) : c;
}


volatile char received_char = 0;  // Store received character

volatile bool new_data_available = false;  // Flag for new data availability
void USART3_4_IRQHandler(void) {
    if (USART3->ISR & USART_ISR_RXNE) {  
        char temp_char = USART3->RDR;  // Read received character

        // Ignore unwanted characters (newline and carriage return)
        if (temp_char == '\r' || temp_char == '\n') {
            return;
        }

        // Only store a new character if none is waiting to be processed
        if (!new_data_available) {  
            received_char = temp_char;
            new_data_available = true;  // Mark as new data available
        }

        //  Clear possible UART errors to prevent multiple triggers
        USART3->ICR |= USART_ICR_ORECF;  // Overrun error
        USART3->ICR |= USART_ICR_FECF;   // Framing error
        USART3->ICR |= USART_ICR_PECF;   // Parity error
    }
}


bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout) {
    // Wait for a NEW character (not the previously received one)
    while (!new_data_available && timeout--); 
    if (timeout == 0) return false;

    new_data_available = false;  //  Reset flag since we've processed the data
    *c = received_char;  //  Store the new character

    // Ignore unwanted newline and carriage return characters
    if (*c == '\r' || *c == '\n') {
        return false;
    }

    return true;
}



char USART3_ReceiveChar(void) {
    while (!(USART3->ISR & USART_ISR_RXNE)); // Wait until a character is received
    return USART3->RDR;  // Read received character
}



void USART_SendChar(char c) {
    while (!(USART3->ISR & USART_ISR_TXE)); // wait until TXE bit become 1 
    USART3->TDR = c; // data receiving 
}

void USART_SendString(const char *str) {
    if (!str) return;
    while (*str) {
        USART_SendChar(*str++);  // Send each character
    }
    USART_SendChar('\r');  // Carriage return
    USART_SendChar('\n');  // Newline
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
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  

    // Set PC6 and PC7 as output mode (01)
    GPIOC->MODER &= ~((3 << (6 * 2)) | (3 << (7 * 2))); // Clear bits
    GPIOC->MODER |= (1 << (6 * 2)) | (1 << (7 * 2));    // Set as output mode

    // Ensure LEDs are OFF initially
    GPIOC->BSRR = (1 << (6 + 16)) | (1 << (7 + 16));
}


void LED_Control(char led, char command) {
    uint16_t pin = (led == 'r') ? (1 << 6) : (led == 'b') ? (1 << 7) : 0;
    
    if (pin) {  // Check if 'r' or 'b' was provided
        if (command == '0') {
            GPIOC->BSRR = pin << 16;  // Turn Off
            USART_SendString((led == 'r') ? "Red LED OFF!\r\n" : "Blue LED OFF!\r\n");
        } 
        else if (command == '1') {
            GPIOC->BSRR = pin;   // Turn On
            USART_SendString((led == 'r') ? "Red LED ON!\r\n" : "Blue LED ON!\r\n");
        } 
        else if (command == '2') {
            GPIOC->ODR ^= pin;   // Toggle
            USART_SendString((led == 'r') ? "Red LED Toggled!\r\n" : "Blue LED Toggled!\r\n");
        } 
        else {
            USART_SendString("Error: Invalid command! Use 0, 1, or 2.\r\n");  
        }
    } else {
        USART_SendString("Error: Use 'r' or 'b' for LED control.\r\n");
    }
}
//just toggling led based on r or b this is not check off section
 void lab4_main() {
    USART3_Init();
    GPIO_Init();

    USART_SendString("USART Ready. Type 'r' for Red LED, 'b' for Blue LED:\r\n");

    while (1) {
        if (new_data_available) {  // Check if new data was received
            new_data_available = false;  // Reset flag

            // Ignore unwanted newline and carriage return
            if (received_char == '\r' || received_char == '\n') {
                continue;  // Ignore and keep looping
            }

            USART_SendString("Received: ");
            USART_SendChar(received_char);
            USART_SendChar('\r');
            USART_SendChar('\n');

            if (received_char == 'r') {
                ToggleRedLED();
                USART_SendString("Red LED Toggled!\r\n");
            } else if (received_char == 'b') {
                ToggleBlueLED();
                USART_SendString("Blue LED Toggled!\r\n");
            } else {
                USART_SendString("Error: Invalid Command! Use 'r' or 'b'.\r\n");
            }
        }
    }
}
//Checkoff 1
void lab4_main_part1() {
    USART3_Init();
    GPIO_Init();

    USART_SendString("USART Ready. Enter LED commands (e.g., r1 for Red ON, b0 for Blue OFF):\r\n");

    while (1) {
        if (new_data_available) {  // Check if new data was received
            new_data_available = false;  // Reset flag
            
            char led = received_char;  //  Store first character (`r` or `b`)
            
            // Debugging: Print what was received
            char debug_msg[30];
            sprintf(debug_msg, "First char: %c\r\n", led);
            USART_SendString(debug_msg);

            // Ensure it's a valid LED command character
            if (led != 'r' && led != 'b') {
                USART_SendString("Error: Use 'r' or 'b' for LED control.\r\n");
                continue;
            }

            //  Wait for second character
            char command;
            if (USART3_ReceiveCharTimeout(&command, 500000)) { // Wait for second character
                //  Debugging: Print the second character
                sprintf(debug_msg, "Second char: %c\r\n", command);
                USART_SendString(debug_msg);

                // Ensure second character is valid (0 or 1)
                if (command != '0' && command != '1') {
                    USART_SendString("Error: Invalid command! Use 0 or 1.\r\n");
                    continue;
                }

                char response[50];
                sprintf(response, "Received: %c%c\r\n", led, command);
                USART_SendString(response);

                LED_Control(led, command);
            } else {
                USART_SendString("Error: Command timeout! Enter two characters.\r\n");
            }
        }
    }
}
