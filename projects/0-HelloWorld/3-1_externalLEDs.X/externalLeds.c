/*
 * File:   externalLeds.c
 * Author: Mel Ho
 * Brief: externalLEDs drives two external LEDS, one red and one green, with two different frequencies
 * 5Hz and 10Hz.
 * Created on January 11, 2021, 1:36 PM
 */


#include "xc.h"
#include "BOARD.h"
#include "leds.h"
#include "Buttons.h"

#define TRUE 1
#define FALSE 0
#define MAX_ITERATION 0x00FF

#define FIFTY_MS 6000
#define ONEHUNDRED_MS 6000 * 100 / 50
#define RED_LED LATDbits.LATD2
#define GREEN_LED LATDbits.LATD1
#define BUTTON PORTDbits.RD3

int i;


int main(void)
{
    BOARD_Init();
    
    /* TRISx Initializations */
    ///// LEDs /////
    // RED LED | pin 6
    TRISDbits.TRISD2 = 0;
    // GREEN LED | pin 5
    TRISDbits.TRISD1 = 0;
    
    ///// Button /////
    // push button | pin 9
    TRISDbits.TRISD3 = 1;
    
    /* LATx Initializations */                                      
    
     ///// LEDs /////
    // RED LED | pin 6
    LATDbits.LATD2 = 0;
    // GREEN LED | pin 5
    LATDbits.LATD1 = 0;
    
    while(TRUE){
        

        if ((i % FIFTY_MS == 0) && (BUTTON == TRUE))
        {
            GREEN_LED ^= 1;
        }
        else if ((i % FIFTY_MS == 0) && (BUTTON == FALSE))
        {
            RED_LED ^= 1;
        }

        if ((i % ONEHUNDRED_MS == 0) && (BUTTON == TRUE))
        {
            RED_LED ^= 1;
        }
        else if ((i % ONEHUNDRED_MS == 0) && (BUTTON == FALSE))
        {
            GREEN_LED ^= 1;
        }
        i++;
    }
}
