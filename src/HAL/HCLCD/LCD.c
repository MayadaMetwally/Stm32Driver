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
typedef enum
{
    CLCD_ReqStart,
    CLCD_ReqInprogress,
    CLCD_ReqDone

}CLCD_WriteState_tenu;

typedef struct
{
    char *Str;
    u8 Len;
    u8 Command;
    CLCD_UserReqType_tenu Type;
    CLCD_WriteState_tenu  State;
    CLCD_BufferState_tenu BufferState;
}CLCD_UserReq_tstr;



typedef struct 
{
    u8 CurrPos;
    CallBackFunc cb;
}CLCD_Write_tstr;




/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern LCD_cfg_t HLCD;
static CLCD_States_tenu  G_CLCD_State = CLCD_OFF;
static CLCD_UserReq_tstr G_UserReq[LCD_BUFFERSIZE];
static CLCD_Write_tstr   G_UserWriteReq[LCD_BUFFERSIZE];
static u8 CLCD_EnablePin = DISABLE;
static u8 CLCD_PositionX=0;
static u8 CLCD_PositionY=0;
static u8 CLCD_ITER     =0;
static u8 CLCD_CurrentBuffer   = 0;
static u8 CLCD_BufferIndex     = 0;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
/**
 * @brief This function handles the state machine for initializing the LCD.
 *
 * This function implements the state machine for initializing the LCD. It progresses through
 * different states of initialization such as powering on, setting functional parameters,
 * controlling display, and clearing the display. Once initialization is complete, it sets
 * the CLCD state to operational.
 *
 * @return None
 */
static void CLCD_InitSM(void);
/**
 * @brief This function sends a command to the LCD.
 *
 * This function takes a command as a parameter and sends it to the LCD. It sets the RS pin to low to indicate
 * a command, and the RW pin to low for write mode. Then, it sets the data pins according to the command.
 *
 * @param Copy_Command: The command to be sent to the LCD.
 *
 * @return None
 */
static void CLCD_SendCommandProcess(u8 Copy_Command);
/**
 * @brief This function controls the enable pin of the LCD.
 *
 * This function takes a parameter to set the status of the enable pin of the LCD.
 * If the copy_pinstatus parameter is GPIO_High, the enable pin is set to high, otherwise, it is set to low.
 *
 * @param Copy_Pinstatus: Status of the enable pin (GPIO_High or GPIO_Low).
 *
 * @return None
 */
static void CLCD_ControlEnablePin(u8 Copy_Pinstatus);
/**
 * @brief Processes the request to set cursor position on the LCD.
 *
 * This function processes the request to set cursor position on the LCD display. It sets the control pins
 * and sends the appropriate command to the LCD module.
 *
 * @return None
 */
static void CLCD_GoToXYProcess(void); 
/**
 * @brief Writes a character to the LCD.
 *
 * This function writes a character to the LCD display. It sets the control pins and data pins accordingly
 * to transmit the character to the LCD module.
 *
 * @param Copy_Char: The character to be written to the LCD.
 *
 * @return None
 */
static void CLCD_WriteCharProcess(u8 Copy_Char);
/**
 * @brief Helper function to send a command to the LCD.
 *
 * This function is a helper function to send a command to the LCD. It manages the process of sending
 * the command to the LCD module, ensuring proper control pin and data pin settings.
 *
 * @param Copy_Command: The command to be sent to the LCD.
 *
 * @return None
 */
static void CLCD_SendCommandHlp(u8 Copy_Command);
/**
 * @brief This function processes the write request for the LCD.
 *
 * This function handles the write request for the LCD. It checks the current state of the write request
 * and writes the characters to the LCD one by one until the end of the string is reached.
 *
 * @return None
 */
static void CLCD_WriteProcess(void);
/**
 * @brief Main LCD task.
 *
 * This function serves as the main task for the LCD module. It processes various user requests
 * asynchronously, such as writing to the LCD, clearing the display, sending commands, and setting
 * cursor positions.
 *
 * @return None
 */
void CLCD_TASK(void);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
/***********************************************************************************************/

