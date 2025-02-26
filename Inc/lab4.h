#ifndef LAB4_H
#define LAB4_H

#include "stm32f0xx.h"

void USART3_Init();

char USART_ReceiveChar();
void USART_SendChar(char c);
void USART_SendString(char *str);

void LED_Init();
void ToggleRedLED();
void ToggleBlueLED();
int lab4_main(void);
#endif 