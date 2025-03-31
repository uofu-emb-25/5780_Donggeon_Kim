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
void Output_Static_DAC(uint8_t value);  // Added function prototype for DAC
void Generate_Ramp_Waveform(void);  // Added function prototype
void Generate_Sine_Waveform(void);  // Added function prototype for sine wave
void Generate_Triangle_Waveform(void);  // Added function prototype

#endif // LAB6_H
