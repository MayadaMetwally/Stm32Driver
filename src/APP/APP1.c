#include "APP/APP1.h"
void APP1_RunnableFunc(void)
{
    u8 Local_Status;

    HSWITCH_GetSwitchStatus(SWITCH_03,&Local_Status);
    if(Local_Status==SWITCH_PRESSED)
    {
    	 HLED_SetStatus(LED_RED2,LED_ON);
    }
    else
    {
    	//HLED_SetStatus(LED_GREEN2,LED_ON);/*do nothing*/
    }

}
