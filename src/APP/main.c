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
CLCD_WriteNumberAsynch(144);
CLCD_WriteStringAsynch("145236",6);
CLCD_GoToXYAsynch(1,2);
CLCD_WriteStringAsynch("MyaADA",6);
CLCD_ClearScreenAsynch();
CLCD_WriteStringAsynch("Metwally",8);

StartSched();



return 0;

}

// ----------------------------------------------------------------------------
