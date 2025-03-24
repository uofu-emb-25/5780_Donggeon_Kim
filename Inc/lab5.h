#ifndef LAB5_H
#define LAB5_H

#include <stdint.h> 

void GPIO_LED_Init(void);

#define LED_RED     (1 << 6)  // PC6
#define LED_BLUE    (1 << 7)  // PC7
#define LED_ORANGE  (1 << 8)  // PC8
#define LED_GREEN   (1 << 9)  // PC9

#endif
