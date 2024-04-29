/***********************************************************************************************/
/* File Name:	Scheduler_Config.h                                                             */
/* Module	: 	Scheduler						                                               */
/* Layer	:	Scheduler										                               */
/* AUTHOR   :   Lina Nassar                                                                    */
/***********************************************************************************************/

#ifndef SCHEDULER_CONFIG_H
#define SCHEDULER_CONFIG_H


/*Configure the tick time of the scheduler in Milliseconds */
#define Sched_TickTime_Ms       1


/*Add the names of the required runnables to the list before Runnables_Num*/


typedef enum
{
    LCD_Runnable=0,
    KPD_Runnables,
    Receiver_Runnable,
    Sender_Runnable,
    Display_Runnable,
    StopWatch_R,

    _Runnables_Num,

}RunnableList_t;


#endif