void CLCD_TASK(void)
{
    if (G_CLCD_State==CLCD_Init_state) // Check if LCD is in initialization state
    {
        CLCD_InitSM(); // Initialize LCD
    }
    else if (G_CLCD_State==CLCD_Operatinal) // Check if LCD is in operational state
    {
       switch (G_UserReq[CLCD_CurrentBuffer].Type) // Check type of current user request
       {
       case WriteReq:
        CLCD_WriteProcess(); // Process write request  
        break;
       case ClearReq:
        CLCD_SendCommandHlp(DISPLAY_CLEAR); // Send command to clear display
        break;
       case CommandReq:
         CLCD_SendCommandHlp(G_UserReq[CLCD_CurrentBuffer].Command); // Send command request
         break;
       case SetPositionReq:
        CLCD_GoToXYProcess(); // Set cursor position request
        break;        
       case ReqDone:
           G_UserReq[CLCD_CurrentBuffer].BufferState=NotBuffered; // Set buffer state to not buffered
           for (CLCD_BufferIndex = 0; CLCD_BufferIndex < LCD_BUFFERSIZE;CLCD_BufferIndex++) // Loop through LCD buffers
           {
               if(G_UserReq[CLCD_BufferIndex].BufferState==Buffered) // Check if buffer is buffered
               {
                   break; // Exit loop
               }
           }
           if(CLCD_BufferIndex==LCD_BUFFERSIZE) // Check if buffer index equals LCD buffer size
           {
             G_UserReq[CLCD_CurrentBuffer].Type=NOReq; // Set request type to no request
             CLCD_CurrentBuffer=0; // Reset current buffer index
           }
           else
           {
               CLCD_CurrentBuffer=CLCD_BufferIndex; // Update current buffer index
               G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqStart; // Set request state to request start
               if (CLCD_CurrentBuffer==LCD_BUFFERSIZE) // Check if current buffer index equals LCD buffer size
               {
                   CLCD_CurrentBuffer=0; // Reset current buffer index
               }
           }
        break;           
       default:
        break;
       }
    }
    else{
        /* Do Nothing */
    }   
}
void CLCD_InitAsynch(void){
    u8 idx =0; // Declare index variable
    GPIO_Pin_tstr LCD; // Declare GPIO pin structure for LCD
    
    LCD.Mode=GPIO_MODE_OP_PP; // Set GPIO mode to output push-pull
    LCD.Speed=GPIO_SPEED_HIGH; // Set GPIO speed to high
  
    // Initialize data pins for the LCD
    for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++)
    {
        LCD.Pin=HLCD.LCD_data_pins[idx].Pin; // Set pin number
        LCD.Port=HLCD.LCD_data_pins[idx].Port; // Set port number
        MGPIO_InitPin(&LCD); // Initialize GPIO pin
    }

    // Initialize control pins for the LCD (RS, RW, E)
    LCD.Pin=HLCD.R_S_pin.Pin; // Set RS pin number
    LCD.Port=HLCD.R_S_pin.Port; // Set RS port number
    MGPIO_InitPin(&LCD); // Initialize RS pin

    LCD.Pin=HLCD.R_W_pin.Pin; // Set RW pin number
    LCD.Port=HLCD.R_W_pin.Port; // Set RW port number
    MGPIO_InitPin(&LCD); // Initialize RW pin

    LCD.Pin=HLCD.E_pin.Pin; // Set E pin number
    LCD.Port=HLCD.E_pin.Port; // Set E port number
    MGPIO_InitPin(&LCD); // Initialize E pin
      
    G_CLCD_State = CLCD_Init_state; // Set LCD state to initialization state   
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

    switch (G_UserReq[CLCD_CurrentBuffer].State)
    {
    case CLCD_ReqStart:
    	G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress;
    break;    
    case CLCD_ReqInprogress:
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
        	G_UserReq[CLCD_CurrentBuffer].Type=ReqDone;
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
void CLCD_SendCommandHlp(u8 Copy_Command)
{
	switch (G_UserReq[CLCD_CurrentBuffer].State)
	{
	case CLCD_ReqStart:
		G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress;
		break;
	case CLCD_ReqInprogress:

            if(CLCD_EnablePin==DISABLE)
            {
            	CLCD_SendCommandProcess(Copy_Command);
                CLCD_EnablePin=ENABLE;
                CLCD_ControlEnablePin(GPIO_High);
            }
            else
            {
                CLCD_EnablePin=DISABLE;
                CLCD_ControlEnablePin(GPIO_Low);
    	        G_UserReq[CLCD_CurrentBuffer].Type=ReqDone;
    	        G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone;
            }

        break;

    default:
        break;



	}

}
/***********************************************************************************************/
void CLCD_GoToXYProcess(void)
{
    switch (G_UserReq[CLCD_CurrentBuffer].State)
    {
    case CLCD_ReqStart:
        	G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress;
        break;
    case CLCD_ReqInprogress:

    	if(CLCD_EnablePin==DISABLE)
    	 {
    		if (CLCD_PositionX == FIRST_LINE && CLCD_PositionY < COLOUM_NUMBER)
    		{
    			CLCD_SendCommandProcess((CLCD_PositionY-1)+128);
    		}
    		else if (CLCD_PositionX == SECOND_LINE && CLCD_PositionY < COLOUM_NUMBER)
    		{
    			CLCD_SendCommandProcess((CLCD_PositionY-1)+192);
    		}

    	    CLCD_EnablePin=ENABLE;
    	    CLCD_ControlEnablePin(GPIO_High);
    	 }
       else
    	{
    	  CLCD_EnablePin=DISABLE;
    	  CLCD_ControlEnablePin(GPIO_Low);
    	  G_UserReq[CLCD_CurrentBuffer].Type=ReqDone;
    	  G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone;
    	}
    	break;
    default:
        break;


    }
}
/******************************************user Functions*****************************************************/
tenu_ErrorStatus CLCD_WriteStringAsynch(char * Add_pStr , u8 Copy_len){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 idx_Buffer=0;
    if(Add_pStr==NULL)
    {
        Local_ErrorStatus = LBTY_ErrorNullPointer;
    }
    else
    {
    for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++)
    {
        if (G_UserReq[idx_Buffer].BufferState==NotBuffered)
        {
            G_UserReq[idx_Buffer].Str=Add_pStr;
            G_UserWriteReq[idx_Buffer].CurrPos=0;
            G_UserReq[idx_Buffer].BufferState=Buffered;
            G_UserReq[idx_Buffer].Len=Copy_len;
            G_UserReq[idx_Buffer].Type=WriteReq;
            G_UserReq[idx_Buffer].State=CLCD_ReqStart;

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
tenu_ErrorStatus CLCD_GoToXYAsynch(u8 Copy_X , u8 Copy_Y)
{
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 idx_Buffer=0;
	if (Copy_X <= SECOND_LINE && Copy_Y < COLOUM_NUMBER)
	{
	    for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++)
	    {
	        if (G_UserReq[idx_Buffer].BufferState==NotBuffered)
	        {

	        		CLCD_PositionX=Copy_X;
	                CLCD_PositionY=Copy_Y;
	                G_UserReq[idx_Buffer].Type=SetPositionReq;
	                G_UserReq[idx_Buffer].BufferState=Buffered;
	                G_UserReq[idx_Buffer].State=CLCD_ReqStart;
	                break;
	        }
	    }

	}
    else
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }

    if (idx_Buffer==LCD_BUFFERSIZE)
    {
    	Local_ErrorStatus=LBTY_NOK;
    }




    return Local_ErrorStatus;
}
void CLCD_ClearScreenAsynch(void)
{
	u8 idx_Buffer=0;
	 for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++)
     {
		if (G_UserReq[idx_Buffer].BufferState==NotBuffered)
		 {
			 G_UserReq[idx_Buffer].Type=ClearReq;
			 G_UserReq[idx_Buffer].BufferState=Buffered;
			 G_UserReq[idx_Buffer].State=CLCD_ReqStart;
             break; 


		 }
	 }


}

void CLCD_WriteCommandAsynch(u8 Copy_Command){
	u8 idx_Buffer=0;
	 for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++)
     {
		if (G_UserReq[idx_Buffer].BufferState==NotBuffered)
		 {
			 G_UserReq[idx_Buffer].Command=Copy_Command;
			 G_UserReq[idx_Buffer].Type=CommandReq;
			 G_UserReq[idx_Buffer].BufferState=Buffered;
			 G_UserReq[idx_Buffer].State=CLCD_ReqStart;
              break; 


		 }
	 }

}
