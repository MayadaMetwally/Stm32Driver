/***********************************************************************************************/
/* File Name:	Scheduler.c                                                                    */
/* Module	: 	Scheduler						                                               */
/* Layer	:	Scheduler										                               */
/* AUTHOR   :   Lina Nassar                                                                    */
/***********************************************************************************************/

#include "SERVICE/SCHED/Scheduler.h"
#include "SERVICE/SCHED/Scheduler_Config.h"
#include "STK/Systick.h"

typedef struct
{
    Runnable_t *Runnable;
    uint32_t    RemainingTimeMs;

} RunnableInfo_t;


extern const Runnable_t Runnable[_Runnables_Num];
RunnableInfo_t RunnableInfo[_Runnables_Num];
uint32_t PendingTicks=0;


static void Sched_TickCb()
{
    PendingTicks++;
}

static void Scheduler()
{
    uint32_t itr;

    for(itr=0 ; itr<_Runnables_Num ; itr++ )
    {
        if((RunnableInfo[itr].Runnable->cb) && (RunnableInfo[itr].RemainingTimeMs == 0))
        {
            RunnableInfo[itr].Runnable->cb();
            RunnableInfo[itr].RemainingTimeMs=RunnableInfo[itr].Runnable->PeriodicityMS;   
        }
        RunnableInfo[itr].RemainingTimeMs -= Sched_TickTime_Ms;
    }
}


void Sched_init(void)
{
    uint32_t itr;

    for(itr = 0; itr<_Runnables_Num; itr++)
    {
        RunnableInfo[itr].Runnable = &Runnable[itr];
        RunnableInfo[itr].RemainingTimeMs = Runnable[itr].FirstDelayMS;
    }

    //SysTick_ConfigureClk(STK_SrcClk_ExtRef);
    SysTick_SetTimeMs(Sched_TickTime_Ms);
    SysTick_SetCallBack(Sched_TickCb);
}

void Sched_Start(void)
{
    SysTick_Start(STK_Mode_Periodic);

    while(1)
    {
        if(PendingTicks)
        {
            PendingTicks--;
            Scheduler();
        }
    }

}

