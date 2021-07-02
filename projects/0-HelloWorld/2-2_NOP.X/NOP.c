/*
 * File:   NOP.c
 * Author: Mel Ho
 * Brief: NOP blinks an LED with a delay of approximately a second. This was estimated
 * using a stopwatch and an oscilloscope.
 * Created on January 9, 2021, 6:24 PM
 */


#include "xc.h" 
#include "BOARD.h"
#include "leds.h"

#define NOPS_FOR_1S 500000
#define TRUE 1
#define LED1_ON 1
#define LED1_OFF 0

void NOP_delay_1s();

int main(void)
{
    BOARD_Init();
    LEDS_Init();
    while (TRUE)
    {
        NOP_delay_1s();
        LEDS_Set(LED1_ON);
        NOP_delay_1s();
        LEDS_Set(LED1_OFF);
    }
}

void NOP_delay_1s()
{
    int i;
    for(i=0; i < NOPS_FOR_1S;i++)
    {
        asm("nop");
    }
}