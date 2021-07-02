/* 
 * File:   CircularBuffer.h
 * Author: Mel Ho
 * Brief: Circular buffer library used to to handle TX and RX buffer operations in our Protocol library.
 *  Users will never interface with this library directly.
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "CircularBuffer.h"
#include "stdint.h"
#include <xc.h>
#include <BOARD.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include "MessageIDs.h"
#include "delays.h"


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function newPacket(void)
 * @param None
 * @return packet, a new packet buffer
 * @brief Initializes a new packet for the circular buffer
 * */
packet newPacket()
{
    int32_t i;
    packet pd;
    for (i = 0; i < MAXPAYLOADLENGTH; i++)
       pd.buffer[i] = 0;
    
    pd.length = MAXPAYLOADLENGTH;
    pd.ID = ID_INVALID;
    return pd;
}

/**
 * @Function isInvalidCharacter(unsigned char charIn)
 * @param charIn, unsigned char
 * @return TRUE or FALSE (1 or 0)
 * @brief Checks to see if a character is equal to HEAD or TAIL.
 * */
uint32_t isInvalidCharacter(unsigned char charIn)
{
    return charIn == HEAD || charIn == TAIL;
}

/**
 * @Function newCircReceiverBuffer(cbRPointer circRBuff)
 * @param circBuff, RX circular buffer pointer
 * @return None
 * @brief Initializes RX circular buffer
 * */
void newCircReceiverBuffer(cbRPointer circRBuff)
{
    circRBuff->head = 0;
    circRBuff->tail = 0;
    circRBuff->length = PACKETBUFFERSIZE;
    
}

/**
 * @Function newCircReceiverBuffer(cbRPointer circRBuff)
 * @param circBuff, circular buffer pointer
 * @return None
 * @brief Initializes circular buffer
 * */
void newCircBuffer(cbPointer circBuff)
{
    int32_t i;
    for (i = 0; i < MAXPAYLOADLENGTH; i++)
        circBuff->buffer[i] = 0;
    
    circBuff->head = 0;
    circBuff->tail = 0;
    circBuff->length = MAXPAYLOADLENGTH;
    
}

/**
 * @Function getBuffLength(cbPointer circBuff)
 * @param circBuff, circular buffer pointer
 * @return buffer length or NULL_VAL_ERROR
 * @brief Returns the length of the buffer. If the buffer is NULL
 * return a NULL_VAL_ERROR
 * */
uint32_t getBuffLength(cbPointer circBuff)
{
    if (circBuff != NULL)
    {
        int32_t tail = circBuff->tail;
        if (circBuff->tail < circBuff->head)
            tail = (circBuff->length + circBuff->tail);
 
        return (tail - circBuff->head);
    }
    else
        return NULL_VAL_ERROR;
}

/**
 * @Function getBuffLength(cbPointer circRBuff)
 * @param circRBuff, circular buffer pointer
 * @return buffer length or NULL_VAL_ERROR
 * @brief Returns the length of the RX buffer. If the buffer is NULL
 * return a NULL_VAL_ERROR
 * */
uint32_t getReceiverBuffLength(cbRPointer circRBuff)
{
    if (circRBuff != NULL)
    {
        int32_t tail = circRBuff->tail;
        if (circRBuff->tail < circRBuff->head)
            tail = (circRBuff->length + circRBuff->tail);
 
        return (tail - circRBuff->head);
    }
    else
        return NULL_VAL_ERROR;
}

/**
 * @Function isEmpty(cbPointer circBuff)
 * @param circBuff, circular buffer pointer
 * @return TRUE or FALSE (1 or 0)
 * @brief Checks to see if circular buffer is empty
 * */
uint32_t isEmpty(cbPointer circBuff)
{
    return (circBuff->head == circBuff->tail);
}

/**
 * @Function isFull(cbPointer circBuff)
 * @param circBuff, circular buffer pointer
 * @return TRUE or FALSE (1 or 0)
 * @brief Checks to see if circular buffer is full
 * */
uint32_t isFull(cbPointer circBuff)
{
    return (circBuff->head == ((circBuff->tail + 1) % circBuff->length));
}


/**
 * @Function read(void* payload)
 * @param circBuff, circular buffer pointer
 * @return payload, character from buffer
 * @brief Returns characters stored at the HEAD of the circular buffer
 * and increments HEAD */
unsigned char read(cbPointer circBuff)
{
    unsigned char payload;
    payload = circBuff->buffer[circBuff->head];
    circBuff->head = (circBuff->head + 1) % circBuff->length;
    return payload;
}

/**
 * @Function write(void* payload)
 * @param circBuff, data,  circular buffer and payload data
 * @return None
 * @brief Writes data into the TAIL of the circular buffer
 * and increments TAIL*/
void write(cbPointer circBuff, unsigned char data)
{
    circBuff->buffer[circBuff->tail] = data;
    circBuff->tail = (circBuff->tail + 1) % circBuff->length;
}

