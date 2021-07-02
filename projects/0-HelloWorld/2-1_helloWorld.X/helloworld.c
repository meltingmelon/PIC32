/*
 * File:   helloworld.c
 * Author: Mel Ho
 * Brief: Basic Hello world which turns on LEDS using buttons from the I/O shield.
 */


#include "xc.h"
#include "leds.h"
#include "Buttons.h"
#include "BOARD.h"
#include "stdio.h"

#define TRUE 1

#define LD12 0x0003
#define LD34 0x000C
#define LD56 0x0030
#define LD78 0x00C0

int main(void)
{
    uint8_t ledConfig = 0;
    uint8_t buttonEvents = 0;    
    BOARD_Init();
    LEDS_Init();
    BUTTONS_Init();
    
    while (TRUE){
        buttonEvents = BUTTONS_Check_Events();
        ledConfig = LEDS_Get();
        if (buttonEvents) {            
            if(buttonEvents & BUTTON_EVENT_1DOWN)
                LEDS_Set(ledConfig | LD12);
            else if (buttonEvents & BUTTON_EVENT_1UP)
                LEDS_Set(ledConfig & ~LD12);
            
            if(buttonEvents & BUTTON_EVENT_2DOWN)
                LEDS_Set(ledConfig | LD34);
            else if (buttonEvents & BUTTON_EVENT_2UP)
                LEDS_Set(ledConfig & ~LD34);
            
            if(buttonEvents & BUTTON_EVENT_3DOWN)
                LEDS_Set(ledConfig | LD56);
            else if (buttonEvents & BUTTON_EVENT_3UP)
                LEDS_Set(ledConfig & ~LD56);
            
            if(buttonEvents & BUTTON_EVENT_4DOWN)
                LEDS_Set(ledConfig | LD78);
            else if (buttonEvents & BUTTON_EVENT_4UP)
                LEDS_Set(ledConfig & ~LD78);
    }
}
}
