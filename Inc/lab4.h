#ifndef LAB4_H
#define LAB4_H

#include "stm32f072xb.h"
#include <stdbool.h>  // Required for bool, true, false

// Define baud rate and system clock
#define BAUD_RATE 115200
#define SYS_CLOCK 48000000  // 8 MHz Clock

// USART Functions
void USART3_Init(int part);                   // Initialize USART3 with part selection
void USART3_4_IRQHandler(void);               // USART3 Interrupt Handler
char USART3_ReceiveChar(void);                // Receive a character over UART
bool USART3_ReceiveCharTimeout(char *c, uint32_t timeout); 
void USART_SendChar(char c);                  // Send one character over UART
void USART_SendString(const char *str);       // Send a string over UART

// GPIO & LED Control Functions
void ToggleRedLED(void);                      // Toggle Red LED (PC6)
void ToggleBlueLED(void);                     // Toggle Blue LED (PC7)
void GPIO_Init(void);                         // Initialize GPIO for LEDs
void LED_Control(char led, char command);     // Control LEDs based on command

// Main Lab4 Function
void lab4_main(void); //내꺼                        // Entry function for Lab 4/void lab4_main_part1(void);                   // Checkoff 1 function
void lab4_main_part2(void);                   // Checkoff 2 function
void lab4_main(void);

//void part1(void);
//void part2(void);




#endif // LAB4_H
