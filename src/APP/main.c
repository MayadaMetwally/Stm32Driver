#include "STD_TYPES.h"
#include "HLED/LED.h"
#include "HSWITCH/SWITCH.h"
#include "SERVICE/SCHED/SCHED.h"
#include "HCLCD/LCD.h"



int main()
{
char array[8]=HCLCD_HEART;
HLED_Init();
HSWITCH_Init();
HLED_SetStatus(LED_GREEN2,LED_ON);
CLCD_InitAsynch();
Sched_Init();
CLCD_WriteSpecialCharAsynch(array,1);
CLCD_WriteStringAsynch("Mayada",6);
CLCD_DisplaySpecialCharAsynch(1);
StartSched();



return 0;

}

// ----------------------------------------------------------------------------
