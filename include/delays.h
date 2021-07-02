/* 
 * File:   delays.h
 * Author: Mel Ho
 * Brief: A basic NOP delay library using the system clock on the UNO32 microcontroller board.
 */

#ifndef DELAYS_H 
#define	DELAYS_H 

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/

/**
 * @Function delay_us(void)
 * @param us, delay time in microseconds
 * @return None
 * @brief Starts a NOP delay in microseconds*/
void delay_us(uint16_t us);

/**
 * @Function delay_ms(void)
 * @param ms, delay time in milliseconds
 * @return None
 * @brief Starts a NOP delay in milliseconds*/
void delay_ms(uint16_t ms);

/**
 * @Function delay_ns(void)
 * @param ns, delay time in nanoseconds
 * @return None
 * @brief Starts a NOP delay in nanoseconds*/
void delay_ns(uint16_t ns);


#endif
