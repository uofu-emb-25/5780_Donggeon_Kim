#include "lab5.h"
#include "stm32f0xx_hal.h"
#include "lab6.h"
/**
 * @brief Entry point for the program.
 */

 extern void Configure_GPIO(void);
int main(void) {
   // HAL_Init();  
   // SystemClock_Config();  
    
  //  return lab5_main();  
      // Initialize peripherals
      Configure_GPIO();
      Configure_ADC();
  
      while (1) {
          // Read ADC value and update LEDs
          uint16_t adc_value = Read_ADC();
          Update_LEDs(adc_value);
      }
}
