/************************************************************************************************************************************************/
/* File             :       HLCD.c                                                                                                         */
/* AUTHOR           :       ’Mayada Metwally                                                                                                        */
/* Origin Date      :       3/20/2024                                                                                                            */
/* SWC              :       Character LCD                                                                                                       */
/************************************************************************************************************************************************/


/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"
#include "HCLCD/LCD.h"
#include "MGPIO/GPIO.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define POWER_PIN_NUMBER   3
#define ENABLE             1
#define DISABLE            0 
#define COLOUM_NUMBER      16
#define FIRST_LINE          0
#define SECOND_LINE         1
#define DISPLAY_CLEAR       0X01


#define CONCAT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)          0b##B7##B6##B5##B4##B3##B2##B1##B0
#define CONCAT(B7,B6,B5,B4,B3,B2,B1,B0)                 CONCAT_HELPER(B7,B6,B5,B4,B3,B2,B1,B0)

#define DISPLAY_ON_OFF        CONCAT(0,0,0,0,1,HLCD_DISPLAY,HLCD_CURSER,HLCD_BLINKING)
#define ENTRY_MODE            CONCAT(0,0,0,0,0,1,HLCD_INCREMENT_DECREMENT,HLCD_SHIFTING)
#define START_ADDREESS        0x80
#define RRTURN_HOME           0x02
#if HLCD_MODE == HLCD_MODE_8_BIT
#define FUNCTION_SET        CONCAT(0,0,1,1,HLCD_LINES_NUMBER,HLCD_FORMAT,0,0)
#define HLCD_PINS_NUMBER    8

#elif HLCD_MODE == HLCD_MODE_4_BIT
#define FUNCTION_SET        CONCAT(0,0,1,0,HLCD_LINES_NUMBER,HLCD_FORMAT,0,0)
#define HLCD_PINS_NUMBER    4

#else

#error "Wrong HLCD_MODE Configuration "

#endif







/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef void(*CallBackFunc )(void);

typedef enum{
    CLCD_OFF,
    CLCD_Init_state,
    CLCD_Operatinal,


}CLCD_States_tenu;

typedef enum{
    PowerON,
    FunctionalSet,
    DisplayControl,
    DisplayClear,
    InitEnd

}CLCD_InitStates_tenu;
typedef enum
{
    NOReq,
    ReqStart,
    WriteReq,
    ClearReq,
    CommandReq,
    SetPositionReq,
    ReqDone

}CLCD_UserReqType_tenu;

typedef enum
{
    CLCD_Idle,
    CLCD_Busy
    
}CLCD_UserReqState_tenu;

typedef enum
{
    NotBuffered,
    Buffered,
}CLCD_BufferState_tenu;

typedef struct
{
    u8 *Str;
    u8 Len;
    CLCD_UserReqType_tenu Type;
    CLCD_BufferState_tenu BufferState;
}CLCD_UserReq_tstr;



typedef struct 
{
    u8 CurrPos;
    CallBackFunc cb;
}CLCD_Write_tstr;

typedef enum
{
    CLCD_WriteStart,
    CLCD_WriteChar,
    CLCD_WriteDone

}CLCD_WriteState_tenu;


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern LCD_cfg_t HLCD;
static CLCD_States_tenu G_CLCD_State = CLCD_OFF;
static CLCD_UserReqType_tenu CLCD_CurReq = NOReq;
static CLCD_UserReq_tstr G_UserReq[LCD_BUFFERSIZE];
static CLCD_Write_tstr G_UserWriteReq[LCD_BUFFERSIZE];
static u8 CLCD_EnablePin = DISABLE;
static u8 CLCD_PositionX=0;
static u8 CLCD_PositionY=0;
static u8 CLCD_ITER     =0;
static u8 CLCD_CurrentBuffer   = 0;
static CLCD_WriteState_tenu CLCD_WriteState =CLCD_WriteStart;
static u8 CLCD_BufferIndex     = 0;
static CLCD_UserReqState_tenu CLCD_State;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void CLCD_InitSM(void);

static void CLCD_SendCommandProcess(u8 Copy_Command);
static void CLCD_ControlEnablePin(u8 Copy_Pinstatus);
static void CLCD_GoToXYProcess(void); 
static void CLCD_WriteCharProcess(u8 Copy_Char);

