#include "STD_TYPES.h"
#include "HLED/LED.h"
#include "HSWITCH/SWITCH.h"
#include "SERVICE/SCHED/SCHED.h"




int main()
{

HLED_Init();
HSWITCH_Init();

Sched_Init();
StartSched();

return 0;

}

// ----------------------------------------------------------------------------
