//done with help of GPT
#include "stm32f072xb.h"
#include "system_setup.h"  // Include header that declares SystemClock_Config
#include <stm32f0xx_hal.h>
#include "main.h"
#include "lab2.h"
#include "lab1.h"
#include "lab3.h"
#include "lab4.h"
extern void SystemClock_Config(void);  // Prevent multiple definition error
//part1
//extern int lab1_part1_main(void);

extern int lab1_part2_main(void);

extern int lab1_givenexample_main(void);

int lab3_main(void);
extern volatile uint32_t main_loop_tick;


int main(void) {
       //return lab1_givenexample_main(); //given exmaple 
       //return lab1_part1_main();
       // return lab1_part2_main();
       //return lab2_part1_main();
       //return lab2_part2_main();
       //return lab3_main();
       //lab4_main(); this is for r and b testing
       lab4_main_part1();// for check off 1
       //lab4_main_part2();
       return 0;
}






