/*
 * File:   FreeRunningTimer.c
 * Author: Mel Ho
 * Brief: Free running timer library. Used to perform an action on the UNO32 microcontroller with
 * a certain frequency without blocking code operations.Please PIC32 FRM Sect. 14 Timers for more information
 * on timer registers.
 */
#ifdef _SUPPRESS_PLIB_WARNING
#undef_SUPPRESS_PLIB_WARNING
#endif

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <BOARD.h>
#include <xc.h>
#include "FreeRunningTimer.h"
#include <sys/attribs.h>
#include "Protocol.h"
#include "delays.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define FPB (BOARD_GetPBClock())
#define TIMERFREQ 1000
#define PRESCALER_4 4 // 1:4
#define LOW 0
#define HIGH 1

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define TIMER_TEST 1

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static uint32_t milliseconds = 0;
static uint32_t microseconds = 0;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function TIMERS_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module */
void FreeRunningTimer_Init(void){
    
    T5CON = LOW;
    TMR5 = LOW;
    
    // Sets timer 5 to interrupt every 1 ms
    PR5 = FPB / TIMERFREQ / PRESCALER_4;
    T5CONbits.TCKPS = 0b010; // 1:4
    
    IPC5bits.T5IP = 3; // sets timer 5 priority to 3
    IPC5bits.T5IS = HIGH;
    
    IFS0bits.T5IF = LOW;
    IEC0bits.T5IE = HIGH;
    
    T5CONbits.ON = HIGH;
}

/**
 * Function: TIMERS_GetMilliSeconds
 * @param None
 * @return the current MilliSecond Count
   */
uint32_t FreeRunningTimer_GetMilliSeconds(void)
{   
    return milliseconds;
    
}

/**
 * Function: TIMERS_GetMicroSeconds
 * @param None
 * @return the current MicroSecond Count
   */
uint32_t FreeRunningTimer_GetMicroSeconds(void)
{
    return (microseconds);
}

/**
 * @Interrupt Timer5IntHandler(void)
 * @param None
 * @return None
 * @brief interrupt triggers every 1 millisecond and populates the millisecond and microsecond variables.*/
void __ISR(_TIMER_5_VECTOR, ipl3auto) Timer5IntHandler(void)
{
    IFS0bits.T5IF = 0;
    milliseconds++;
    microseconds += 1000;
}


/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
#ifdef TIMER_TEST
#include "FreeRunningTimer.h"
#include <stdio.h>
#include "Protocol.h"

int main(void)
{
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    char testMessage[MAXPAYLOADLENGTH];
    uint32_t currentMilli = 0;
    uint32_t currentMicro = 0;
    
    TRISDbits.TRISD8 = 0;
    LATDbits.LATD8 = 0;
    
    sprintf(testMessage, "Free Running Timer Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);
    
    while(1)
    {
        if (FreeRunningTimer_GetMilliSeconds() % 2000 == 0) {

            LEDS_SET(0xFFFF);
            LATDbits.LATD8 ^= 1;
            

            currentMicro = FreeRunningTimer_GetMicroSeconds();
            currentMilli = FreeRunningTimer_GetMilliSeconds();
            
            sprintf(testMessage,"ms: %d\tus: %d\r\n",currentMilli, currentMicro);
            Protocol_SendDebugMessage(testMessage);
            delay_ms(1);
        }
        else
            LEDS_SET(0x0000);
    }
    
}
#endif