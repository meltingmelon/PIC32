/*
 * File:   RotaryEncoder.c
 * Author: Mel Ho
 * Brief:  Functions for the AS5047D Rotary Encoder. This library was specifically designed for both controlling a system using a rotary encoder, and 
 * using the rotary encoder as sensor for an angle compensation control loop with a DC motor. The SPI protocol was used for communication.
 * Please refer to the spec sheet: https://ams.com/AS5047D for more information on the communication protocol used with this unit.
 * Created on February 1, 2021, 10:06 PM
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <proc/p32mx340f512h.h>
#include <xc.h>
#include <BOARD.h>
#include "FreeRunningTimer.h"
#include "Protocol.h"
#include "RotaryEncoder.h"
#include "string.h"
#include "stdio.h"
#include "delays.h"
#include "MessageIDs.h"
#include <sys/attribs.h>
#include <stdint.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define OUTPUT 0
#define INPUT 1

#define ENCODER_ROLLOVER (1 << 14)

#define FPB BOARD_GetPBClock()
#define DESIRED_FREQ 5000000 //5MHz
#define TIMER_FREQ 1000 // 1000Hz

#define PRESCALER_4 4

#define DIAAGC ((1 << 14) + 0x3FFC)
#define SEND_DIAAGC (DIAAGC | checkParity(DIAAGC) << 15)

#define ANGLE ((1 << 14) + 0x3FFE)
#define READ_ANGLE (ANGLE | checkParity(ANGLE) << 15)

#define ANGLECOM ((1 << 14) + 0x3FFF)
#define READ_ANGLECOM (ANGLECOM | (checkParity(ANGLECOM) << 15))

#define NOP ((1 << 14) + 0x0000)
#define SEND_NOP (NOP | (checkParity(NOP) << 15))

#define HIGH 1
#define LOW 0

#define SS_PIN_10_INIT (TRISGbits.TRISG9 = OUTPUT)
#define SS_LOW (LATGbits.LATG9 = LOW)
#define SS_HIGH (LATGbits.LATG9 = HIGH)

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define ENCODER 1
//#define ENCODER_INTERRUPT 1
//#define ENCODER_RATE 1
//#define SPI 1
//#define ENCODER_ACCUMULATE 1


/*******************************************************************************
 * STATIC VARIABLES                                                            *
 ******************************************************************************/
static char message[MAXPAYLOADLENGTH];
static uint16_t rawAngle = 0;
static uint32_t milliseconds = 0;

static uint16_t currentAngle = 0;
static uint16_t prevAngle = 0;
static int32_t totalAngle = 0;
static iht32_t angularVelocity = 0;

static int32_t prevTick = 0;
static int32_t encoderRate = 0;
static int32_t rolloverCount = 0;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function checkParity(uint32_t in)
 * @param in, unsigned int to check parity
 * @return parity, parity bit (0 or 1)
 * @brief  Checks the even parity of the SPI transaction by shifting through each bit and perform an AND bitwise operation.*/
uint16_t checkParity(uint32_t in)
{
    uint16_t parity = 0;
    int32_t i;
    for(i=0x0001; i <= 0x8000; i <<= 1)
    {
        if (in & i)
            parity += 1;
    }
    parity = parity % 2;
    return parity;
}

/**
 * @Function RotaryEncoder_Init(char interfaceMode)
 * @param interfaceMode, one of the two #defines determining the interface
 * @return SUCCESS or ERROR
 * @brief initializes hardware in appropriate mode along with the needed interrupts. Can be configured to either
 * code blocking mode or interrupt mode. See PIC32 FRM Sect. 23: SPI for more details on the SPI control registers. */
