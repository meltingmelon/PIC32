/* 
 * File:   Protocol.c
 * Author: Mel Ho
 * Brief: The Protocol library that interfaces the UNO32 microcontroller with our python control GUI.
 * This custom library is used to send serial communication between our microcontroller and the proprietary
 * serial console provided in this course using UART1 of the UNO32. Please refer to the PIC32 FRM Sect. 21
 * UART for more information on how the UART1 register is being set. 
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <xc.h>
#include <BOARD.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include "Protocol.h"
#include "MessageIDs.h"
#include "delays.h"
#include "CircularBuffer.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define TRUE 1
#define FALSE 0

#define HIGH 1
#define LOW 0

#define NULL_VAL_ERROR 89
#define BUFF_EMPTY 100
#define BUFF_FULL 110
#define NOP 1

#define FPB (BOARD_GetPBClock())
#define BUFFER_LENGTH 135
#define ms_SCALE (FPB/40000);

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define CIRCBUFF_TX_TEST 1
//#define  CHECKSUM_TEST 1
//#define CIRCBUFF_RX_ECHO_TEST 1
//#define SEND_MESSAGE 1
//#define TEST_ENDIANS 1
//#define TEST_LEDS 1
//#define TEST_HARNESS


/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
// Enumerated states for RX buffer state machine.
enum PayloadStateType {
    waitingForHead,
    waitingForLength,
    waitingForMessageID,
    receivingPayload,
    waitingForTail,
    compareChecksum  
};

static char message[MAXPAYLOADLENGTH];
enum PayloadStateType State = waitingForHead;

struct packet onePayload;
packetPointer payloadBuff;

struct circReceiverBuffer rxBuff;
cbRPointer receiverBuffer;

struct circBuffer txBuff;
cbPointer transmitBuffer;

static int32_t iteration = 0;
static unsigned char checksum = 0;
static unsigned char protocol_error = FALSE;

static char isWritingToBuffer = FALSE;
static char transmitCollision = FALSE;

static char isReceivingFromBuffer = FALSE;
static char receiveCollision = FALSE;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
int32_t Protocol_Init()
{
    LEDS_INIT(); // Enable LEDs on UNO32 I/O shield for testing
    
    transmitBuffer = (struct circBuffer*) &txBuff;          // Instantiate TX circular buffer
    newCircBuffer(transmitBuffer);
    
    receiverBuffer = (struct circReceiverBuffer*) &rxBuff;  // Instantiate RX circular buffer 
    newCircReceiverBuffer(receiverBuffer);
 
    U1MODE = LOW;  // Clear UART1
    U1STA = LOW;   

    //U1BRG calculated using eq 21-1 in FRM Sect. 21 UART
    U1BRG = (uint32_t) (FPB / (16 * UART_BAUDRATE)) - 1;  
    
    U1MODEbits.ON = HIGH;    // Enable UART1
    U1MODEbits.RXINV = LOW;
    
    IPC6bits.U1IP = 2;       // UART1 priority set to 2
    
    U1STAbits.URXISEL = LOW;
    U1STAbits.UTXINV = LOW;
    U1STAbits.URXEN = HIGH;  // Enable RX 
    U1STAbits.UTXEN = HIGH;  // Enable TX
    
    IEC0bits.U1RXIE = HIGH;  // Enable RX interrupt
    IFS0bits.U1RXIF = LOW;   // Clear RX interrupt flag
    IEC0bits.U1TXIE = HIGH;  // Enable TX interrupt
    IFS0bits.U1TXIF = LOW;   // Clear TX interrupt flag
 
    return SUCCESS;
}

/**
 * @Function Protocol_SendMessage(unsigned char len, void *Payload)
 * @param len, length of full Payload variable
 * @param Payload, pointer to data, will be copied in during the function
 * @return SUCCESS or ERROR
 * @brief  */
