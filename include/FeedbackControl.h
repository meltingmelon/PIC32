/*
 * File:   FeedbackControl.h
 * Author: Mel Ho
 * Brief: Proportional Integratral Derivative Control library for a DC Motor.
 */

#ifndef FEEDBACKCONTROL_H
#define	FEEDBACKCONTROL_H

/*******************************************************************************
 * PUBLIC #DEFINES                                                            *
 ******************************************************************************/
#define FEEDBACK_MAXOUTPUT_POWER 27
#define MAX_CONTROL_OUTPUT (1 << FEEDBACK_MAXOUTPUT_POWER)

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function FeedbackControl_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes the controller to the default values and (P,I,D)->(1, 0, 0)*/
int32_t FeedbackControl_Init(void);

/**
 * @Function FeedbackControl_SetProportionalGain(int32_t newGain);
 * @param newGain, integer proportional gain
 * @return SUCCESS or ERROR
 * @brief sets the new P gain for controller */
int32_t FeedbackControl_SetProportionalGain(int32_t newGain);

/**
 * @Function FeedbackControl_SetIntegralGain(int32_t newGain);
 * @param newGain, integer integral gain
 * @return SUCCESS or ERROR
 * @brief sets the new I gain for controller */
int32_t FeedbackControl_SetIntegralGain(int32_t newGain);

/**
 * @Function FeedbackControl_SetDerivativeGain(int32_t newGain);
 * @param newGain, integer derivative gain
 * @return SUCCESS or ERROR
 * @brief sets the new D gain for controller */
int32_t FeedbackControl_SetDerivativeGain(int32_t newGain);

/**
 * @Function FeedbackControl_GetPorportionalGain(void)
 * @param None
 * @return Proportional Gain
 * @brief retrieves requested gain */
int32_t FeedbackControl_GetProportionalGain(void);

/**
 * @Function FeedbackControl_GetIntegralGain(void)
 * @param None
 * @return Integral Gain
 * @brief retrieves requested gain */
int32_t FeedbackControl_GetIntegralGain(void);

/**
 * @Function FeedbackControl_GetDerivativeGain(void)
 * @param None
 * @return Derivative Gain
 * @brief retrieves requested gain */
int32_t FeedbackControl_GetDerivativeGain(void);

/**
 * @Function FeedbackControl_Update(int32_t referenceValue, int32_t sensorValue)
 * @param referenceValue, wanted reference
 * @param sensorValue, current sensor value
 * @brief performs feedback step according to algorithm in lab manual */
int32_t FeedbackControl_Update(int32_t referenceValue, int32_t sensorValue);

/**
 * @Function FeedbackControl_ResetController(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief resets integrator and last sensor value to zero */
int32_t FeedbackControl_ResetController(void);


#endif	/* FEEDBACKCONTROL_H */