int32_t RotaryEncoder_Init(char interfaceMode)
{
    switch(interfaceMode)
    {
        case (ENCODER_BLOCKING_MODE): 
            SPI2CON = 0;
            SPI2CONbits.MSTEN = HIGH;
            SPI2CONbits.SMP = HIGH;
            SPI2CONbits.CKP = LOW;
            SPI2CONbits.CKE = LOW;
            SPI2CONbits.MODE32 = LOW;
            SPI2CONbits.MODE16 = HIGH;

            // SPI2BRG is calculated using Eq. 23-3 in the PIC32 FRM
            SPI2BRG = (FPB / DESIRED_FREQ / 2) - 1;
            SPI2BUF;

            SS_PIN_10_INIT;

            SPI2CONbits.ON = 1;
            break;
            
        case (ENCODER_INTERRUPT_MODE):
            
            SPI2CON = 0;
            SPI2BRG = (FPB / DESIRED_FREQ / 2) - 1;
            SPI2BUF = 0;

            SPI2CONbits.MSTEN = HIGH;
            SPI2CONbits.FRMEN = LOW;
            SPI2CONbits.MODE16 = HIGH;
            SPI2CONbits.MODE32 = LOW;

            SPI2CONbits.SMP = HIGH;
            SPI2CONbits.CKP = LOW;
            SPI2CONbits.CKE = LOW; 
             
            
            SPI2BUF; // clears SPI buffer
            IFS1bits.SPI2RXIF = LOW; // clears SPI interrupt flag
            IPC7bits.SPI2IP = 5; // sets SPI priority to 5

            SS_PIN_10_INIT;
              
            T2CON = 0;
            T2CONbits.TCKPS = 0b000; // 1:4 ratio
            PR2 = FPB / TIMER_FREQ; // for 1ms interrupt
            IPC2bits.T2IP = 4; // sets T2 priority to 4
            IFS0bits.T2IF = LOW; // clear T2 flag

            SPI2CONbits.ON = HIGH;
            T2CONbits.ON = HIGH;
            
            // turn T2 timer ON
            IEC0bits.T2IE = HIGH; // enable T2 interrupt
            IEC1bits.SPI2RXIE = HIGH; // enable SPI RX interrupt

            SS_LOW;
            SPI2BUF = READ_ANGLECOM;
            while (SPI2STATbits.SPIBUSY); // wait for the SPI transaction to happen
            SS_HIGH;
            break;
    }
    return SUCCESS;
}

/**
 * @Function int RotaryEncoder_ReadRawAngle(void)
 * @param None
 * @return 14-bit number representing the raw encoder angle (0-16384) */
uint16_t RotaryEncoder_ReadRawAngle(void)
{
    SS_LOW;
    delay_us(1);
    SPI2BUF = READ_ANGLECOM;
    while(!SPI2STATbits.SPIRBF);
    SS_HIGH;
    delay_us(1);
    SPI2BUF;
    
    SS_LOW;
    delay_us(1);
    SPI2BUF = SEND_NOP;
    while(!SPI2STATbits.SPIRBF);
    SS_HIGH;
    delay_us(1);
    
    // Reads raw angle with the top two misc bits cleared
    rawAngle = (0x3FFF & SPI2BUF);
    return(rawAngle);
}

/**
 * @Function RotaryEncoder_ReadRawAngleInterruptMode(void)
 * @param None
 * @return currentAngle, thn current angle of the AS5047D rotary encoder
 * @brief a wrapper function that returns the current angle read by the rotary encoder when set to interrupt mode */
uint16_t RotaryEncoder_ReadRawAngleInterruptMode(void)
{
    return currentAngle;
}

/**
 * @Function RotaryEncoder_CheckRate(uint16_t currentTick)
 * @param currentTick, the current position read by the encoder
 * @return w, the rate of the encoder
 * @brief returns the angular rate of the AS5047D rotary encoder */
int32_t RotaryEncoder_CheckRate(uint16_t currentTick)
{
    int16_t w = 0;
    if ((currentTick - prevTick) >= TICK_RATE)
    {
        prevTick = currentTick;
        
        w = currentAngle - prevAngle;
        prevAngle = currentAngle;
        
        if (w > MAX_RATE)
            w -= ENCODER_ROLLOVER;
        
        if (w < -MAX_RATE)
            w += ENCODER_ROLLOVER;
        
        return w;
    }
}

/**
 * @Function RotaryEncoder_AccumulatedAngle(void)
 * @param None
 * @return totalAngle, the accumulated angle
 * @brief returns the angle of the output shaft accounting for rollovers in the encoder. */
int32_t RotaryEncoder_AccumulatedAngle(void)
{    
    angularVelocity = currentAngle - prevAngle;
    prevAngle = currentAngle;
    if (angularVelocity > MAX_RATE)
    {
        angularVelocity -= ENCODER_ROLLOVER;
        rolloverCount -= ENCODER_ROLLOVER;
    }
    
    if (angularVelocity < -MAX_RATE)
    {
        angularVelocity += ENCODER_ROLLOVER;
        rolloverCount += ENCODER_ROLLOVER;
    }
    totalAngle = currentAngle + rolloverCount;
    return totalAngle;
}


/*******************************************************************************
 * INTERRUPTS                                                                  *
 ******************************************************************************/
/**
 * @Interrupt Timer2IntHandler(void)
 * @param None
 * @return None
 * @brief Sends command READ_ANGLECOM to get the measured anple with dynamic angle error compensation*/
