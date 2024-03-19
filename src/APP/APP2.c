#include "APP/APP2.h"

void APP2_RunnableFunc(void)
{
    u8 Local_Status;
    HSWITCH_GetSwitchStatus(SWITCH_02,&Local_Status);
    if(Local_Status==SWITCH_PRESSED)
    {
        HLED_SetStatus(LED_RED2,LED_OFF);
    }
    else
    {
        /*do nothing*/
    }

}
