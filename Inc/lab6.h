#ifndef LAB6_H
#define LAB6_H

#include "stm32f072xb.h"

// Function prototypes
void Configure_GPIO_Lab6(void);
void Configure_ADC(void);
void Configure_DAC(void);
void Configure_TIM3_PWM(void);
uint16_t Read_ADC(void);
void Update_LEDs(uint16_t adc_value);
void lab6_checkoff1(void);
void lab6_checkoff2(void);

#endif // LAB6_H