int32_t Protocol_SendMessage(unsigned char len, unsigned char ID, void *Payload)
{
    int32_t i;   
    char ch;
    char *payload = (char*)Payload;
    checksum = 0;
    
    PutChar(HEAD);      // Enqueue the HEAD
    delay_ms(1);

    // Calculate correct length of payload argument + 1 (for message ID)
    PutChar((unsigned char)((int)len + 1));
    delay_ms(1);

    // Initialize the checksum with the ID
    checksum = Protocol_CalcIterativeChecksum(ID, checksum);
    
    PutChar(ID);        // Enqueue the ID
    delay_ms(1);


    // Enqueue the data one byte at a time(and update your checksum)
    for (i=0; i < (unsigned int)len; i++)
    {
        PutChar((payload[i]));
        delay_ms(1);
        ch = (payload[i]);
        checksum = Protocol_CalcIterativeChecksum(ch, checksum);
    }
    
    PutChar(TAIL);     // Enqueue the TAIL
    delay_ms(1);

    PutChar(checksum); // Enqueue the CHECKSUM
    delay_ms(1);

    PutChar('\r');     // Enqueue \r\n
    delay_ms(1);

    PutChar('\n');
    delay_ms(1);

    return SUCCESS;

}

/**
 * @Function Protocol_SendDebugMessage(char *Message)
 * @param Message, Proper C string to send out
 * @return SUCCESS or ERROR
 * @brief Takes in a proper C-formatted string and sends it out using ID_DEBUG
 * @warning this takes an array, do <b>NOT</b> call sprintf as an argument.
 * */
int32_t Protocol_SendDebugMessage(char *Message)
{
    if (Protocol_IsError())
        return ERROR;
    
    unsigned char message_length = strlen(Message);
    Protocol_SendMessage(message_length, ID_DEBUG, Message);
    return SUCCESS;
}

/**
 * @Function Protocol_ReadNextID(void)
 * @param None
 * @return Reads ID of next Packet
 * @brief Returns ID_INVALID if no packets are available
 * */
unsigned char Protocol_ReadNextID(void)
{
    if (!Protocol_IsMessageAvailable())
        return (unsigned char) ID_INVALID;
    
    return (unsigned char)(receiverBuffer->payloads[receiverBuffer->head].ID);
}

/**
 * @Function Protocol_GetPayload(void* payload)
 * @param payload, Memory location to put payload
 * @return SUCCESS or ERROR
 * @brief 
 * */
int32_t Protocol_GetPayload(void* Payload)
{
    if (Protocol_IsError())
    {
        return ERROR;
    }
    *((unsigned int*) Payload) = *((unsigned int*)receiverBuffer->payloads[receiverBuffer->head].buffer);
    receiverBuffer->head = (receiverBuffer->head + 1) % receiverBuffer->length;
    
    return SUCCESS;
}
/**
 * @Function Protocol_IsMessageAvailable(void)
 * @param None
 * @return TRUE if Queue is not Empty
 * @brief 
 * */
char Protocol_IsMessageAvailable(void)
{
    return ((receiverBuffer->head != receiverBuffer->tail));
}

/**
 * @Function Protocol_IsQueueFull(void)
 * @param None
 * @return TRUE is QUEUE is Full
 * @brief 
 * */
char Protocol_IsQueueFull(void)
{
    return (receiverBuffer->head == ((receiverBuffer->tail + 1) % receiverBuffer->length));
}

/**
 * @Function Protocol_IsError(void)
 * @param None
 * @return TRUE if error
 * @brief Returns if error has occurred in processing, clears on read
 * */
char Protocol_IsError(void)
{
    if (protocol_error == TRUE)
    {    
        protocol_error = FALSE;
        return TRUE;
    }
    return FALSE;
}

/**
 * @Function Protocol_ShortEndednessConversion(uint16_t inVariable)
 * @param inVariable, short to convert endedness
 * @return converted short
 * @brief Converts endedness of a short. This is a bi-directional operation so only one function is needed
 * */
uint16_t Protocol_ShortEndednessConversion(uint16_t inVariable)
{
    return (inVariable >> 8) | (inVariable << 8);
}

/**
 * @Function char Protocol_IntEndednessConversion(unsigned int32_t inVariable)
 * @param inVariable, int32_t to convert endedness
 * @return converted short
 * @brief Converts endedness of a int. This is a bi-directional operation so only one function is needed
 * */
uint32_t Protocol_IntEndednessConversion(uint32_t inVariable)
{
    uint32_t b0,b1,b2,b3;

    b0 = (inVariable & 0x000000ff) << 24;
    b1 = (inVariable & 0x0000ff00) << 8;
    b2 = (inVariable & 0x00ff0000) >> 8;
    b3 = (inVariable & 0xff000000) >> 24;
    
    return (b0 | b1 | b2 | b3);
}