static void CLCD_WriteProcess(void);
void CLCD_TASK(void);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
void CLCD_TASK(void)
{
    if (G_CLCD_State==CLCD_Init_state)
    {
        CLCD_InitSM();
    }
    else if (G_CLCD_State==CLCD_Operatinal)
    {
       switch (CLCD_CurReq)
       {
       case WriteReq:
        
        CLCD_WriteProcess();   
        break;
       case ClearReq:
        CLCD_SendCommandProcess(DISPLAY_CLEAR);
        break;
       case SetPositionReq:
        CLCD_GoToXYProcess();
        break;        
       case NOReq:
        break;           
       default:
        break;
       }
    }
    else{
        /*DoNothing*/
    }   
}
void CLCD_InitAsynch(void){
    u8 idx =0;
    GPIO_Pin_tstr LCD;
    LCD.Mode=GPIO_MODE_OP_PP;
    LCD.Speed=GPIO_SPEED_HIGH;
  
        
    for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++)
    {
        LCD.Pin=HLCD.LCD_data_pins[idx].Pin;
        LCD.Port=HLCD.LCD_data_pins[idx].Port;
        MGPIO_InitPin(&LCD);
    }

        LCD.Pin=HLCD.R_S_pin.Pin;
        LCD.Port=HLCD.R_S_pin.Port;
        MGPIO_InitPin(&LCD);

        LCD.Pin=HLCD.R_W_pin.Pin;
        LCD.Port=HLCD.R_W_pin.Port;
        MGPIO_InitPin(&LCD);

        LCD.Pin=HLCD.E_pin.Pin;
        LCD.Port=HLCD.E_pin.Port;
        MGPIO_InitPin(&LCD);
      
        G_CLCD_State = CLCD_Init_state;   
}
void CLCD_InitSM(void){
    static CLCD_InitStates_tenu SLocal_InitState = PowerON;
    static u8 Counter =0; 
    switch (SLocal_InitState)
    {
    case PowerON:
        SLocal_InitState = FunctionalSet;
        CLCD_EnablePin=DISABLE;
        CLCD_ControlEnablePin(GPIO_Low);
        break;
    case FunctionalSet:
    if(Counter>=19)
    {
        if(CLCD_EnablePin==DISABLE)
        {
            CLCD_SendCommandProcess(FUNCTION_SET);
            CLCD_EnablePin=ENABLE;
            CLCD_ControlEnablePin(GPIO_High);
        }
        else
        {
            CLCD_EnablePin=DISABLE;
            CLCD_ControlEnablePin(GPIO_Low);
            SLocal_InitState = DisplayControl;

        }
    } 
        Counter++;   
        break;
    case DisplayControl:
        if(CLCD_EnablePin==DISABLE)
        {
            CLCD_SendCommandProcess(DISPLAY_ON_OFF);
            CLCD_EnablePin=ENABLE;
            CLCD_ControlEnablePin(GPIO_High);
        }
        else
        {
            CLCD_EnablePin=DISABLE;
            CLCD_ControlEnablePin(GPIO_Low);
            SLocal_InitState = DisplayClear;

        }
        
        break;
    case DisplayClear:
        if(CLCD_EnablePin==DISABLE)
        {
            CLCD_SendCommandProcess(DISPLAY_CLEAR);
            CLCD_EnablePin=ENABLE;
            CLCD_ControlEnablePin(GPIO_High);
        }
        else
        {
            CLCD_EnablePin=DISABLE;
            CLCD_ControlEnablePin(GPIO_Low);
            SLocal_InitState =InitEnd;

        }    
        
        break;

    case InitEnd:
        G_CLCD_State = CLCD_Operatinal;
        break;
                                          
    default:
        break;
    }

}
/***********************************************************************************************/
void CLCD_ControlEnablePin(u8 Copy_Pinstatus){
    MGPIO_SetPin(HLCD.E_pin.Port,HLCD.E_pin.Pin,Copy_Pinstatus);
}
/***********************************************************************************************/
void CLCD_SendCommandProcess(u8 Copy_Command){
    u8 idx=0;
        MGPIO_SetPin(HLCD.R_S_pin.Port,HLCD.R_S_pin.Pin,GPIO_Low);
        MGPIO_SetPin(HLCD.R_W_pin.Port,HLCD.R_W_pin.Pin,GPIO_Low);
   
        for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++)
        {
            MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,((Copy_Command>>idx)&0x01));

        }

    

}
/***********************************************************************************************/
void CLCD_WriteProcess(void){
     CLCD_State=CLCD_Busy;
    switch ( CLCD_WriteState)
    {
    case CLCD_WriteStart:
    CLCD_WriteState=CLCD_WriteChar;
    break;    
    case CLCD_WriteChar:
        if(G_UserWriteReq[CLCD_CurrentBuffer].CurrPos<G_UserReq[CLCD_CurrentBuffer].Len)
        {
            if(CLCD_EnablePin==DISABLE)
            {
                CLCD_ITER=G_UserWriteReq[CLCD_CurrentBuffer].CurrPos;
                CLCD_WriteCharProcess(G_UserReq[CLCD_CurrentBuffer].Str[CLCD_ITER]);
                CLCD_EnablePin=ENABLE;
                CLCD_ControlEnablePin(GPIO_High);
            }
            else
            {
                CLCD_EnablePin=DISABLE;
                CLCD_ControlEnablePin(GPIO_Low); 
                G_UserWriteReq[CLCD_CurrentBuffer].CurrPos++;
            }
        }
        else
        {
            CLCD_WriteState=CLCD_WriteDone;
        }
        break;
    case CLCD_WriteDone:
        G_UserReq[CLCD_CurrentBuffer].BufferState=NotBuffered;
        for (CLCD_BufferIndex = 0; CLCD_BufferIndex < LCD_BUFFERSIZE;CLCD_BufferIndex++)
        {
            if(G_UserReq[CLCD_BufferIndex].BufferState==Buffered)
            {
                break;
            }
        }
        if(CLCD_BufferIndex==LCD_BUFFERSIZE)
        {
                 CLCD_CurReq=NOReq;
                CLCD_CurrentBuffer=0;
        }
        else 
        {
            CLCD_CurrentBuffer=CLCD_BufferIndex;
            CLCD_WriteState=CLCD_WriteStart;
            if (CLCD_CurrentBuffer==LCD_BUFFERSIZE)
            {
                CLCD_CurrentBuffer=0;
            }
                        
        }  

        break;    
    default:
        break;
    }
}
/***********************************************************************************************/
void CLCD_WriteCharProcess(u8 Copy_Char){
    u8 idx=0;
   
        MGPIO_SetPin(HLCD.R_S_pin.Port,HLCD.R_S_pin.Pin,GPIO_High);
        MGPIO_SetPin(HLCD.R_W_pin.Port,HLCD.R_W_pin.Pin,GPIO_Low);
        for (idx=0 ; idx <HLCD_PINS_NUMBER; idx++)
        {
            MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,(Copy_Char>>idx)&1);
        }
}
/***********************************************************************************************/
void CLCD_GoToXYProcess(void)
{
	if (CLCD_PositionX == FIRST_LINE && CLCD_PositionY < COLOUM_NUMBER)
	{
		CLCD_SendCommandProcess((CLCD_PositionY-1)+128);
	}
	else if (CLCD_PositionX == SECOND_LINE && CLCD_PositionY < COLOUM_NUMBER)
	{
		CLCD_SendCommandProcess((CLCD_PositionY-1)+192);
	}
}
/******************************************user Functions*****************************************************/
tenu_ErrorStatus CLCD_WriteString(char * Add_pStr , u8 Copy_len){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 idx_Buffer=0;
    //u8 idx_String=0;
    if(Add_pStr==NULL)
    {
        Local_ErrorStatus = LBTY_ErrorNullPointer;
    }
    else
    {//if(G_CLCD_State==CLCD_Operatinal&&G_UserReq.Type==NOReq)
    for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++)
    {
        if (G_UserReq[idx_Buffer].BufferState==NotBuffered)
        {
            //*for string idx*//
                G_UserReq[idx_Buffer].Str=Add_pStr;
            
            G_UserWriteReq[idx_Buffer].CurrPos=0;
            G_UserReq[idx_Buffer].BufferState=Buffered;
            G_UserReq[idx_Buffer].Type=WriteReq; 
            G_UserReq[idx_Buffer].Len=Copy_len;
            if(CLCD_CurReq==NOReq)
            {
               CLCD_CurReq=WriteReq; 
               CLCD_WriteState=CLCD_WriteStart;
            }
            break;                   
        }        
    }   
    if (idx_Buffer==LCD_BUFFERSIZE)
    {
        Local_ErrorStatus=LBTY_NOK;
    }

    }
    return Local_ErrorStatus;

}
tenu_ErrorStatus CLCD_GoToXY(u8 Copy_X , u8 Copy_Y)
{
    tenu_ErrorStatus Local_EroorStatus = LBTY_OK;
	if (Copy_X <= SECOND_LINE && Copy_Y < COLOUM_NUMBER)
	{
		CLCD_PositionX=Copy_X;
        CLCD_PositionY=Copy_Y;
        G_UserReq[CLCD_CurrentBuffer].Type=SetPositionReq;

	}
    else
    {
        Local_EroorStatus = LBTY_ErrorInvalidInput;
    }
    return Local_EroorStatus;
}
void CLCD_ClearScreen(void)
{
    G_UserReq[CLCD_CurrentBuffer].Type=ClearReq;
}
/*tenu_ErrorStatus CLCD_WriteCommand(u8 Copy_Command){

}*/