#include "STD_TYPES.h"
#include "HLED/LED.h"
#include "HSWITCH/SWITCH.h"
#include "SERVICE/SCHED/SCHED.h"
#include "HCLCD/LCD.h"



int main()
{

HLED_Init();
HSWITCH_Init();
HLED_SetStatus(LED_GREEN2,LED_ON);
CLCD_InitAsynch();
Sched_Init();
CLCD_WriteString("BASSAM",6);
StartSched();



return 0;

}

// ----------------------------------------------------------------------------
