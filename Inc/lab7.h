#ifndef LAB7_H
#define LAB7_H

#include <stdint.h>  // 
int lab7_main(void);  // Function prototype

// PI controller variables to be monitored by STMStudio
extern int16_t target_rpm;
extern int16_t motor_speed;
extern int16_t error;
extern int16_t error_integral;
extern int16_t output;
extern int16_t duty_cycle;
extern uint16_t adc_value;
extern int16_t Kp;
extern int16_t Ki;

#endif
