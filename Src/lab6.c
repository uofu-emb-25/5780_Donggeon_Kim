#include "lab6.h"

// Define LED pins
#define LED1_PIN 6  
#define LED2_PIN 7
#define LED3_PIN 8
#define LED4_PIN 9
#define ADC_CHANNEL 10  // PC0 -> ADC_IN10

// Configure GPIO: LED Outputs and ADC Input
void Configure_GPIO_Lab6(void) {  // Renamed to avoid conflict
    // Enable clock for GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    
    // Set LED pins as outputs
    GPIOC->MODER |= (1 << (LED1_PIN * 2)) | (1 << (LED2_PIN * 2)) | 
                     (1 << (LED3_PIN * 2)) | (1 << (LED4_PIN * 2));

    // Set PC0 as Analog Mode for ADC input
    GPIOC->MODER |= (3 << (0 * 2));  // PC0 Analog Mode
}

// Configure ADC in continuous conversion mode
void Configure_ADC(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL); // Wait for calibration to finish
    ADC1->CFGR1 |= ADC_CFGR1_CONT;  
    ADC1->CHSELR = ADC_CHSELR_CHSEL10;
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_ADSTART;
}

// Read ADC value
uint16_t Read_ADC(void) {
    while (!(ADC1->ISR & ADC_ISR_EOC)); // Wait until conversion is complete
    return ADC1->DR;
}

// Update LEDs based on ADC value
void Update_LEDs(uint16_t adc_value) {
    GPIOC->ODR = 0; // Turn off all LEDs first

    if (adc_value > 64)  GPIOC->ODR |= (1 << LED1_PIN);
    if (adc_value > 128) GPIOC->ODR |= (1 << LED2_PIN);
    if (adc_value > 192) GPIOC->ODR |= (1 << LED3_PIN);
    if (adc_value > 240) GPIOC->ODR |= (1 << LED4_PIN);
}

// Lab 6 Checkoff 1 - Read ADC and Update LEDs
void lab6_checkoff1(void) {
    Configure_GPIO_Lab6();
    Configure_ADC();

    while (1) {
        uint16_t adc_value = Read_ADC();

        // If ADC is very low, turn OFF all LEDs
        if (adc_value < 50) {
            GPIOC->ODR = 0;
        }
        // If ADC is medium, turn ON one LED
        else if (adc_value < 150) {
            GPIOC->ODR = (1 << LED1_PIN);
        }
        // If ADC is high, turn ON multiple LEDs
        else if (adc_value < 250) {
            GPIOC->ODR = (1 << LED1_PIN) | (1 << LED2_PIN);
        }
        // If ADC is very high, turn ON all LEDs
        else {
            GPIOC->ODR = (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | (1 << LED4_PIN);
        }

        // Small delay to stabilize
        for (volatile int i = 0; i < 50000; i++);
    }
}
