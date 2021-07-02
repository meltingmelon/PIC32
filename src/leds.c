/*
 * File:   leds.c
 * Author: Mel Ho
 *
 * LED Library of UNO32 I/O shield
 * Created on December 19, 2012, 2:08 PM
 */

#include <stdint.h>
#include <xc.h>
#include "Buttons.h"
#include "BOARD.h"

#define OUTPUT 0x0000
#define OFF 0x0000

void LEDS_Init()
{ 
    TRISE = OUTPUT;
    LATE = OFF;
}

uint8_t LEDS_Get()
{
    uint8_t leds = LATE;
    return leds;
}

void LEDS_Set(uint8_t x)
{
    LATE = x;
}
