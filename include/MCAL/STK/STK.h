/*
 * STK.h
 *
 *  Created on: Mar 8, 2024
 *      Author: DELL
 */

#ifndef MCAL_SYSTICK_STK_H_
#define MCAL_SYSTICK_STK_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "STK_Config.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define STK_AHB_DIV_8                  STK_SYSTEM_CLK/8
#define STK_AHB                        STK_SYSTEM_CLK
/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void (*STK_CBF_t)(void);


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

tenu_ErrorStatus MSTK_SetSTKCallBack(STK_CBF_t Fptr);
/**
 * @brief Initialize the SysTick timer.
 *
 * This function initializes the SysTick timer by configuring its control register and choosing the clock source.
 * It is part of the SysTick timer module initialization.
 */
void MSTK_Init (void);
tenu_ErrorStatus MSTK_SetTimerMS (u32 Copy_MSTime);
tenu_ErrorStatus MSTK_StartTimerMSSingle ();
tenu_ErrorStatus MSTK_StartTimerMSPeriodic ();

/**
 * @brief Delay for a specified number of ticks using busy-wait.
 *
 * This function causes the system to wait for a specified number of ticks using busy-wait.
 *
 * @param Copy_Ticks Number of ticks to wait.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MSTK_SetBusyWaitMS (u32 Copy_MSTime);
/**
 * @brief Set a single interval for the SysTick timer.
 *
 * This function sets a single interval for the SysTick timer and associates a callback function to be executed
 * at the end of the interval.
 *
 * @param Copy_Ticks Number of ticks for the interval.
 * @param Fptr Pointer to the callback function.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MSTK_SetIntervalSingleMS (u32 Copy_MSTime, void (*Fptr)(void) );
/**
 * @brief Set a periodic interval for the SysTick timer.
 *
 * This function sets a periodic interval for the SysTick timer and associates a callback function to be executed
 * at the end of each interval.
 *
 * @param Copy_Ticks Number of ticks for the interval.
 * @param Fptr Pointer to the callback function.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MSTK_SetIntervalPeriodicMS (u32 Copy_MSTime, void (*Fptr)(void) );
/**
 * @brief Stop the interval for the SysTick timer.
 *
 * This function stops the SysTick timer interval.
 */
void MSTK_StopIntervalMS (void);
/**
 * @brief Get the elapsed time since the start of the interval.
 *
 * This function calculates and returns the elapsed time since the start of the SysTick timer interval.
 *
 * @param ADD_ElasedTime Pointer to store the elapsed time.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus  MSTK_GetElapsedTimeMS (u32 *ADD_ElasedTime);
/**
 * @brief Get the remaining time until the end of the interval.
 *
 * This function calculates and returns the remaining time until the end of the SysTick timer interval.
 *
 * @param ADD_RemainingTime Pointer to store the remaining time.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus  MSTK_GetRemainingTimeMS (u32 *ADD_RemainingTime);





#endif /* MCAL_SYSTICK_STK_H_ */
