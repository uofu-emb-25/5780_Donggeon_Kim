//done with help of GPT
//still not perfect i think 
#include "stm32f072xb.h"
#include "system_setup.h"  // Include header that declares SystemClock_Config
#include <stm32f0xx_hal.h>
#include "main.h"
#include "lab2.h"
#include "lab1.h"
#include "lab3.h"
#include "lab4.h"
#include "lab5.h"
extern void SystemClock_Config(void);  // Prevent multiple definition error
//part1
//extern int lab1_part1_main(void);

extern int lab1_part2_main(void);

extern int lab1_givenexample_main(void);

int lab3_main(void);
extern volatile uint32_t main_loop_tick;
// Function prototypes
void UART2_Init(void);
void UART2_SendChar(char c);
int _write(int file, char *ptr, int len);

int main(void) {
       //return lab1_givenexample_main(); //given exmaple 
       //return lab1_part1_main();
       // return lab1_part2_main();
       //return lab2_part1_main();
       //return lab2_part2_main();
       //return lab3_main();
       //lab4_main(); this is for r and b testing
     // lab4_main_part1();// for check off 1
    // lab4_main_part2();

       SystemClock_Config();
       GPIO_LED_Init();
       GPIO_I2C_Init();
       I2C2_Reset();
       I2C2_Init();
   
       lab5_main_part1();
 

      
       return 0;
}






