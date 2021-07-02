/*
 * File:   DCMotorDrive.c
 * Author: Mel Ho
 * Brief: This library provides functions to set and get the motorspeed of an RC Servo
 * motor using the output compare (OC) register. Please see PIC32 FRM Sect. 16 for more information
 * about the OC registers.
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

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define TIMER_FREQ 500
#define PRESCALER_4 4
#define FPB BOARD_GetPBClock()

#define PWM_SCALE_FACTOR 1000

#define LOW 0
#define HIGH 1

#define OUTPUT 0
#define INPUT 1

#define IN1_INIT (TRISDbits.TRISD0 = OUTPUT) // pin 3
#define IN2_INIT (TRISDbits.TRISD8 = OUTPUT) // pin 2

#define IN1_LOW (LATDbits.LATD0 = LOW)
#define IN1_HIGH (LATDbits.LATD0 = HIGH)

#define IN2_LOW (LATDbits.LATD8 = LOW)
#define IN2_HIGH (LATDbits.LATD8 = HIGH)

#define IN1 PORTDbits.RD0
#define IN2 PORTDbits.RD8

#define PWM_FAULT_DISABLED 0b110
#define TIMER3_SOURCE_CLOCK 1

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define DCMOTOR_TEST 1

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static char message[MAXPAYLOADLENGTH];
static int32_t currentMotorSpeed = 0;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function DCMotorDrive_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes timer3 to 2Khz and set up the pins
 * @warning you will need 3 pins to correctly drive the motor  */
int32_t DCMotorDrive_Init(void)
{
    T3CON = 0;
    TMR3 = 0;
    PR3 = FPB / TIMER_FREQ / PRESCALER_4;
    T2CONbits.TCKPS = 0b010;
    
    IN1_INIT;
    IN2_INIT;
    
    OC3CON = 0;
    OC3R = 0;
    OC3RS = 0; // initialized with 0% duty cycle
    
    // Motor Forward
    IN1_HIGH;
    IN2_LOW;
    
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;
    
    OC3CONbits.OCTSEL = TIMER3_SOURCE_CLOCK;
    OC3CONbits.OCM  = PWM_FAULT_DISABLED;
    
    T3CONbits.TON = HIGH;
    OC3CONbits.ON = HIGH;
    
    return SUCCESS;
}


/**
 * @Function DCMotorDrive_SetMotorSpeed(int32_t newMotorSpeed)
 * @param newMotorSpeed, in units of Duty Cycle (+/- 1000)
 * @return SUCCESS or ERROR
 * @brief Sets the new duty cycle for the motor, 0%->0, 100%->1000 */
int32_t DCMotorDrive_SetMotorSpeed(int32_t newMotorSpeed)
{
    int32_t pwm = 0;
    if (newMotorSpeed > 0)
    {
        IN1_HIGH;
        IN2_LOW;
    }
    else if (newMotorSpeed < 0)
    {
        IN1_LOW;
        IN2_HIGH;
    }
    
    pwm = PR3 * abs(newMotorSpeed) / PWM_SCALE_FACTOR;
    currentMotorSpeed = newMotorSpeed;
    
    OC3RS = pwm;
    return SUCCESS;
}


/**
 * @Function DCMotorControl_GetMotorSpeed(void)
 * @param None
 * @return duty cycle of motor 
 * @brief returns speed in units of Duty Cycle (+/- 1000) */
int32_t DCMotorDrive_GetMotorSpeed(void)
{
    return currentMotorSpeed;
}


/**
 * @Function DCMotorDrive_SetBrake(void)
 * @param None
 * @return SUCCESS or FAILURE
 * @brief set the brake on the motor for faster stop */
int32_t DCMotorDrive_SetBrake(void)
{
    IN1_LOW;
    IN2_LOW;
    
    return SUCCESS;
}

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
#ifdef DCMOTOR_TEST
int main(void)
{
    BOARD_Init();
    DCMotorDrive_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    FreeRunningTimer_Init();

    
    uint32_t hz = 100;
    uint32_t hz_velocity = 2;
    unsigned char messageID;
    uint32_t milliseconds = 0;
    
    int32_t payload = 0;
    int32_t operationStatus = 0;
    int32_t encoderRate = 0;
    
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);

    while(1)
    {
        if (Protocol_IsMessageAvailable())
        {
            messageID = Protocol_ReadNextID();
            switch(messageID)
            {
            case (ID_COMMAND_OPEN_MOTOR_SPEED):
                Protocol_GetPayload(&payload);
                payload = Protocol_IntEndednessConversion(payload);
                operationStatus = DCMotorDrive_SetMotorSpeed(payload);
                if (operationStatus == SUCCESS)
                    Protocol_SendMessage(sizeof(operationStatus), ID_COMMAND_OPEN_MOTOR_SPEED_RESP, &operationStatus);
                break;
            }
        }
        
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        
        if (milliseconds > hz_velocity)
        {
            hz_velocity += 2;
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
