/*
 * File:   improvedHelloWorld.c
 * Author: Mel Ho
 * Brief: improvedHelloWorld counts up in binary with leds incrementing
 * until a button is pressed or the leds go over the max iteration.
 */


#include "xc.h"
#include "BOARD.h"
#include "Buttons.h"
#include "leds.h"

#define TRUE 1
#define MAX_ITERATION 0x00FF

#define NOPS_FOR_MS 6000 / 43

uint8_t LEDBits = 0;

int count = 0;
void NOP_delay(int k);

int main(void)
{
    BOARD_Init();
    BUTTONS_Init();
    LEDS_Init();
    TRISDbits.TRISD1 = 0;
    while(TRUE){
        LEDS_Set(LEDBits);
        LEDBits++;
        if (LEDBits > MAX_ITERATION)
            LEDBits = 0;
        NOP_delay(250);

    }
}

void NOP_delay(int k)
{
    int i;
    for(i=0; i < NOPS_FOR_MS * k;i++)
    {
        asm("nop");
        if (i % 500 == 0)
        {
            if (BTN1 | BTN2 | BTN3 | BTN4)
                LEDBits = 0;
        }
    }
}