void __ISR(_TIMER_2_VECTOR) Timer2IntHandler(void)
{
    SS_LOW;
    SPI2BUF = READ_ANGLECOM;
    IFS0bits.T2IF = LOW;
    SPI2BUF;
}

/**
 * @Interrupt SPI2Interrupt(void)
 * @param None
 * @return None
 * @brief sets currentAngle to the measured angle value when SPI2 interrupt is triggered*/
void __ISR(_SPI_2_VECTOR) __SPI2Interrupt(void)
{
    IFS1bits.SPI2RXIF = 0;

    //Reads raw angle with the top two misc bits cleared
    currentAngle = (0x3FFF & SPI2BUF); 
    SS_HIGH;
}


/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
#ifdef SPI
int main(void)
{
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_SPI_MODE);
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    while(1)
    {
        unsigned short testMsg = (1 << 14) + 0xAA;
        testMsg = SEND_NOP;
        SS_LOW;
        SPI2BUF = testMsg;
        while(SPI2STATbits.SPIBUSY);
        SS_HIGH;
        delay_ns(350);
        sprintf(message, "%X", SPI2BUF);
        Protocol_SendDebugMessage(message);
        delay_ms(10);
    }
}
#endif

#ifdef ENCODER
int main(void)
{
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_BLOCKING_MODE);
    FreeRunningTimer_Init();
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);

    unsigned short test = RotaryEncoder_ReadRawAngle();
    test = (0x3FFF & test);
    test = Protocol_ShortEndednessConversion(test);
    Protocol_SendMessage(2,ID_ROTARY_ANGLE,&test);
    
    while(1)
    {
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        test = RotaryEncoder_ReadRawAngle();
        test = (0x3FFF & test);
      
        test = Protocol_ShortEndednessConversion(test);
        if (milliseconds % 50 == 0)
        {
            Protocol_SendMessage(2,ID_ROTARY_ANGLE,&test);
        }
      
    }
//    {
//        test = RotaryEncoder_ReadRawAngle();
//        test =(test << 2);
//        sprintf(message, "%i", test);
//        Protocol_SendDebugMessage(message);
//        delay_ms(10);
//    }

}
#endif

#ifdef ENCODER_INTERRUPT
int main(void)
{
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    FreeRunningTimer_Init();
    
    unsigned int hz = 10;
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);

    sprintf(message, "Encoder Interrupt Mode");
    Protocol_SendDebugMessage(message);

    while(1)
    {

        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds > hz)
        {
            hz += 10;
            currentAngle = (0x3FFF & currentAngle);
            currentAngle = Protocol_ShortEndednessConversion(currentAngle);
            Protocol_SendMessage(2,ID_ROTARY_ANGLE,&currentAngle);
            
        }
      
    }
}
#endif

#ifdef ENCODER_RATE
int main(void)
{
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    FreeRunningTimer_Init();
    
    unsigned int hz = 100;
    unsigned int hz_velocity = TICK_RATE;
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);

    sprintf(message, "Encoder Interrupt Mode");
    Protocol_SendDebugMessage(message);

    while(1)
    {

        milliseconds = FreeRunningTimer_GetMilliSeconds();
        
        if (milliseconds > hz_velocity)
        {
            hz_velocity += TICK_RATE;
            encoderRate = RotaryEncoder_CheckRate(milliseconds);
            
        }
        if (milliseconds > hz)
        {
            hz += 100;
            encoderRate = Protocol_IntEndednessConversion(encoderRate);
            Protocol_SendMessage(sizeof(encoderRate),ID_REPORT_RATE,&encoderRate);
            
        }
      
    }
}
#endif

#ifdef ENCODER_ACCUMULATE
int main(void)
{
    BOARD_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    FreeRunningTimer_Init();
    
    unsigned int hz_5ms = 100;
    unsigned int hz_2ms = 2;
    
    signed int accumAngle = 0;
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);

    sprintf(message, "Encoder Interrupt Mode");
    Protocol_SendDebugMessage(message);

    while(1)
    {

        milliseconds = FreeRunningTimer_GetMilliSeconds();
        
        if (milliseconds > hz_2ms)
        {
            hz_2ms += 2;
            accumAngle = RotaryEncoder_AccumulatedAngle();
            
        }
        if (milliseconds > hz_5ms)
        {
            hz_5ms += 100;
            accumAngle = Protocol_IntEndednessConversion(accumAngle);
            Protocol_SendMessage(sizeof(accumAngle),ID_ENCODER_ABS,&accumAngle);
            
        }
      
    }
}
#endif