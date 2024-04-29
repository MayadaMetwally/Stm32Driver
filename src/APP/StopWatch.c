#include "STD_TYPES.h"
#include "APP/Receiver.h"
#include "HCLCD/LCD.h"



typedef struct 
{
     u8 Hrs;
     u8 Mins;
     u8 Secs;
     u16 Ms;
}Stopwatch_Time;

extern RecType_tstr ReceiveType;


void Stopwatch_Runnable(void)
{
    
    
    static u8 Stopwatch_On=0;
    static Stopwatch_Time SW_Time={0,0,0,0};

    if( (ReceiveType.Mode == STOP_WATCH_MODE))
    {
        Stopwatch_On=1;
    }
    
    if (Stopwatch_On && ( (ReceiveType.StopWatchOperation == STOP_WATCH_MODE_ACTION_START) || (ReceiveType.StopWatchOperation == STOP_WATCH_MODE_ACTION_CONTINUE)) )
    {   
        //HLED_SetStatus(LED_RED,LED_ON);
        SW_Time.Ms += 200;
        if(SW_Time.Ms == 1000)
        {
            SW_Time.Ms = 0;
            SW_Time.Secs += 1;

            if(SW_Time.Secs == 60)
            {
                SW_Time.Secs = 0;
                SW_Time.Mins += 1;

                if(SW_Time.Mins == 60)
                {
                    SW_Time.Mins = 0;
                    SW_Time.Hrs += 1;

                    if(SW_Time.Hrs == 59)
                    {
                        SW_Time.Hrs=0;
                    }
                }
            }
        }
    }
    else if( Stopwatch_On && (ReceiveType.StopWatchOperation == STOP_WATCH_MODE_ACTION_STOP))
    {
        Stopwatch_On= 0;
        SW_Time.Hrs= 0;
        SW_Time.Mins= 0;
        SW_Time.Secs= 0;
        SW_Time.Ms= 0;    
    }

    if( (ReceiveType.Mode == STOP_WATCH_MODE))
    {
        CLCD_ClearScreenAsynch();
        CLCD_WriteStringAsynch("Stopwatch:", 10);
        CLCD_GoToXYAsynch(1,0);
        CLCD_WriteNumberAsynch(SW_Time.Hrs);
        CLCD_WriteStringAsynch(":",1);
        CLCD_WriteNumberAsynch(SW_Time.Mins);
        CLCD_WriteStringAsynch(":",1);
        CLCD_WriteNumberAsynch(SW_Time.Secs);
        CLCD_WriteStringAsynch(":",1);
        CLCD_WriteNumberAsynch(SW_Time.Ms);
            
    }
    

}