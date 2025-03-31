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

// DAC and Waveform Functions
void Output_Static_DAC(uint8_t value);
void Generate_Ramp_Waveform_Once(void);
void Generate_Sine_Waveform_Once(void);
void Generate_Triangle_Waveform_Once(void);
void Generate_Waveform(uint8_t type);

// Button Control
void Configure_Button(void);
uint8_t Read_Button(void);

#endif // LAB6_H
