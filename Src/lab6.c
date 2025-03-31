#include "lab6.h"

// Define LED pins
#define LED1_PIN 6  
#define LED2_PIN 7
#define LED3_PIN 8
#define LED4_PIN 9
#define ADC_CHANNEL 10  // PC0 -> ADC_IN10

// Configure GPIO: LED Outputs and ADC Input
void Configure_GPIO(void) {
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
    // Enable ADC clock
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;

    // Calibrate ADC
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL); // Wait for calibration to finish

    // Set ADC resolution to 8-bit and continuous conversion mode
    ADC1->CFGR1 |= ADC_CFGR1_CONT;  

    // Select ADC channel 10 (PC0)
    ADC1->CHSELR = ADC_CHSELR_CHSEL10;

    // Enable ADC
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY)); // Wait until ADC is ready

    // Start ADC conversion
    ADC1->CR |= ADC_CR_ADSTART;
}

// Read ADC value
uint16_t Read_ADC(void) {
    while (!(ADC1->ISR & ADC_ISR_EOC)); // Wait until conversion is complete
    return ADC1->DR; // Return ADC value
}

// Update LEDs based on ADC value
void Update_LEDs(uint16_t adc_value) {
    GPIOC->ODR = 0; // Turn off all LEDs first

    if (adc_value > 64)  GPIOC->ODR |= (1 << LED1_PIN);
    if (adc_value > 128) GPIOC->ODR |= (1 << LED2_PIN);
    if (adc_value > 192) GPIOC->ODR |= (1 << LED3_PIN);
    if (adc_value > 240) GPIOC->ODR |= (1 << LED4_PIN);
}
