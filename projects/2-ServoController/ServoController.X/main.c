/*
 * File:   main.c
 * Author: Mel Ho
 *
 * This program controls an RC servo using either an ultrasonic sensor or the ASD5047D rotary encoder.
 * The PIC32 microcontroller 
 * Created on February 8, 2021, 10:54 AM
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "xc.h"
#include "BOARD.h"
#include "stdio.h"
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "MessageIDs.h"
#include "RCServo.h"
#include "RotaryEncoder.h"
#include "string.h"
#include "stdio.h"
#include "delays.h"
#include "PingSensor.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define PING_SENSOR 0
#define ENCODER 1

#define ANGLE_DIFF 120 // 120 deg
#define ANGLE_DIFF_FRACTIONAL 0.00833 // 1 / 120

#define IN_RANGE 0x02
#define UNDER_RANGE 0x04
#define OVER_RANGE 0x01

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static char message[MAXPAYLOADLENGTH];

static uint16_t rawAngle = 0;
static int16_t angleConversion = 0;
static uint32_t mappedAngle = 0;

static float lerpRatio = 0; //linear interpolation ratio

static uint16_t pingValue = 0;
static int16_t cmToAngle = 0;
static uint16_t angleToTicks = 0;

static unsigned char ID;
static uint32_t payload;

static uint32_t milliseconds;
static uint32_t desiredMs = 50;

union AngleData {
    struct {
        signed int Angle;
        unsigned char status;
    };
    char asChar[5];
};

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function lerp(uint32_t min, uint32_t max, float ratio)
 * @param min, max, ratio, min, max, lerp ratio of our mapping.
 * @return value between min and max inputs
 * @brief Linear interpolation function used to map our encoder angles into a range between 600-2400 motor ticks.*/
uint32_t lerp(uint32_t min, uint32_t max, float ratio)
{
    return min + ratio * (max - min);
}

/*******************************************************************************
 * MAIN  PROGRAM                                                               *
 ******************************************************************************/
int main(void)
{
    BOARD_Init();
    FreeRunningTimer_Init();
    RCServo_Init();
    PingSensor_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_BLOCKING_MODE);
    
    // Send ID_DEBUG Message of Compilation Date
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    // Holds Control Mode
    uint32_t controlMode = ENCODER;

    while(1)
    {
        ID = Protocol_ReadNextID();
        Protocol_GetPayload(&payload);
        
        union AngleData angleReport; // angle report for the encoder
        

        // Switch between the inputs (encoder/ping) based on the ID_LAB2_INPUT_SELECT message
        if (ID == ID_LAB2_INPUT_SELECT)
            controlMode = payload;
        
        switch(controlMode)
        {
            case(PING_SENSOR):
              
              // Get Ping Value in cm
              pingValue = PingSensor_GetDistance();
              
              // checks to see if ping value is between 25cm-125cm
              if (pingValue >= 25 && pingValue <= 125)
              {
                // Converts cm to Angles (from lab 2 for += 60 deg)
                cmToAngle = -60 + (120 * ((pingValue * 0.01) - 0.25)); 
                
                // Scaling angles to tick values
                angleToTicks = 1500 + (cmToAngle * 15);
                
                // binds ping values to max and min pulse rates
                if (angleToTicks > RC_SERVO_MAX_PULSE)
                    angleToTicks = RC_SERVO_MAX_PULSE;
                
                if (angleToTicks < RC_SERVO_MIN_PULSE)
                    angleToTicks = RC_SERVO_MIN_PULSE;
                
                RCServo_SetPulse(angleToTicks);
                
              }
                break;
                
            case(ENCODER):
                
                rawAngle = RotaryEncoder_ReadRawAngle();
                
                // 0-360 deg conversion
                //Converts raw angle ticks to degrees. 360 / 16200 (largest deg/tick size) = 0.02222
                angleConversion = (rawAngle * .022);
                
                // converts from 0-360 deg => +-180 deg
                if (angleConversion > 179)
                    angleConversion = angleConversion - 360;
                
                // Linear interpolation is used to map the converted angles to values that fit the range of 600-2400us
                // normalization: (angle + (angle_difference / 2 )) / angle_difference. In this case angle_diff = 120 deg
                lerpRatio = (angleConversion + (ANGLE_DIFF / 2)) * ANGLE_DIFF_FRACTIONAL;
                
                // returns mapped values into motor us 
                mappedAngle = lerp(RC_SERVO_MIN_PULSE,RC_SERVO_MAX_PULSE,lerpRatio);
                
                // Converts degrees to millidegrees and Big Endian
                angleReport.Angle = Protocol_IntEndednessConversion((signed int)(angleConversion * 1000));
                
                // Checks to see if the encoder angle is between the RC-SERVO range (+-60 deg)
                if (mappedAngle >= RC_SERVO_MIN_PULSE && mappedAngle <= RC_SERVO_MAX_PULSE)
                {
                    angleReport.status = IN_RANGE;
                    RCServo_SetPulse(mappedAngle);
                }
                else if (mappedAngle < RC_SERVO_MIN_PULSE)
                    angleReport.status = UNDER_RANGE;
                
                else if (mappedAngle > RC_SERVO_MAX_PULSE)
                    angleReport.status = OVER_RANGE;
                
                break;
        }

        // Send Angle Report at 20Hz
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds >= desiredMs) 
        {
            desiredMs += 50;
            Protocol_SendMessage(5, ID_LAB2_ANGLE_REPORT, &angleReport);
        }
        
    }
    return 0;
}

