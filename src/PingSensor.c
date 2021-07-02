/* 
 * File:  PingSensor.c
 * Author: Mel Ho
 * Brief:  A library for the HC-SR04 ultrasonic proximity sensor:
 * https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf. To capture and calculate distance
 * in cm, the IC3 register was used to determine time of flight with its interrupt triggering on every edge. Please
 * refer to the PIC32 FRM Sect. 15 Input Capture for more information on how it is configured. State machine for interrupt
 * logic is inside the Lab 2 Report of this Github repo.
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "PingSensor.h"
#include "BOARD.h"
#include <xc.h>
#include <sys/attribs.h>
#include "FreeRunningTimer.h"
#include "Protocol.h"
#include "string.h"
#include "stdio.h"
#include "MessageIDs.h"
#include "delays.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define TIME_OF_FLIGHT 340

#define FPB (BOARD_GetPBClock())
#define TIMERFREQ_TR4 16.667
#define TIMERFREQ_TR2 50
#define PRESCALER_16 16
#define PRESCALER_64 64
#define CM_TO_MM 10
#define OUTPUT 0
#define INPUT 1

#define LOW 0
#define HIGH 1

#define FALSE 0
#define TRUE 1

#define TIMER_2 1
#define SIMPLE_CAPTURE_EVERY 0b110
#define FIRST_CAPTURE_RISE 1
#define INTERRUPT_EVERY_CAPTURE 0b00;

#define RISING_EDGE 1
#define FALLING_EDGE 0

#define TRIGGER_PIN_INIT (TRISDbits.TRISD8 = OUTPUT)
#define TRIGGER_PIN LATDbits.LATD8 //Pin 2

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static uint16_t uptime = 0;
static uint16_t downtime = 0;
static uint16_t timeInterval = 0;
static uint16_t distance = 0;

static uint32_t edgeState = RISING_EDGE;

static char message[MAXPAYLOADLENGTH];
static uint32_t milliseconds = 0;
static uint32_t Hz = 100;

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
// #define PING_TEST 1

/*******************************************************************************
 * PUBLIC FUNCTIONs                                                            *
 ******************************************************************************/
/**
 * @Function PingSensor_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes hardware for PingSensor with the needed interrupts */
int PingSensor_Init(void)
{
    T4CON = 0;
    TMR4 = 0;
    
    T2CON = 0;
    TMR2 = 0;
    
    IC3CON = 0;
    IC3CONbits.ICTMR = TIMER_2;            //TIMER2
    IC3CONbits.ICM = SIMPLE_CAPTURE_EVERY; //Edge Detect Mode - every edge
    IC3CONbits.FEDGE = FIRST_CAPTURE_RISE;
    IC3CONbits.ICI = INTERRUPT_EVERY_CAPTURE;
    IC3BUF;                                // clear input capture buffer
    
    IPC3bits.IC3IP = 2; //IC3 PRIORITY (higher than timer 2)
    IFS0bits.IC3IF = 0;
    IEC0bits.IC3IE = 1;
    
    T2CONbits.TCKPS = 0b100; // 1:16
    T4CONbits.TCKPS = 0b110; // 1:64
    
    // I set both PRs to my desired tick value, given the frequency I want that is then prescaled to fit a 16-bit timer.
    PR2 = FPB / TIMERFREQ_TR2 / PRESCALER_16; // interrupt every 20ms / 20000us
    PR4 = FPB / TIMERFREQ_TR4 / PRESCALER_64; // interrupt at 60ms (16.666667Hz)
    
    IPC4bits.T4IP = 1; // TIMER 4 PRIORITY
    IPC4bits.T4IS = 1;
    
    IPC2bits.T2IP = 3; // TIMER 2 PRIORITY
    IPC2bits.T2IS = 2;
    
    IFS0bits.T4IF = 0;  
    IEC0bits.T4IE = 1;
    
    TRIGGER_PIN_INIT;
    TRIGGER_PIN = LOW;
    
    T4CONbits.ON = TRUE;
    T2CONbits.ON = TRUE;
    IC3CONbits.ON = TRUE;
    
    return SUCCESS;  
}

/**
 * @Function int PingSensor_GetDistance(void)
 * @param None
 * @return uint16_t corresponding to distance in millimeters */
uint16_t PingSensor_GetDistance(void)
{
    // time = timeInteveral * (4 x Pre-scaler) / Fosc (number of ticks x time of each tick).
    // this converts from ticks to us.
    // distance  = (time/2) / 58 (convert to cm)
    uint16_t dist = ((timeInterval * ((4 * PRESCALER_16) / TIMERFREQ_TR2)) / 58 / 2);
    return (dist);
}

/*******************************************************************************
 * INTERRUPTS                                                                  *
 ******************************************************************************/

/**
 * @Interrupt Timer4IntHandler(void)
 * @param None
 * @return None
 * @brief When timer 4's interrupt occurs, the ping sensor is triggered to send an ultrasonic burst*/
void __ISR(_TIMER_4_VECTOR) Timer4IntHandler(void)
{
    IFS0bits.T4IF = 0;
    TRIGGER_PIN = HIGH;
    delay_us(10);
    TRIGGER_PIN = LOW;
}

/**
 * @Interrupt IC3Interrupt(void)
 * @param None
 * @return None
 * @brief the echo state is captured on every edge and the time duration is calculated based on the edge state of
 * our signal. */
void __ISR(_INPUT_CAPTURE_3_VECTOR) __IC3Interrupt(void)
{
    IFS0bits.IC3IF = 0;
    switch(edgeState)
    {
        case RISING_EDGE:
            if (edgeState == PORTDbits.RD10)
            {
                uptime = (0xFFFF & IC3BUF);
                edgeState = FALLING_EDGE;
                
                IC3BUF; // Clear buffer if it's still not empty
            }
            break;
            
        case FALLING_EDGE:
            if (edgeState == PORTDbits.RD10)
            {
                downtime = (0xFFFF & IC3BUF);
                timeInterval = downtime - uptime;
                edgeState = RISING_EDGE;
    
                IC3BUF; // Clear buffer if it's still not empty
            }
            break;
    }
            IC3BUF;

}


#ifdef PING_TEST
int main(void)
{
    BOARD_Init();
    PingSensor_Init();
    FreeRunningTimer_Init();
    Protocol_Init();
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    while(1)
    {
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds >= Hz) 
        {
            Hz += 100;
            distance = PingSensor_GetDistance();
            distance = Protocol_ShortEndednessConversion(distance);
            Protocol_SendMessage(2, ID_PING_DISTANCE,&distance);

        }
    }
}
#endif
