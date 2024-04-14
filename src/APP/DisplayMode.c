#include "APP/DisplayMode.h"
#include "APP/Receiver.h"

#define DISPLAY_PERIODICITY       1
#define IS_MONTH_30_DAY(Month)    ((Month)==4||(Month)==6||(Month)==9||(Month)==11) 
#define IS_MONTH_31_DAY(Month)    ((Month)==1||(Month)==3||(Month)==5||(Month)==7||(Month)==8||(Month)==10||(Month)==12) 

#define HOURE_SECOND_DIGIT             6
#define HOURE_FIRST_DIGIT              (u8)(HOURE_SECOND_DIGIT+1) 

#define MINUTE_SECOND_DIGIT            (u8)(HOURE_FIRST_DIGIT+2)
#define MINUTE_FIRST_DIGIT             (u8)(MINUTE_SECOND_DIGIT+1)  

#define SECOND_SECOND_DIGIT            (u8)(MINUTE_FIRST_DIGIT+2)
#define SECOND_FIRST_DIGIT             (u8)(SECOND_SECOND_DIGIT+1)

#define YEAR_FOURTH_DIGIT              6       
#define YEAR_THIRD_DIGIT               (u8)(YEAR_FOURTH_DIGIT+1) 
#define YEAR_SECOND_DIGIT              (u8)(YEAR_THIRD_DIGIT+1) 
#define YEAR_FIRST_DIGIT               (u8)(YEAR_SECOND_DIGIT+1) 

#define MONTH_SECOND_DIGIT             (u8)(YEAR_FIRST_DIGIT+2)
#define MONTH_FIRST_DIGIT              (u8)(MONTH_SECOND_DIGIT+1) 

#define DAY_SECOND_DIGIT               (u8)(MONTH_FIRST_DIGIT+2)
#define DAY_FIRST_DIGIT                (u8)(DAY_SECOND_DIGIT+1) 




typedef struct 
{
   u16 Year;
   u8 Month;
   u8 Day;

}Date_tstr;

typedef struct 
{
   u8 Hour;
   u8 Minuts;
   u8 Second;
   u8 MiliSecond;

}Time_tstr;

extern RecType_tstr ReceiveType;

static void Diplay_EditHlp(void);
static Time_tstr Time=
{
    .Hour=23,
    .Minuts=59,
    .Second=50,
    .MiliSecond=2,
        
};
static Date_tstr Date=
{
    .Year=2024,
    .Month=12,
    .Day=30,
        
};

