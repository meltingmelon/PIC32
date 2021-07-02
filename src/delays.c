/* 
 * File:   delays.c
 * Author: Mel Ho
 * Brief: A basic NOP delay library using the system clock on the UNO32 microcontroller board.
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "delays.h"
#include "xc.h"
#include "BOARD.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define FPB BOARD_GetPBClock()

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function delay_us(void)
 * @param us, delay time in microseconds
 * @return None
 * @brief Starts a NOP delay in microseconds*/
void delay_us(uint16_t us)
{
    us *= FPB / 1000000;
    _CP0_SET_COUNT(0);
    
    while(us > _CP0_GET_COUNT());
}

/**
 * @Function delay_ms(void)
 * @param ms, delay time in milliseconds
 * @return None
 * @brief Starts a NOP delay in milliseconds*/
void delay_ms(uint16_t ms)
{
    delay_us(ms * 1000);
}

/**
 * @Function delay_ns(void)
 * @param ns, delay time in nanoseconds
 * @return None
 * @brief Starts a NOP delay in nanoseconds*/
void delay_ns(uint16_t ns)
{
    delay_us(ns / 1000);
}

