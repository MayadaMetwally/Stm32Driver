/***********************************************************************************************/
/* File Name:	Scheduler.h                                                                    */
/* Module	: 	Scheduler						                                               */
/* Layer	:	Scheduler										                               */
/* AUTHOR   :   Lina Nassar                                                                    */
/***********************************************************************************************/

#ifndef SCHEDULER_H
#define SCHEDULER_H


#include "STD_Types.h"

typedef void (*runnablecb_t) (void);

typedef struct
{
    char * name;
    uint32_t Priority;
    runnablecb_t cb;
    uint32_t FirstDelayMS; 
    uint32_t PeriodicityMS;
}Runnable_t;

void Sched_init(void);

void Sched_Start(void);




#endif