void DisplayDateTime(void)
{


Time.Second+=DISPLAY_PERIODICITY;

if(ReceiveType.Mode==NORMAL_MODE)
{
    
    CLCD_ClearScreenAsynch();
    CLCD_WriteStringAsynch("DATE:",5);
    /*year*/
    if((Date.Year<1000)&&(Date.Year>=100))
    {
        CLCD_WriteNumberAsynch(0);
    }
    else if((Date.Year<100)&&Date.Year>=10)
    {
        CLCD_WriteStringAsynch("00",2);        
    }
    else if(Date.Year<10)
    {
        CLCD_WriteStringAsynch("000",3);        
    }    
    CLCD_WriteNumberAsynch(Date.Year);
    CLCD_WriteStringAsynch("/",1);
    /*Month*/
    if(Date.Month<10)
    {
        CLCD_WriteNumberAsynch(0);
    }
    CLCD_WriteNumberAsynch(Date.Month);
    CLCD_WriteStringAsynch("/",1);
    /*Day*/
    if(Date.Day<10)
    {
        CLCD_WriteNumberAsynch(0);
    }
    
    CLCD_WriteNumberAsynch(Date.Day);
    CLCD_GoToXYAsynch(1,0);
    /*Time*/
    CLCD_WriteStringAsynch("TIME:",5);
    /*Hour*/
    if(Time.Hour<10)
    {
       CLCD_WriteNumberAsynch(0); 
    }
    CLCD_WriteNumberAsynch(Time.Hour);
    CLCD_WriteStringAsynch(":",1);
    /*Minutes*/
    if(Time.Minuts<10)
    {
        CLCD_WriteNumberAsynch(0);
    }
    CLCD_WriteNumberAsynch(Time.Minuts);
    CLCD_WriteStringAsynch(":",1);
    /*Second*/
    if(Time.Second<10)
    {
        CLCD_WriteNumberAsynch(0);
    }
    CLCD_WriteNumberAsynch(Time.Second);
    CLCD_DisplaySpecialCharAsynch(1);
    if(ReceiveType.NormalModeEdit==NORMAL_MODE_EDIT)
    {
        Diplay_EditHlp();
        ReceiveType.NormalModeOperation=NO_ACTION;
    }   
    else
    {
        /*Do Nothing*/
    }
    
    if(Time.Second>59)
    {
        Time.Second=0;
        Time.Minuts++;
    }
    if (Time.Minuts>59)
    {
        Time.Minuts=0;
        Time.Hour++;
    }
    if(Time.Hour>23)
    {
        Time.Hour=0;
        Date.Day++;
    }
    if(Date.Day==29&&Date.Month==2)
    {
        if(!(Date.Year % 4 == 0 && (Date.Year % 100 != 0 || Date.Year % 400 == 0)))
        {
            Date.Day=1;
            Date.Month++;
        }
        else
        {
            /*Do Nothing*/
        }

    }
    else if (Date.Day==30&&Date.Month==2)
    {
        if((Date.Year % 4 == 0 && (Date.Year % 100 != 0 || Date.Year % 400 == 0)))
        {
            Date.Day=1;
            Date.Month++;
        }
        else
        {
            /*Do Nothing*/
        }
    }
    else if (Date.Day==31&&(IS_MONTH_30_DAY(Date.Month)))
    {
        Date.Day=1;
        Date.Month++;
    }
    else if (((Date.Day==32)&&(IS_MONTH_31_DAY(Date.Month)))||Date.Day>31)
    {
        Date.Day=1;
        Date.Month++;
    }
    if (Date.Month>12)
    {
        Date.Month=1;
        Date.Year++;
    }

}
else 
{
    /*Do Nothing*/
}


}

