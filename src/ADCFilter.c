/* 
 * File:   ADCFilter.c
 * Author: Mel Ho
 * Brief: Analog-to-Digital Conversion filtering library for the UNO32 microcontroller's built-in 10-bit ADC.
 * See the PIC32 FRM Sect. 17 for more information about the AD registers.
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <proc/p32mx340f512h.h>
#include <xc.h>
#include "ADCFilter.h" // The header file for this source file.
#include "BOARD.h"
#include "stdio.h"
#include "string.h"
#include "MessageIDs.h"
#include <sys/attribs.h>
#include "FrequencyGenerator.h"
#include "FreeRunningTimer.h"
#include "Protocol.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define DESIRED_PINS 0b1111101011101011 // 0xFAEB
#define CSSL_PINS ~(DESIRED_PINS)
#define TPB_K 348

#define UNSIGNED_16BIT_INT 0b000
#define AUTO_CONVERT 0b111
#define AUTO_SAMPLE 1

#define LOW 0
#define HIGH 1

#define INTERNAL_VOLT_REF 0
#define SCAN_MODE 1
#define INTERRUPT_AT_FOURTH 3
#define BUFFER_16BIT 0

#define DISABLED 0
#define ENABLED 1

#define CLEAR 0

#define USE_PERIPHERAL_CLK 0
#define ADC_CLOCK_PRESCALER 173 //((TPB_K / 2) - 1)
#define SAMPLE_TIME 16

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define ADC_TEST

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static int16_t ADCFilter[NUM_OF_CHANNELS][FILTERLENGTH];
static int16_t ADCData[NUM_OF_CHANNELS][FILTERLENGTH];

static int16_t lastIndex = 0;
static unsigned char message[MAXPAYLOADLENGTH];

union adcReading {
    struct {
        int16_t unfiltered;
        int16_t filtered;
    }; unsigned char aschar[4];
};

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function ADCFilter_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes ADC system along with naive filters */
int32_t ADCFilter_Init(void)
{
    AD1CON1 = DISABLED;
    IEC1bits.AD1IE = DISABLED;
   
    AD1PCFGbits.PCFG2 = LOW;
    AD1PCFGbits.PCFG4 = LOW;
    AD1PCFGbits.PCFG8 = LOW;
    AD1PCFGbits.PCFG10 = LOW;
    
    AD1CON1bits.FORM = UNSIGNED_16BIT_INT;
    AD1CON1bits.SSRC = AUTO_CONVERT;
    
    AD1CON2bits.VCFG = INTERNAL_VOLT_REF;
    AD1CON2bits.CSCNA = SCAN_MODE;
    AD1CON2bits.SMPI = INTERRUPT_AT_FOURTH; // based on A0-A3
    AD1CON2bits.BUFM = BUFFER_16BIT;
    
    AD1CON3bits.ADRC = USE_PERIPHERAL_CLK;
    AD1CON3bits.SAMC = SAMPLE_TIME;
    AD1CON3bits.ADCS = ADC_CLOCK_PRESCALER;
    
    AD1CSSLbits.CSSL2 = HIGH;
    AD1CSSLbits.CSSL4 = HIGH;
    AD1CSSLbits.CSSL8 = HIGH;
    AD1CSSLbits.CSSL10 = HIGH;
    

    IFS1bits.AD1IF = CLEAR;
    IEC1bits.AD1IE = ENABLED;
    IPC6bits.AD1IP = 5;
    AD1CON1bits.ON = ENABLED;
    AD1CON1bits.ASAM = AUTO_SAMPLE;
    return SUCCESS;

}

/**
 * @Function ADCFilter_RawReading(int16_t pin)
 * @param pin, which channel to return
 * @return un-filtered AD Value
 * @brief returns current reading for desired channel */
int16_t ADCFilter_RawReading(int16_t pin)
{
    return ADCData[pin][lastIndex];
}

/**
 * @Function ADCFilter_FilteredReading(int16_t pin)
 * @param pin, which channel to return
 * @return Filtered AD Value
 * @brief returns filtered signal using weights loaded for that channel */
int16_t ADCFilter_FilteredReading(int16_t pin)
{
    int16_t filteredData = 0;
    filteredData = ADCFilter_ApplyFilter(ADCFilter[pin],ADCData[pin], lastIndex);
    return filteredData;
}

/**
 * @Function int16_t ADCFilter_ApplyFilter(int16_t filter[], int16_t values[], int16_t startIndex)
 * @param filter, pointer to filter weights
 * @param values, pointer to circular buffer of values
 * @param startIndex, location of first sample so filter can be applied correctly
 * @return Filtered and Scaled Value
 * @brief returns final signal given the input arguments
 * @warning returns a short but internally calculated value should be an int */
int16_t ADCFilter_ApplyFilter(int16_t filter[], int16_t values[], int16_t startIndex)
{
    int32_t filteredValue = 0;
    int16_t i =  startIndex;
    uint32_t j;
    
    for (j = 0; j < FILTERLENGTH; j++)
       
    {
        i--;
        if (i < 0 || i == -1)
            i = FILTERLENGTH - 1;

        filteredValue += (filter[j] * values[i]);      
    }
    filteredValue = ((filteredValue >> 15) & 0xffff); // divide by 2^15 and mask bits.

    return ((int16_t)filteredValue);
}

