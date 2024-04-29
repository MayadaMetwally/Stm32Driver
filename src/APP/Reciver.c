#include "APP/Receiver.h"




RecType_tstr ReceiveType={.Mode=NORMAL_MODE,.NormalModeOperation=NO_ACTION,.StopWatchOperation=NO_ACTION,.NormalModeEdit=NO_ACTION};



void Receive_RunnableFunc(void)
{
    static u8 Local_Count=1;
    u8 Loc_ReceiveValue=NULL ;

   // USART_ReceiveBufferAsynchronous(&rx_buff);
    USART_ReceiveByteSynchByTime(USART1,&Loc_ReceiveValue);
    switch (Loc_ReceiveValue) {
    case 'M': /*  Mode */
        ReceiveType.Mode ^= 1; // Toggle Mode
        break;
    case 'E':
        if(ReceiveType.Mode==NORMAL_MODE)
        {
            ReceiveType.NormalModeEdit =NORMAL_MODE_EDIT ; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        } 
    
        break;

    case 'K': /* Ok to save edit */
        if(ReceiveType.Mode==NORMAL_MODE&&ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
        {
            ReceiveType.NormalModeOperation=EDIT_MODE_OK;
            ReceiveType.NormalModeEdit=NO_ACTION; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        }

        break;
    case '+': /* Increase time/date in Normal Mode */
        if(ReceiveType.Mode==NORMAL_MODE&&ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
        {
            ReceiveType.NormalModeOperation=NORMAL_MODE_ACTION_INCREASE; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        }
        break;
    case '-': /* Decrease time/date in Normal Mode */
        if(ReceiveType.Mode==NORMAL_MODE&&ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
        {
            ReceiveType.NormalModeOperation=NORMAL_MODE_ACTION_DECREASE; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        }
        break;
    case 'U': /* Move Up in Normal Mode */
        if(ReceiveType.Mode==NORMAL_MODE&&ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
        {
            ReceiveType.NormalModeOperation=NORMAL_MODE_ACTION_UP; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        }
        break;
    case 'D': /* Move Down in Normal Mode */
        if(ReceiveType.Mode==NORMAL_MODE&&ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
        {
            ReceiveType.NormalModeOperation=NORMAL_MODE_ACTION_DOWN; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        }
        break;
    case 'R': /* Move Right in Normal Mode */
        if(ReceiveType.Mode==NORMAL_MODE&&ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
        {
            ReceiveType.NormalModeOperation=NORMAL_MODE_ACTION_RIGHT; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        }
        break;
    case 'L': /* Move Left in Normal Mode */
        if(ReceiveType.Mode==NORMAL_MODE&&ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
        {
            ReceiveType.NormalModeOperation=NORMAL_MODE_ACTION_LEFT; // Assigning a single character
        }
        else
        {
            /*Do Nothing*/
        }
        break;
    case 'S': /* Start/continue in StopWatch Mode */
        if(ReceiveType.Mode==STOP_WATCH_MODE)
        {
            Local_Count%=3;
            switch (Local_Count)
            {
            case 1:
                ReceiveType.StopWatchOperation=STOP_WATCH_MODE_ACTION_START;
                break;
            case 2:
                ReceiveType.StopWatchOperation=STOP_WATCH_MODE_ACTION_PAUSE;

                break;            
            case 0:
                ReceiveType.StopWatchOperation=STOP_WATCH_MODE_ACTION_CONTINUE;
                Local_Count++;
                break;
            default:
                break;                        
            }
            Local_Count++;

        }
        else
        {
            /*Do Nothing*/
        }
        break;
    case 'C': /* Stop in StopWatch Mode */
        if(ReceiveType.Mode==STOP_WATCH_MODE)
        {
            ReceiveType.StopWatchOperation=STOP_WATCH_MODE_ACTION_STOP;
            Local_Count=1;

        }
        break;
    default:
        break;
    }



    

   
    
}