void Diplay_EditHlp(void)
{
    static u8 Local_RowLinePos=0;
    static u8 Local_ColLinePos=1;
    CLCD_GoToXYAsynch(Local_RowLinePos,Local_ColLinePos);
    switch (ReceiveType.NormalModeOperation)
    {
    case NO_ACTION:
        break;  
    case EDIT_MODE_OK:
        Local_ColLinePos=1;
        Local_RowLinePos=0;
        break;  
    case NORMAL_MODE_ACTION_UP:
        Local_RowLinePos^=1;
        break;
    case NORMAL_MODE_ACTION_DOWN:
        Local_RowLinePos^=1;
        break;
    case NORMAL_MODE_ACTION_RIGHT:
        Local_ColLinePos++;
        if(Local_ColLinePos==16)
        {
            Local_ColLinePos=0;
            Local_RowLinePos^=1;
        }      
        break;
    case NORMAL_MODE_ACTION_LEFT:
        if(Local_ColLinePos==0)
        {
            Local_ColLinePos=15;
            Local_RowLinePos^=1;
        }
        else
        {
            Local_ColLinePos--;
        }

        break;

    case NORMAL_MODE_ACTION_INCREASE:
        if(Local_RowLinePos==0)
        {
            switch (Local_ColLinePos)
            {
            case YEAR_FOURTH_DIGIT:
                Date.Year+=1000;
                break;
            case YEAR_THIRD_DIGIT:
                Date.Year+=100;
                break;                
            case YEAR_SECOND_DIGIT:
                Date.Year+=10;
                break;
            case YEAR_FIRST_DIGIT:
                Date.Year+=1;
                break;
            case DAY_SECOND_DIGIT:
                Date.Day+=10;
                break;
            case DAY_FIRST_DIGIT:
                Date.Day+=1;
                break;                
            case MONTH_SECOND_DIGIT:
                if (Date.Month>9)
                {
                    Date.Year++;
                    Date.Month-=2;
                } 
                else
                {
                     Date.Month+=10;
                }               
                break;
            case MONTH_FIRST_DIGIT:
               if (Date.Month==12)
                {
                    Date.Year++;
                    Date.Month=0;
                }  
                Date.Month+=1;
                break;      
            default:
                break;
            }
        }
        else if (Local_RowLinePos==1)
        {
            switch (Local_ColLinePos)
            {
            case HOURE_SECOND_DIGIT:
                if(Time.Hour>13)
                {
                    Date.Day++;
                    Time.Hour-=14;
                }
                else
                {
                    Time.Hour+=10;
                }
                break;
            case HOURE_FIRST_DIGIT:
                if(Time.Hour==23)
                {
                    Date.Day++;
                    Time.Hour=0;
                }            
                Time.Hour+=1;
                break;                
            case MINUTE_SECOND_DIGIT:
                if(Time.Minuts>49)
                {
                    Time.Hour++;
                    Time.Minuts-=50;
                } 
                else
                {
                    Time.Minuts+=10;                    
                }           
                break; 
            case MINUTE_FIRST_DIGIT:
                if(Time.Minuts==59)
                {
                    Time.Hour++;
                    Time.Minuts=0;
                }               
                Time.Minuts+=1;
                break;
            case SECOND_SECOND_DIGIT:
                if(Time.Second>49)
                {
                    Time.Minuts++;
                    Time.Second-=50;
                } 
                else
                {
                    Time.Second+=10;                    
                }              
                break;
            case SECOND_FIRST_DIGIT:
                if(Time.Second==59)
                {
                    Time.Minuts++;
                    Time.Second=0;
                }               
                Time.Second+=1;
                break;
            default:
                break;
            }
        }       

        
        break;
    case NORMAL_MODE_ACTION_DECREASE:
        if(Local_RowLinePos==0)
        {
            switch (Local_ColLinePos)
            {
            case YEAR_FOURTH_DIGIT:
                if(Date.Year>999)
                {
                    Date.Year-=1000;
                }
                break;
            case YEAR_THIRD_DIGIT:
                if(Date.Year>99)
                {
                    Date.Year-=100;
                }
                break;             
            case YEAR_SECOND_DIGIT:
                if(Date.Year>9)
                {
                    Date.Year-=10;
                }
                break;
            case YEAR_FIRST_DIGIT:
                if(Date.Year>0)
                {
                    Date.Year-=1;
                }
                break;
            case DAY_SECOND_DIGIT:
                if(Date.Day>9)
                {
                    Date.Day-=10;
                }
                break;
            case DAY_FIRST_DIGIT:
                if(Date.Day>0)
                {
                Date.Day-=1;  
                }
                break;  
            case MONTH_SECOND_DIGIT:
                if(Date.Month>9)
                {
                Date.Month-=10;
                }
                break;
            case MONTH_FIRST_DIGIT:
                if(Date.Month>0)
                {
                Date.Month-=1;
                }
                break;    
            default:
                break;
            }
        }
        else if (Local_RowLinePos==1)
        {
            switch (Local_ColLinePos)
            {
            case HOURE_SECOND_DIGIT:
                if(Time.Hour>9)
                {
                    Time.Hour-=10;
                }              
                break;
            case HOURE_FIRST_DIGIT:
                if(Time.Hour>0)
                {
                    Time.Hour-=1;
                }
                break;                
            case MINUTE_SECOND_DIGIT:
                if(Time.Minuts>9)
                {
                    Time.Minuts-=10;
                }
                break; 
            case MINUTE_FIRST_DIGIT:
                if(Time.Minuts>0)
                {
                    Time.Minuts-=1;
                }
                break;
            case SECOND_SECOND_DIGIT:
                if(Time.Second>9)
                {
                    Time.Second-=10;
                }
                break;
            case SECOND_FIRST_DIGIT:
                if (Time.Second)
                {
                    Time.Second-=1;                    
                }     
                break;
            default:
                break;
            }
        }
        break;
                        
    default:
        break;
    }

    

}