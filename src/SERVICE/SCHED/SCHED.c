/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"
#include "SERVICE/SCHED/SCHED.h"
#include "HLED/LED.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
extern Runnable_tstr Runnables[_RUNNABLE_NUM];


typedef struct 
{
    Runnable_tstr *Runnable;
    u32 RemainTimeMs;
}RunnableInfo_tstr;

RunnableInfo_tstr Runnable_array[_RUNNABLE_NUM];


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
 u32 PendingTicks=0;


/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
/**
 * @brief SysTick Timer Interrupt Handler.
 *
 * This function is the interrupt handler for the SysTick timer. It is automatically called when the SysTick
 * timer reaches zero, indicating the end of the programmed interval. The behavior of the handler depends on
 * whether the interval is periodic or single.
 */
 void TickCb(void);


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
void Sched_Init(void)
{
	u32 idx=0;
	MSTK_Init();
    for (idx=0 ; idx<_RUNNABLE_NUM ; idx++)
    {
    	Runnable_array[idx].Runnable=&Runnables[idx];
    /* Runnable_array[idx].Runnable->CallBack=Runnables[idx].CallBack;
        Runnable_array[idx].Runnable->FirstDelayMs=Runnables[idx].FirstDelayMs;
        Runnable_array[idx].Runnable->PeriodicityMs=Runnables[idx].PeriodicityMs;
        Runnable_array[idx].Runnable->Priority=Runnables[idx].Priority;*/
        Runnable_array[idx].RemainTimeMs=Runnables[idx].FirstDelayMs;


    }


    MSTK_SetTimerMS(TICK_TIME);
    MSTK_SetSTKCallBack(TickCb);
	//we start sys tick at sched_stat
	
}

/**
 * @brief SysTick timer callback function.
 *
 * This function is called when the SysTick timer reaches zero. It increments the PendingTicks counter,
 * indicating the occurrence of a system tick.
 */
void TickCb(void)
{
	PendingTicks++;
}

tenu_ErrorStatus StartSched(void)
{
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
	Local_ErrorStatus = MSTK_StartTimerMSPeriodic();
	while(1)
	{
		// if pendingTicks is greater than one, it means
		// that cpu load is over 100%, because two syscalls
		// occurred when sched was already running
		if(PendingTicks)
		{
			PendingTicks--;

            Sched();
			
		}
	}
    return Local_ErrorStatus;
}

tenu_ErrorStatus Sched(void){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u32 idx=0;

    for(idx =0 ; idx<_RUNNABLE_NUM ; idx++)
    {
        if(Runnable_array[idx].Runnable->CallBack&&Runnable_array[idx].RemainTimeMs==0)
        {
            Runnable_array[idx].Runnable->CallBack();
            Runnable_array[idx].RemainTimeMs=Runnable_array[idx].Runnable->PeriodicityMs;

        }
        else{
            /*do nothing*/
        }
        Runnable_array[idx].RemainTimeMs-=TICK_TIME;

    } 


    return Local_ErrorStatus;


}


