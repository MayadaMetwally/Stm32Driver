#include "APP/APP1.h"
#include "HKPD/KYD.h"
#include "HCLCD/LCD.h"
void APP1_RunnableFunc(void)
{
    u8 Local_Key;
    KPD_GetPressedKey(&Local_Key);
   
    if(Local_Key=='1')
    {
        CLCD_WriteStringAsynch("1",1);
    }
    else if(Local_Key=='2')
    {
        CLCD_WriteStringAsynch("2",1);
    }
    else if(Local_Key=='3')
    {
        CLCD_WriteStringAsynch("3",1);
    }   
    else if(Local_Key=='4')
    {
        CLCD_WriteStringAsynch("4",1);
    }
    else if(Local_Key=='5')
    {
        CLCD_WriteStringAsynch("5",1);
    }
    else if(Local_Key=='6')
    {
        CLCD_WriteStringAsynch("6",1);
    }   
    else if(Local_Key=='7')
    {
        CLCD_WriteStringAsynch("7",1);
    } 
    else if(Local_Key=='8')
    {
        CLCD_WriteStringAsynch("8",1);
    }
    else if(Local_Key=='9')
    {
        CLCD_WriteStringAsynch("9",1);
    }       
    else if(Local_Key=='A')
    {
        CLCD_WriteStringAsynch("A",1);
    }   
    else if(Local_Key=='B')
    {
        CLCD_WriteStringAsynch("B",1);
    }
     else if(Local_Key=='C')
    {
        CLCD_WriteStringAsynch("C",1);
    }  
    else if(Local_Key=='D')
    {
        CLCD_WriteStringAsynch("C",1);
    }    
    else if(Local_Key=='E')
    {
        CLCD_WriteStringAsynch("8",1);
    }
    else if(Local_Key=='F')
    {
        CLCD_WriteStringAsynch("F",1);
    }  
    else if(Local_Key=='G')
    {
        CLCD_WriteStringAsynch("G",1);
    }    
    else if(Local_Key=='H')
    {
        CLCD_WriteStringAsynch("H",1);
    }   

  
     
   


}
