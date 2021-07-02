/* 
 * File:   leds.h
 * Author: Mel Ho
 *
 * Created on December 19, 2012, 2:08 PM
 */

#ifndef LEDS_H
#define	LEDS_H

#include <stdint.h>
#include <xc.h>
#include <GenericTypeDefs.h>
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

/** LEDS_Init()
 *  initializes LEDS to outputs in TRIS and off with LAT
 **/
void LEDS_Init();

/**
 * 
 * @return an array of the different LED States
 */
uint8_t LEDS_Get();

/**
 * 
 * @param x
 */
void LEDS_Set(uint8_t x);

#endif