/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"
#include "SERVICE/SCHED/SCHED.h"


/********************************************************************************************************/
/************************************************externs*************************************************/
/********************************************************************************************************/
extern void HSWITCH_Runnable(void);
extern void APP1_RunnableFunc(void);
extern void APP2_RunnableFunc(void);
extern void TrafficLights (void);





/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
Runnable_tstr Runnables[_RUNNABLE_NUM]={
    [SW_Runnable]={.PeriodicityMs=5,.FirstDelayMs=1,.CallBack=HSWITCH_Runnable,.Priority=SW_Runnable},
    [APP1_Runnable]={.PeriodicityMs=50,.FirstDelayMs=5,.CallBack=APP1_RunnableFunc,.Priority=APP1_Runnable},
    [APP2_Runnable]={.PeriodicityMs=50,.FirstDelayMs=6,.CallBack=APP2_RunnableFunc,.Priority=APP2_Runnable},
	[TrafficLight_Runnable]={.PeriodicityMs=2000,.FirstDelayMs=2,.CallBack=TrafficLights,.Priority=TrafficLight_Runnable},


};
 

