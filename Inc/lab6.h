#ifndef LAB6_H
#define LAB6_H

#include "stm32f0xx.h"

// Function prototypes
void Configure_GPIO(void);
void Configure_ADC(void);
uint16_t Read_ADC(void);
void Update_LEDs(uint16_t adc_value);

#endif // LAB6_H
