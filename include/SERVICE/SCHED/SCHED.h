#ifndef SERVICE_SCHED_SCHED_H_
#define SERVICE_SCHED_SCHED_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "SCHED_Config.h"
#include "STK/STK.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define TICK_TIME   1


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void(*RunabbleCb_t) (void);

typedef struct
{
   u32 PeriodicityMs;
   u32 Priority;
   RunabbleCb_t CallBack;
   u32 FirstDelayMs; 

}Runnable_tstr;



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
/**
 * @brief Initializes the scheduler.
 *
 * This function initializes the scheduler by setting up the SysTick timer and initializing the Runnable_array
 * with the provided Runnables data.
 */
void Sched_Init(void);

/**
 * @brief Starts the scheduler.
 *
 * This function starts the scheduler by initiating the SysTick timer and entering a loop where it continuously
 * checks for pending ticks. When a tick is detected, the scheduler function (Sched) is called.
 *
 * @return tenu_ErrorStatus: Error status, LBTY_OK if successful, LBTY_NOK if an error occurs.
 */
tenu_ErrorStatus StartSched(void);
/**
 * @brief Scheduler function.
 *
 * This function iterates through the Runnable_array, checks if a runnable task is ready to execute,
 * and calls its associated callback function if conditions are met. It also updates the remaining time
 * for each runnable task.
 *
 * @return tenu_ErrorStatus: Error status, LBTY_OK if successful, LBTY_NOK if an error occurs.
 */
tenu_ErrorStatus Sched(void);

#endif
