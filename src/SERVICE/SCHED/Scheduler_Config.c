/***********************************************************************************************/
/* File Name:	Scheduler_Config.c                                                             */
/* Module	: 	Scheduler						                                               */
/* Layer	:	Scheduler										                               */
/* AUTHOR   :   Lina Nassar                                                                    */
/***********************************************************************************************/

#include "SERVICE/SCHED/Scheduler_Config.h"
#include "SERVICE/SCHED/Scheduler.h"

/*Add prototypes of the runnables here
*   Example: extern void Run_1(void); */

/********************************************************************************************************/
/************************************************externs*************************************************/
/********************************************************************************************************/
extern void HSWITCH_Runnable(void);
extern void APP1_RunnableFunc(void);
extern void APP2_RunnableFunc(void);
extern void TrafficLights (void);
extern void CLCD_TASK(void);
extern void KPD_Runnable(void);
extern void DisplayDateTime(void);
extern void Sender_RunnableFunc(void);
extern void Receive_RunnableFunc(void);
extern   void Stopwatch_Runnable(void);

/*Add each of the runnables to the array and fill its required fields*/
/*  Example: 
        [Run_1]=
        {
            .name= "Run_1"   ,
            .Priority= 1     ,
            .cb = Run1_cb    ,    
            .FirstDelayMS= 15,   
            .PeriodicityMS= 5,
        },
*/
const Runnable_t Runnable[_Runnables_Num]=
{
    [LCD_Runnable]=
    {
        .name= "LCD",
        .Priority= LCD_Runnable,
        .cb = CLCD_TASK,    
        .FirstDelayMS= 0,
        .PeriodicityMS= 2,
    },
    [KPD_Runnables]=
    {
        .name= "KPD",
        .Priority= KPD_Runnables,
        .cb = KPD_Runnable,    
        .FirstDelayMS= 1,
        .PeriodicityMS= 20,
    },
    [Display_Runnable]=
    {
        .name= "Disply",
        .Priority= Display_Runnable,
        .cb = DisplayDateTime,    
        .FirstDelayMS= 5,
        .PeriodicityMS= 400 ,
    },
    [Sender_Runnable]=
    {
        .name= "Sender",
        .Priority= Sender_Runnable,
        .cb = Sender_RunnableFunc,    
        .FirstDelayMS= 6,
        .PeriodicityMS= 60,
    },
    [Receiver_Runnable]=
    {
        .name= "Receiver",
        .Priority= Receiver_Runnable,
        .cb = Receive_RunnableFunc,    
        .FirstDelayMS= 2,
        .PeriodicityMS= 30 ,
    },
    [StopWatch_R]=
    {
        .name= "StopWatch",
        .Priority= StopWatch_R,
        .cb = Stopwatch_Runnable,    
        .FirstDelayMS= 7,
        .PeriodicityMS= 200 ,
    },
    
    

};

 