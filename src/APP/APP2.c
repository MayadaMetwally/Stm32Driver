#include "APP/APP2.h"
#include "HKPD/KYD.h"
#include "MUSART/USART.h"

USART_TXBuffer tx6_buff = 
{
	.Channel = USART1,
	.Data = NULL,
	.Size = 1
};

void APP2_RunnableFunc(void)
{
    u8 Local_Key=0;
   KPD_GetPressedKey(&Local_Key);
    if(Local_Key!=0)
    {
         tx6_buff.Data=&Local_Key;
        USART_SendBufferZeroCopy(&tx6_buff);
    }
    else
    {
        
    } 
    

}
