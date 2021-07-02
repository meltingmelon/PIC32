/*
 * File:   busContention.c
 * Author: Mel Ho
 * Brief: busContention generates a 10Hz square wave off of an output pin
 * set to open drain driven by a 5V signal. It then uses another input pin connected to a button to check
 * if the button has been pressed.
 */


#include "xc.h"
#include "BOARD.h"
#include "leds.h"

#define TRUE 1
#define LED1_ON 1
#define LED1_OFF 0
#define FIFTY_MS 6000 * 50/30 //For some reason I had to shift delay to maintain 10Hz

// Pin 6
#define ODC_OUTPUT LATDbits.LATD2
// Pin 5
#define INPUT PORTDbits.RD1
int i;

uint8_t buffer = 1;
 
int main(void) {
    BOARD_Init();
    LEDS_Init();
    
    ODCDbits.ODCD2 = 1;    
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD1 = 1;
    
    LATDbits.LATD2 = 0;
    while(TRUE)
    {
        if (i % FIFTY_MS == 0)
            ODC_OUTPUT ^= 1;
        
        if (INPUT != ODC_OUTPUT)
                LEDS_Set(LED1_ON);
        else
            LEDS_Set(LED1_OFF);
        i++;
    }
}