/*******************************************************************************
 * PRIVATE FUNCTIONS
 * generally these functions would not be exposed but due to the learning nature of the class they
 * are to give you a theory of how to organize the code internal to the module
 ******************************************************************************/

/**
 * @Function char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum)
 * @param charIn, new char to add to the checksum
 * @param curChecksum, current checksum, most likely the last return of this function, can use 0 to reset
 * @return the new checksum value
 * @brief Returns the BSD checksum of the char stream given the curChecksum and the new char
 * */
unsigned char Protocol_CalcIterativeChecksum(unsigned char charIn, unsigned char curChecksum)
{
    curChecksum = (curChecksum >> 1) + ((curChecksum & 1) << 7);
    curChecksum += charIn;
    curChecksum &= 0xff;
    return curChecksum;
}

/**
 * @Function void Protocol_runReceiveStateMachine(unsigned char charIn)
 * @param charIn, next character to process
 * @return None
 * @brief Runs the protocol state machine for receiving characters, it should be called from 
 * within the interrupt and process the current character. Please see state machine in the Lab 1 Report for
 * more details. */
void Protocol_RunReceiveStateMachine(unsigned char charIn)
{
  switch(State)
  {
      case waitingForHead:
          
          iteration = 0;
          if (charIn == HEAD && !Protocol_IsQueueFull())
          {
              State = waitingForLength;
          }
          break;
          
      case waitingForLength:    
          
          if (charIn > MAXPAYLOADLENGTH || charIn <= 0)
          {
              protocol_error = TRUE;
              State = waitingForHead;
              return;          
          }
          
          receiverBuffer->payloads[receiverBuffer->tail].length = (unsigned int) charIn;
          State = waitingForMessageID;

          break;
          
      case waitingForMessageID:

          if(isInvalidCharacter(charIn))
          {
              protocol_error = TRUE;
              State = waitingForHead;
              return;
          }
          
          receiverBuffer->payloads[receiverBuffer->tail].ID = (int)charIn;
          checksum = Protocol_CalcIterativeChecksum(charIn, checksum);
          
          if (receiverBuffer->payloads[receiverBuffer->tail].ID == ID_LEDS_GET)
          {
              State = waitingForTail;
              return;
          }
          
          State = receivingPayload;
          
          break;
          
          
      case receivingPayload:                
          if (iteration == receiverBuffer->payloads[receiverBuffer->tail].length)
          {
              protocol_error = TRUE;
              State = waitingForHead;
              return;
          }
          
          if (isInvalidCharacter(charIn))
          {
              protocol_error = TRUE;
              State = waitingForHead;
              return;
          }

          
          receiverBuffer->payloads[receiverBuffer->tail].buffer[iteration] = charIn;
          checksum = Protocol_CalcIterativeChecksum(charIn,checksum);
 
          
          if (iteration ==  receiverBuffer->payloads[receiverBuffer->tail].length - 2)
          {
  
              State = waitingForTail;
              return;
          }
          
          iteration++;
          break;
          
          
      case waitingForTail:
          if (charIn != TAIL)
          {
              protocol_error = TRUE;
              State = waitingForHead;
              return;
          }        
          State = compareChecksum;
          break;
          
      case compareChecksum:
          State = waitingForHead;
          if (charIn == checksum)
          {
              checksum = 0;
              if (receiverBuffer->payloads[receiverBuffer->tail].ID == ID_LEDS_SET)
              {
                  LEDS_SET(receiverBuffer->payloads[receiverBuffer->tail].buffer[0]);
                  checksum = 0;
                  iteration = 0;
                  break;
              }

              if (receiverBuffer->payloads[receiverBuffer->tail].ID == ID_LEDS_GET)
            {
                sprintf(message, "%c", PORTE);
                Protocol_SendMessage(1,ID_LEDS_STATE, message);
                checksum = 0;
                iteration = 0;
                break;
            }

            receiverBuffer->tail = (receiverBuffer->tail + 1) % receiverBuffer->length;
            iteration = 0;
            break;
          }
          protocol_error = TRUE;
          checksum = 0;
          iteration = 0;
          break;
      default:
          break;
                  
          
  }
}

