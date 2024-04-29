/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "APP/Sender.h"


/* USART_TXBuffer tx1_buff = 
{
	.Channel = USART1,
	.Data = NULL,
	.Size = 1
}; */

void Sender_RunnableFunc(void)
{
    u8 Local_Key=0;
   KPD_GetPressedKey(&Local_Key);
    if(Local_Key!=0)
    {
        //tx1_buff.Data=&Local_Key;
        USART_SendByteSynchByTime(USART1,Local_Key);
       // USART_SendBufferZeroCopy(&tx1_buff);
        Local_Key=0;
        
    }
    else
    {
        
    } 
    

}

