#include "main.h"
#include <stm32f0xx_hal.h>
#include "lab1.h"

int main(void)
{
  #ifdef LAB1
    lab1_main();
#else
    #error No valid target specified
#endif

}