/**
 * @Function char PutChar(char ch)
 * @param ch, new char to add to the circular buffer
 * @return SUCCESS or ERROR
 * @brief adds to circular buffer if space exists, if not returns ERROR
* */
int32_t PutChar(char ch)
{
    if (transmitBuffer == NULL)
        return ERROR;
    
    if(!isFull(transmitBuffer))
    {    
        isWritingToBuffer = TRUE;
        write(transmitBuffer, ch);
        isWritingToBuffer = FALSE;
        
        if(U1STAbits.TRMT)
            IFS0bits.U1TXIF = 1;
    }
        
    return SUCCESS;
}



/*******************************************************************************
 * INTERRUPTS                                                                  *
 ******************************************************************************/
/**
 * @Interrupt IntUart1Handler(void)
 * @param None
 * @return None
 * @brief Interrupt triggers whenever the U1RXIF or U1TTXIF are triggered.
 * If the RX flag is triggered (i.e. a message is being received, the UNO32 runs the
 * RX state machine. Else if the TX flag is triggered (i.e. a transmission is being sent), we
 * populate the TX buffer register with our character.*/
void __ISR ( _UART1_VECTOR ) IntUart1Handler(void) {
 
    if (IFS0bits.U1RXIF)
    {
        IFS0bits.U1RXIF = 0;
        if (!isReceivingFromBuffer)
        {
          Protocol_RunReceiveStateMachine(U1RXREG);
        }
        else
            receiveCollision = TRUE;
    }
    if (IFS0bits.U1TXIF)
    {
        IFS0bits.U1TXIF = 0;
        if(!isEmpty(transmitBuffer))
        {
            if (!isWritingToBuffer)
            {
                U1TXREG = read(transmitBuffer);
            }
            else {
                transmitCollision = TRUE;
            }
        }
    }
}

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/

#ifdef CHECKSUM_TEST
#include "xc.h"
#include "BOARD.h"
#include "Protocol.h"
#include "stdio.h"
#include "string.h"

int32_t main(void)
{
    BOARD_Init();
    Protocol_Init();
    unsigned char checksum = (unsigned char) 0x0000;
    unsigned char payload1 = (unsigned char) 0x80;
    unsigned char payload2 = (unsigned char) 0x7f;
    unsigned char payload3 = (unsigned char) 0x35;
    

    checksum = Protocol_CalcIterativeChecksum(payload1, checksum);
    checksum = Protocol_CalcIterativeChecksum(payload2, checksum);
    checksum = Protocol_CalcIterativeChecksum(payload3, checksum);
    PutChar(checksum);
    return 0;
}
#endif

#ifdef CIRCBUFF_TX_TEST
#include "xc.h"
#include "BOARD.h"
#include <sys/attribs.h>
#include <GenericTypeDefs.h>
#include <string.h>
#include "stdio.h"

#define BUFFER_SIZE 50
int32_t main(void)
{
    BOARD_Init();
    Protocol_Init();
    char buffer[BUFFER_SIZE];
    int32_t i;
    sprintf(buffer, "Testing a bunch of different strings. It all began when...");
   
    for (i = 0; i < strlen(buffer); i++)
        PutChar(buffer[i]);
    return 0;
}
#endif

#ifdef CIRCBUFF_RX_ECHO_TEST
#include "xc.h"
#include "BOARD.h"
#include <sys/attribs.h>
#include <GenericTypeDefs.h>
#include <string.h>
#include "stdio.h"

#define BUFFER_SIZE 50
int32_t main(void)
{
    BOARD_Init();
    Protocol_Init();
    
    while(TRUE)
        if(getBuffLength(transmitBuffer) <= BUFFER_LENGTH)
        if (getBuffLength(oldReceiverBuffer) != 0)
            PutChar(GetChar());
    
    return 0;
}
#endif

#ifdef SEND_MESSAGE
#include "xc.h"
#include "BOARD.h"
#include "Protocol.h"
#include "stdio.h"
#include "string.h"

