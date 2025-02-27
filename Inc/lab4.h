#ifndef LAB4_H
#define LAB4_H

#include "stm32f072xb.h"
#include <stdbool.h>  // Required for bool, true, false

// Define baud rate and system clock
#define BAUD_RATE 115200
#define SYS_CLOCK 8000000  // 8 MHz Clock

// Function Declarations

void USART3_Init(void);                      // Initialize USART3
void USART3_4_IRQHandler(void);              // USART3 Interrupt Handler
char to_lower(char c);                       // Convert uppercase to lowercase
void USART_SendChar(char c);                 // Send one character over UART
void USART_SendString(const char *str);      // Send a string over UART
char USART3_ReceiveChar(void);               // Receive a character over UART
bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout); // Receive with timeout

// GPIO & LED Control Functions
void ToggleRedLED(void);                     // Toggle Red LED (PC6)
void ToggleBlueLED(void);                    // Toggle Blue LED (PC7)
void GPIO_Init(void);                        // Initialize GPIO for LEDs
void LED_Control(char led, char command);    // Control LEDs based on command

// Main Lab4 Function
void lab4_main(void);                        // Entry function for Lab 4
void lab4_main_part1(void);
void lab4_main_part2(void);

#endif // LAB4_H