/**
 * @Function ADCFilter_SetWeights(int16_t pin, int16_t weights[])
 * @param pin, which channel to return
 * @param pin, array of int16_ts to load into the filter for the channel
 * @return SUCCESS or ERROR
 * @brief loads new filter weights for selected channel */
int32_t ADCFilter_SetWeights(int16_t pin, int16_t weights[])
{
    uint32_t i;
    for (i = 0; i < FILTERLENGTH; i++)
    {
        ADCFilter[pin][i] = weights[i];
    }

    return SUCCESS;
}

/*******************************************************************************
 * INTERRUPTS                                                                  *
 ******************************************************************************/
/**
 * @Interrupt SPI2Interrupt(void)
 * @param None
 * @return None
 * @brief Loads the ADC buffer with the latest input readings for each analog input.*/
void __ISR(_ADC_VECTOR) ADCIntHandler(void)
{
    IFS1bits.AD1IF = 0;
    
    ADCData[0][lastIndex] = ADC1BUF0;
    ADCData[1][lastIndex] = ADC1BUF1;
    ADCData[2][lastIndex] = ADC1BUF2;
    ADCData[3][lastIndex] = ADC1BUF3;
    
    lastIndex = (lastIndex + 1) % FILTERLENGTH; 
}


/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
#ifdef ADC_TEST
int main()
{
    BOARD_Init();
    FreeRunningTimer_Init();
    FrequencyGenerator_Init();
    Protocol_Init();
    ADCFilter_Init();
    
    uint32_t milliseconds = 0;
    uint32_t hz = 10;
    unsigned char channel = 0;
    unsigned char messageID;
    uint32_t i;
    uint32_t freqState = 0;
    int16_t CurFrequency;
    
    uint32_t operationStatus;
    
//    union filters filterValues;
    int16_t filters[FILTERLENGTH];
    union adcReading adcValues;
    
    
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    while (1)
    {
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds > hz)
        {
            hz += 10;
            if (Protocol_IsMessageAvailable())
            {
                messageID = Protocol_ReadNextID();
                sprintf(message, "message ID: %d",messageID);
                Protocol_SendDebugMessage(message);
                switch(messageID)
                {

                case (ID_ADC_SELECT_CHANNEL):
                    Protocol_GetPayload(&channel);
                    sprintf(message, "channel: %c", channel);
                    Protocol_SendDebugMessage(message);
                    Protocol_SendMessage(sizeof(channel), ID_ADC_SELECT_CHANNEL_RESP, &channel);
                    break;

                case (ID_ADC_FILTER_VALUES):
                    Protocol_GetPayload(&filters);
                    Protocol_SendMessage(sizeof(filters), ID_DEBUG, &filters);
                    Protocol_SendMessage(sizeof(filters[10]), ID_DEBUG, &filters[10]);
                    for (i=0; i < FILTERLENGTH; i++)
                    {
                        filters[i] = Protocol_int16_tEndednessConversion(filters[i]);
                    }
                    Protocol_SendMessage(sizeof(filters), ID_DEBUG, &filters);
                    Protocol_SendMessage(sizeof(filters[10]), ID_DEBUG, &filters[10]);
                    operationStatus = ADCFilter_SetWeights(channel, filters);
                    if (operationStatus == SUCCESS)
                    {
                        operationStatus = Protocol_IntEndednessConversion(operationStatus);
                        Protocol_SendMessage(sizeof(ADCFilter[channel]), ID_DEBUG, &ADCFilter[channel]);
                        Protocol_SendMessage(sizeof(ADCFilter[channel][10]), ID_DEBUG, &ADCFilter[channel][10]);
                        Protocol_SendMessage(sizeof(operationStatus), ID_ADC_FILTER_VALUES_RESP, &operationStatus);
                    }
                    break;
                    
                case (ID_LAB3_SET_FREQUENCY):
                    Protocol_GetPayload(&CurFrequency);
                    CurFrequency = Protocol_int16_tEndednessConversion(CurFrequency);
                    sprintf(message,"Current Freq: %i", CurFrequency);
                    Protocol_SendDebugMessage(message);
                    FrequencyGenerator_SetFrequency(CurFrequency);
                    break;
                    
                case (ID_LAB3_FREQUENCY_ONOFF):
                    Protocol_GetPayload(&freqState);
                    if (freqState)
                    {
                        FrequencyGenerator_On();
                        sprintf(message,"ON");
                        Protocol_SendDebugMessage(message);
                    }
                    else
                    {
                        FrequencyGenerator_Off();
                        sprintf(message,"OFF");
                        Protocol_SendDebugMessage(message);
                    }
                    break;
                }
            }
            
//            Protocol_SendMessage(&ADCData[channel], ID_DEBUG, &ADCData[channel]);
//            adcValues.filtered = ADCFilter_FilteredReading(channel);
            adcValues.unfiltered = Protocol_int16_tEndednessConversion(ADCFilter_RawReading(channel));
            adcValues.filtered = Protocol_int16_tEndednessConversion(ADCFilter_FilteredReading(channel));
            Protocol_SendMessage(sizeof(adcValues.aschar), ID_ADC_READING, &adcValues.aschar);
            
//                LEDS_SET(0x0);
        }        
    }
    
    
    
    return 0;
}
#endif