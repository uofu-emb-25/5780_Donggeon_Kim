#ifndef LAB6_H
#define LAB6_H

#include "stm32f072xb.h"

// Function prototypes
void Configure_GPIO_Lab6(void);  // Renamed to avoid conflict
void Configure_ADC(void);
uint16_t Read_ADC(void);
void Update_LEDs(uint16_t adc_value);
void lab6_checkoff1(void);  // Function for Lab 6 Checkoff 1

#endif // LAB6_H
