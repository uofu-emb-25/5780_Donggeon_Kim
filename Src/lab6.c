#include "lab6.h"

// Define LED pins and ADC channel
#define LED1_PIN 6  
#define LED2_PIN 7
#define LED3_PIN 8
#define LED4_PIN 9
#define ADC_CHANNEL 10  // PC0 -> ADC_IN10

// Configure GPIO for ADC and PWM Output
void Configure_GPIO_Lab6(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN; // Enable GPIO clocks

    // Set PC6-PC9 as Alternate Function (AF0 for TIM3 PWM)
    GPIOC->MODER |= (2 << (LED1_PIN * 2)) | (2 << (LED2_PIN * 2)) |
                    (2 << (LED3_PIN * 2)) | (2 << (LED4_PIN * 2));

    // Set PA4 as Analog Mode (DAC_OUT1)
    GPIOA->MODER |= (3 << (4 * 2));  // PA4 = 11 (Analog mode)
}

// Configure ADC in continuous conversion mode
void Configure_ADC(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
    ADC1->CR |= ADC_CR_ADCAL;
    while (ADC1->CR & ADC_CR_ADCAL);

    ADC1->CFGR1 |= ADC_CFGR1_CONT;
    ADC1->CHSELR = ADC_CHSELR_CHSEL10;
    ADC1->CR |= ADC_CR_ADEN;
    while (!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_ADSTART;
}

// Configure Timer 3 for PWM Output
void Configure_TIM3_PWM(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 79;
    TIM3->ARR = 255;

    TIM3->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos) | (6 << TIM_CCMR1_OC2M_Pos);
    TIM3->CCMR2 |= (6 << TIM_CCMR2_OC3M_Pos) | (6 << TIM_CCMR2_OC4M_Pos);

    TIM3->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E;
    TIM3->CR1 |= TIM_CR1_CEN;
}

// Read ADC value
uint16_t Read_ADC(void) {
    while (!(ADC1->ISR & ADC_ISR_EOC));
    return ADC1->DR;
}

// Update LEDs based on ADC value using PWM
void Update_LEDs(uint16_t adc_value) {
    uint8_t brightness = adc_value / 16;
    TIM3->CCR1 = brightness;
    TIM3->CCR2 = brightness;
    TIM3->CCR3 = brightness;
    TIM3->CCR4 = brightness;
}

// Lab 6 Checkoff 1: ADC to PWM LED Brightness Control
void lab6_checkoff1(void) {
    Configure_GPIO_Lab6();
    Configure_ADC();
    Configure_TIM3_PWM();

    while (1) {
        uint16_t adc_value = Read_ADC();
        Update_LEDs(adc_value);
    }
}

// Configure DAC
void Configure_DAC(void) {
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;  // Enable DAC clock
    DAC->CR |= DAC_CR_EN1;  // Enable DAC Channel 1 (PA4)
}

// Output a fixed DAC voltage
void Output_Static_DAC(uint8_t value) {
    DAC->DHR8R1 = value;  // Set DAC output voltage (8-bit)
}
void Generate_Ramp_Waveform(void) {
    while (1) {
        for (uint8_t i = 0; i < 255; i++) {
            DAC->DHR8R1 = i;  // Increase voltage step-by-step
            for (volatile int j = 0; j < 1000; j++);  // Delay to control speed
        }
    }
}
// Define a Sine Wave lookup table (32 samples)
const uint8_t sine_wave[32] = {
    127,151,175,197,216,232,244,251,254,251,244,232,216,197,175,151,
    127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102
};

// Generate Sine Wave on DAC
void Generate_Sine_Waveform(void) {
    while (1) {
        for (int i = 0; i < 32; i++) {
            DAC->DHR8R1 = sine_wave[i];  // Output sine wave value
            for (volatile int j = 0; j < 1000; j++);  // Delay for waveform timing
        }
    }
}
void Generate_Triangle_Waveform(void) {
    while (1) {
        for (uint8_t i = 0; i < 255; i++) {
            DAC->DHR8R1 = i;  // Rising edge
            for (volatile int j = 0; j < 1000; j++);
        }
        for (uint8_t i = 255; i > 0; i--) {
            DAC->DHR8R1 = i;  // Falling edge
            for (volatile int j = 0; j < 1000; j++);
        }
    }
}
void Generate_Waveform(uint8_t type) {
    switch (type) {
        case 0:
            Generate_Sine_Waveform();
            break;
        case 1:
            Generate_Triangle_Waveform();
            break;
        case 2:
            Generate_Ramp_Waveform();
            break;
        default:
            Generate_Sine_Waveform();
            break;
    }
}
void Configure_Button(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock
    GPIOC->MODER &= ~(3 << (13 * 2));  // Set PC13 as input
}

uint8_t Read_Button(void) {
    return (GPIOC->IDR & (1 << 13)) == 0;  // Button press detected
}
// Lab 6 Checkoff 2
void lab6_checkoff2(void) {
    Configure_DAC();
    Configure_Button();
    uint8_t waveform = 0;

    while (1) {
        if (Read_Button()) {
            waveform = (waveform + 1) % 3;  // Cycle through waveforms
            Generate_Waveform(waveform);
        }
    }
}