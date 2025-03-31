#include "lab6.h"

// Define LED pins
#define LED1_PIN 6  
#define LED2_PIN 7
#define LED3_PIN 8
#define LED4_PIN 9
#define ADC_CHANNEL 10  // PC0 -> ADC_IN10

// Configure GPIO for PWM Output
void Configure_GPIO_Lab6(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock

    // Set PC6, PC7, PC8, PC9 as Alternate Function (AF0 for TIM3 PWM)
    GPIOC->MODER &= ~((3 << (LED1_PIN * 2)) | (3 << (LED2_PIN * 2)) |
                       (3 << (LED3_PIN * 2)) | (3 << (LED4_PIN * 2)));
    GPIOC->MODER |= ((2 << (LED1_PIN * 2)) | (2 << (LED2_PIN * 2)) |
                      (2 << (LED3_PIN * 2)) | (2 << (LED4_PIN * 2))); // AF Mode

    // Set AF0 (TIM3_CH1 to CH4)
    GPIOC->AFR[0] |= (0 << (LED1_PIN * 4)) | (0 << (LED2_PIN * 4));
    GPIOC->AFR[1] |= (0 << ((LED3_PIN - 8) * 4)) | (0 << ((LED4_PIN - 8) * 4));
}

// Configure ADC in continuous conversion mode
void Configure_ADC(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);  // Wait for calibration

    ADC1->CFGR1 &= ~ADC_CFGR1_RES;  // Set ADC resolution to 12-bit
    ADC1->CFGR1 |= ADC_CFGR1_CONT;  // Continuous mode
    ADC1->CHSELR = ADC_CHSELR_CHSEL10;  // Select PC0 (ADC_IN10)

    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY)); // Wait for ADC to be ready

    ADC1->CR |= ADC_CR_ADSTART; // Start ADC conversion
}

// Configure Timer 3 for PWM Output
void Configure_TIM3_PWM(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;  // Enable TIM3 clock

    TIM3->PSC = 79;  // Set prescaler to divide 8MHz by 80 (100kHz)
    TIM3->ARR = 255;  // Set max PWM value (0-255 scale)

    // Enable PWM Mode 1 on TIM3 Channels 1-4 (PC6, PC7, PC8, PC9)
    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos);  // CH1, CH2 PWM
    TIM3->CCMR2 |= (6 << TIM_CCMR2_OC3M_Pos) | (6 << TIM_CCMR2_OC4M_Pos);  // CH3, CH4 PWM

    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;  // Enable PWM outputs

    TIM3->CR1 |= TIM_CR1_CEN;  // Start Timer 3
}

// Read ADC value
uint16_t Read_ADC(void) {
    while (!(ADC1->ISR & ADC_ISR_EOC));  // Wait for conversion
    return ADC1->DR;
}

// Update LEDs based on ADC value using PWM brightness
void Update_LEDs(uint16_t adc_value) {
    uint8_t brightness = adc_value / 16;  // Scale ADC (0-4095) to PWM (0-255)

    TIM3->CCR1 = brightness;  // PC6 (LED1)
    TIM3->CCR2 = brightness;  // PC7 (LED2)
    TIM3->CCR3 = brightness;  // PC8 (LED3)
    TIM3->CCR4 = brightness;  // PC9 (LED4)
}

// Lab 6 Checkoff 1 - Read ADC and Update LEDs
void lab6_checkoff1(void) {
    Configure_GPIO_Lab6();
    Configure_ADC();
    Configure_TIM3_PWM();  // Enable PWM

    while (1) {
        uint16_t adc_value = Read_ADC();
        Update_LEDs(adc_value);
    }
}
