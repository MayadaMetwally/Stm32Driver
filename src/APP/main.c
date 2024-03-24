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
CLCD_GoToXYAsynch(0,1);
CLCD_WriteNumberAsynch(144);
CLCD_WriteStringAsynch("Metwally",8);
CLCD_GoToXYAsynch(1,2);
CLCD_WriteStringAsynch("145236",6);

CLCD_WriteStringAsynch("Mayada",6);



StartSched();



return 0;

}

// ----------------------------------------------------------------------------
