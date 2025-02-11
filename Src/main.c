//done with help of GPT
#include "stm32f072xb.h"
#include "system_setup.h"  // Include header that declares SystemClock_Config
#include <stm32f0xx_hal.h>
#include "main.h"
#include "lab2.h"
#include "lab1.h"
extern void SystemClock_Config(void);  // Prevent multiple definition error
//part1
//extern int lab1_part1_main(void);

extern int lab1_part2_main(void);

extern int lab1_givenexample_main(void);




int main(void) {
       //return lab1_givenexample_main(); //given exmaple 
       //return lab1_part1_main();
       // return lab1_part2_main();
       //return lab2_part1_main();
    return lab2_part2_main();
}