int32_t main(void)
{
    BOARD_Init();
    Protocol_Init();
    
    payload->payloadArray[0] = (unsigned char) 74;
    payload->payloadArray[1] = (unsigned char) 65;
    payload->payloadArray[2] = (unsigned char) 73;
    unsigned char length = (unsigned char)strlen(payload->payloadArray);
    unsigned char ID = (unsigned char) 0x81;
    Protocol_SendMessage(length, ID, payload->payloadArray);
    return 0;
}
#endif

#ifdef TEST_ENDIANS
int32_t main(void)
{
    BOARD_Init();
    Protocol_Init();
    
    short shortTestValue = 0xDEAD;
    short shortResultValue;
    int32_t intTestValue = 0xDEADBEEF;
    int32_t intResultValue;

    sprintf(testMessage, "State Machine Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);

    sprintf(testMessage, "hello, world!");
    Protocol_SendDebugMessage(testMessage);
//    
    shortResultValue = Protocol_ShortEndednessConversion(shortTestValue);
    sprintf(testMessage, "Short Endedness Conversion: IN: 0x%X OUT: 0x%X", shortTestValue&0xFFFF, shortResultValue&0xFFFF);
    Protocol_SendDebugMessage(testMessage);
            
    intResultValue = Protocol_IntEndednessConversion(intTestValue);
    sprintf(testMessage, "int32_t Endedness Conversion: IN: 0x%X OUT: 0x%X", intTestValue, intResultValue);
    Protocol_SendDebugMessage(testMessage);


    
    return 0;
}
#endif

#ifdef TEST_LEDS
int32_t main(void)
{
    BOARD_Init();
    Protocol_Init();
    short shortTestValue = 0xDEAD;
    short shortResultValue;
    int32_t intTestValue = 0xDEADBEEF;
    int32_t intResultValue;
    
//    sprintf(testMessage, "State Machine Test Compiled at %s %s", __DATE__, __TIME__);
//    Protocol_SendDebugMessage(testMessage);
//    sprintf(testMessage, "hello, world!");
//    Protocol_SendDebugMessage(testMessage);
////    
//    shortResultValue = Protocol_ShortEndednessConversion(shortTestValue);
//    sprintf(testMessage, "Short Endedness Conversion: IN: 0x%X OUT: 0x%X", shortTestValue&0xFFFF, shortResultValue&0xFFFF);
//    Protocol_SendDebugMessage(testMessage);
//            
//    intResultValue = Protocol_IntEndednessConversion(intTestValue);
//    sprintf(testMessage, "int32_t Endedness Conversion: IN: 0x%X OUT: 0x%X", intTestValue, intResultValue);
//    Protocol_SendDebugMessage(testMessage);
    while(1);
//    LEDS_SET(0x4F);
    return 0;
}
#endif
    
#ifdef TEST_HARNESS
    int32_t main(void)
{
    BOARD_Init();
    Protocol_Init();
    char testMessage[MAXPAYLOADLENGTH];
    
    sprintf(testMessage, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(testMessage);
    
    short shortTestValue = 0xDEAD;
    short shortResultValue;
    int32_t intTestValue = 0xDEADBEEF;
    int32_t intResultValue;
    
    shortResultValue = Protocol_ShortEndednessConversion(shortTestValue);
    sprintf(testMessage, "Short Endedness Conversion: IN: 0x%X OUT: 0x%X", shortTestValue&0xFFFF, shortResultValue&0xFFFF);
    Protocol_SendDebugMessage(testMessage);
    
    
    intResultValue = Protocol_IntEndednessConversion(intTestValue);
    sprintf(testMessage, "int32_t Endedness Conversion: IN: 0x%X OUT: 0x%X", intTestValue, intResultValue);
    Protocol_SendDebugMessage(testMessage);
    
    unsigned int32_t pingValue = 0xfff;
    checksum = 0;
    while (1) {
        if (Protocol_IsMessageAvailable()) {
            if (Protocol_ReadNextID() == ID_PING) {
                // send pong in response here
                Protocol_GetPayload(&pingValue);
                pingValue = Protocol_IntEndednessConversion(pingValue);  
                pingValue >>= 1;
                pingValue = Protocol_IntEndednessConversion(pingValue);
                Protocol_SendMessage(4, ID_PONG, &pingValue);
            }
        }
    }
    while (1);
    return 0;
}
#endif