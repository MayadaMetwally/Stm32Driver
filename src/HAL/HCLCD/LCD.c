/************************************************************************************************************************************************/
/* File             :       HLCD.c                                                                                                         */
/* AUTHOR           :       Mayada Metwally                                                                                                        */
/* Origin Date      :       3/20/2024                                                                                                            */
/* SWC              :       Character LCD                                                                                                       */
/************************************************************************************************************************************************/


/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"
#include "HCLCD/LCD.h"
#include "MGPIO/GPIO.h"
#include <string.h> // for strcpy
#include<stdlib.h>



/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define POWER_PIN_NUMBER    3  // Define power pin number
#define ENABLE              1  // Define enable status
#define DISABLE             0  // Define disable status
#define COLOUM_NUMBER       16 // Define number of columns
#define FIRST_LINE          0 // Define first line number
#define SECOND_LINE         1 // Define second line number
#define DISPLAY_CLEAR       0X01 // Define command for clearing display
#define CLCD_NUMBER_OF_CGRAM_BYTES  8 
#define CLCD_SET_CGR_ADDRESS        64 
#define CLCD_FIRST_PART             0
#define CLCD_FIRST_PART_SEND        1
#define CLCD_SECOND_PART            2
#define CLCD_FIRST_SECOND_PART      3 



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
typedef void(*CallBackFunc )(void);  // Define callback function type

typedef enum{  // Define CLCD states
    CLCD_OFF,
    CLCD_Init_state,
    CLCD_Operatinal,
}CLCD_States_tenu;

typedef enum{  // Define CLCD initialization states
    PowerON,
    FunctionSet4bitMode,
    FunctionalSet,
    DisplayControl,
    DisplayClear,
    InitEnd
}CLCD_InitStates_tenu;

typedef enum  // Define user request types
{
    NOReq,
    WriteReq,
	WriteNumReq,
    WriteSpecialChar,
    DisplaySpecialChar,
    ClearReq,
    CommandReq,
    SetPositionReq,
    ReqDone
}CLCD_UserReqType_tenu;

typedef enum  // Define user request states
{
    CLCD_Idle,
    CLCD_Busy
}CLCD_UserReqState_tenu;

typedef enum  // Define buffer state
{
    NotBuffered,
    Buffered,
}CLCD_BufferState_tenu;

typedef enum  // Define write state
{
    CLCD_ReqStart,
    CLCD_ReqInprogress,
    CLCD_SetDDRAM,
    CLCD_DisplaySpecialChar,
    CLCD_ReqDone
}CLCD_WriteState_tenu;

typedef struct  // Define user request structure
{
    char *Str;
    u8 Len;
    u8 Command;
    s32 Number;
    u8 CGRAMBlockNumber;
    CLCD_UserReqType_tenu Type;
    CLCD_WriteState_tenu  State;
    CLCD_BufferState_tenu BufferState;
}CLCD_UserReq_tstr;

typedef struct  // Define write structure
{
    u8 CurrPos;
    CallBackFunc cb;
    u8 CLCD_PositionX; // Initialize X position
    u8 CLCD_PositionY; // Initialize Y position
}CLCD_Write_tstr;




/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern LCD_cfg_t HLCD; // Declare LCD configuration structure
static CLCD_States_tenu  G_CLCD_State = CLCD_OFF; // Initialize CLCD state
static CLCD_UserReq_tstr G_UserReq[LCD_BUFFERSIZE]; // Define array for user requests
static CLCD_Write_tstr   G_UserWriteReq[LCD_BUFFERSIZE]; // Define array for write requests
static u8 CLCD_EnablePin = DISABLE; // Initialize enable pin status
static u8 CLCD_ITER     =0; // Initialize iteration counter
static u8 CLCD_CurrentBuffer   = 0; // Initialize current buffer index
static u8 CLCD_BufferIndex     = 0; // Initialize buffer index
static u8 CLCD_PartCount=0;


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
 * @brief Writes a number to the LCD asynchronously.
 * 
 * This function writes a number to the LCD display asynchronously. It first reverses the number
 * and then writes each digit to the display. It handles cases when the number is 0 and disables
 * the LCD after completion.
 * 
 * @details
 * The function starts by initializing a static variable `Local_Reversed` to 1. It then checks the
 * state of the LCD write request. If the state is `CLCD_ReqStart`, it checks if the number to be
 * written is 0. If it is, the function writes '0' to the LCD, enables the LCD, sets the enable
 * pin high, and updates the request type and state. If the number is not 0, it moves to the
 * in-progress state.
 * 
 * In the in-progress state (`CLCD_ReqInprogress`), the function reverses the number using a while
 * loop and writes each digit to the LCD. It checks if the reversed number is not 1, and if so,
 * it writes each digit to the LCD, enables the LCD, and sets the enable pin high. Once the
 * writing is complete, it updates the request type and state, disables the LCD, and sets the
 * enable pin low.
 * 
 * @note This function assumes that the LCD control pins and the LCD write character process
 * functions (`CLCD_ControlEnablePin` and `CLCD_WriteCharProcess`) are defined elsewhere.
 */
static void CLCD_WriteNumProcess(void);
/**
 * @brief Processes the write special character request for the LCD.
 * 
 * This function processes the write special character request for the LCD based on the current state.
 */
static void CLCD_WriteSpecialCharProcess(void);

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
       case WriteNumReq:
    	CLCD_WriteNumProcess(); //write number request
    	break;
       case WriteSpecialChar:
        CLCD_WriteSpecialCharProcess();
        break; 
       case DisplaySpecialChar:
        if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
        {
            CLCD_WriteCharProcess(G_UserReq[CLCD_CurrentBuffer].CGRAMBlockNumber);
            CLCD_EnablePin=ENABLE; // Enable LCD
            CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

        }
        else
        {
            CLCD_EnablePin=DISABLE; // Disable LCD
            CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
            if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
            {
                G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
                G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Set request state to done
                if(CLCD_PartCount==CLCD_SECOND_PART)
                {
                    CLCD_PartCount=CLCD_FIRST_PART;
                }

            }


        }        
       
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
/***********************************************************************************************/
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
/***********************************************************************************************/
void CLCD_InitSM(void){
    static CLCD_InitStates_tenu SLocal_InitState = PowerON; // Declare local initialization state
    static u8 Counter =0; // Initialize counter for state transition delay
    switch (SLocal_InitState)
    {
    case PowerON:
        #if HLCD_MODE == HLCD_MODE_8_BIT
        SLocal_InitState = FunctionalSet; // Move to functional set state
        #elif HLCD_MODE == HLCD_MODE_4_BIT
        SLocal_InitState = FunctionSet4bitMode; // Move to functional set state        
        #endif
        CLCD_EnablePin=DISABLE; // Disable enable pin
        CLCD_ControlEnablePin(GPIO_Low); // Set enable pin to low
        break;
    case FunctionSet4bitMode:
        if(CLCD_EnablePin==DISABLE) // Check if enable pin is disabled
        {
            MGPIO_SetPin(HLCD.R_S_pin.Port,HLCD.R_S_pin.Pin,GPIO_Low); // Set RS pin low to indicate command mode
            MGPIO_SetPin(HLCD.R_W_pin.Port,HLCD.R_W_pin.Pin,GPIO_Low); // Set RW pin low to indicate write mode
            MGPIO_SetPin(HLCD.LCD_data_pins[0].Port,HLCD.LCD_data_pins[0].Pin,GPIO_Low);
            MGPIO_SetPin(HLCD.LCD_data_pins[1].Port,HLCD.LCD_data_pins[1].Pin,GPIO_High);
            MGPIO_SetPin(HLCD.LCD_data_pins[2].Port,HLCD.LCD_data_pins[2].Pin,GPIO_Low);
            MGPIO_SetPin(HLCD.LCD_data_pins[3].Port,HLCD.LCD_data_pins[3].Pin,GPIO_Low);
            CLCD_EnablePin=ENABLE; // Enable enable pin
            CLCD_ControlEnablePin(GPIO_High); // Set enable pin to high
        }
        else
        {
            CLCD_EnablePin=DISABLE; // Disable enable pin
            CLCD_ControlEnablePin(GPIO_Low); // Set enable pin to low   
            SLocal_InitState = FunctionalSet; // Move to functional set state      

        }    

        break;    
    case FunctionalSet:
    if(Counter>=19) // Check if counter reaches delay value
    {
        if(CLCD_EnablePin==DISABLE) // Check if enable pin is disabled
        {
            CLCD_SendCommandProcess(FUNCTION_SET); // Send function set command to LCD
            CLCD_EnablePin=ENABLE; // Enable enable pin
            CLCD_ControlEnablePin(GPIO_High); // Set enable pin to high
        }
        else
        {
            CLCD_EnablePin=DISABLE; // Disable enable pin
            CLCD_ControlEnablePin(GPIO_Low); // Set enable pin to low
            if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
            {
                SLocal_InitState = DisplayControl; // Move to display control state
                if(CLCD_PartCount==CLCD_SECOND_PART)
                {
                    CLCD_PartCount=CLCD_FIRST_PART;
                }

            }


        }
    } 
        Counter++; // Increment counter
        break;
    case DisplayControl:
        if(CLCD_EnablePin==DISABLE) // Check if enable pin is disabled
        {
            CLCD_SendCommandProcess(DISPLAY_ON_OFF); // Send display on/off command to LCD
            CLCD_EnablePin=ENABLE; // Enable enable pin
            CLCD_ControlEnablePin(GPIO_High); // Set enable pin to high
        }
        else
        {
            CLCD_EnablePin=DISABLE; // Disable enable pin
            CLCD_ControlEnablePin(GPIO_Low); // Set enable pin to low
            if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
            {
                SLocal_InitState = DisplayClear; // Move to display clear state
                if(CLCD_PartCount==CLCD_SECOND_PART)
                {
                    CLCD_PartCount=CLCD_FIRST_PART;
                }

            }

            

        }
        
        break;
    case DisplayClear:
        if(CLCD_EnablePin==DISABLE) // Check if enable pin is disabled
        {
            CLCD_SendCommandProcess(DISPLAY_CLEAR); // Send display clear command to LCD
            CLCD_EnablePin=ENABLE; // Enable enable pin
            CLCD_ControlEnablePin(GPIO_High); // Set enable pin to high
        }
        else
        {
            CLCD_EnablePin=DISABLE; // Disable enable pin
            CLCD_ControlEnablePin(GPIO_Low); // Set enable pin to low
            if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
            {
                SLocal_InitState =InitEnd; // Move to initialization end state
                if(CLCD_PartCount==CLCD_SECOND_PART)
                {
                    CLCD_PartCount=CLCD_FIRST_PART;
                }

            }         

        }    
        
        break;
    case InitEnd:
        G_CLCD_State = CLCD_Operatinal; // Set CLCD state to operational
        break;
                                          
    default:
        break;
    }

}
/***********************************************************************************************/
void CLCD_ControlEnablePin(u8 Copy_Pinstatus){
    MGPIO_SetPin(HLCD.E_pin.Port,HLCD.E_pin.Pin,Copy_Pinstatus); // Set the enable pin status
}
/***********************************************************************************************/
void CLCD_SendCommandProcess(u8 Copy_Command){
    u8 idx=0; // Declare index variable
    MGPIO_SetPin(HLCD.R_S_pin.Port,HLCD.R_S_pin.Pin,GPIO_Low); // Set RS pin low to indicate command mode
    MGPIO_SetPin(HLCD.R_W_pin.Port,HLCD.R_W_pin.Pin,GPIO_Low); // Set RW pin low to indicate write mode
    #if HLCD_MODE == HLCD_MODE_8_BIT
    for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++) // Loop through data pins
    {
        MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,((Copy_Command>>idx)&0x01)); // Set data pins according to command
    }
    CLCD_PartCount=CLCD_FIRST_SECOND_PART;

    #elif HLCD_MODE == HLCD_MODE_4_BIT
    if(CLCD_PartCount==CLCD_FIRST_PART)
    {
        for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++) // Loop through data pins
        {
            MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,((Copy_Command>>(idx+4))&0x01)); // Set data pins according to command
        }
        CLCD_PartCount=CLCD_FIRST_PART_SEND;
    }

    else if(CLCD_PartCount==CLCD_FIRST_PART_SEND)
    {
        for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++) // Loop through data pins
        {
            MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,((Copy_Command>>(idx))&0x01)); // Set data pins according to command
        }
        CLCD_PartCount=CLCD_SECOND_PART;
    }

    #endif
}
/***********************************************************************************************/
void CLCD_WriteProcess(void){

    switch (G_UserReq[CLCD_CurrentBuffer].State) // Check current state of the write request
    {
    case CLCD_ReqStart:
        G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress; // Move to in-progress state
    break;    
    case CLCD_ReqInprogress:
        if(G_UserWriteReq[CLCD_CurrentBuffer].CurrPos<G_UserReq[CLCD_CurrentBuffer].Len) // Check if there are more characters to write
        {
            if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
            {
                CLCD_ITER=G_UserWriteReq[CLCD_CurrentBuffer].CurrPos; // Set current position
                CLCD_WriteCharProcess(G_UserReq[CLCD_CurrentBuffer].Str[CLCD_ITER]); // Write character to LCD
                CLCD_EnablePin=ENABLE; // Enable LCD
                CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

            }
            else
            {
                CLCD_EnablePin=DISABLE; // Disable LCD
                CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
                if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
                {
                    G_UserWriteReq[CLCD_CurrentBuffer].CurrPos++; // Move to next character position
                    if(CLCD_PartCount==CLCD_SECOND_PART)
                    {
                        CLCD_PartCount=CLCD_FIRST_PART;
                    }

                }

               
            }
        }
        else
        {
            G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
            G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Set request state to done

        }
        break;

    default:
        break;
    }
}
/***********************************************************************************************/
void CLCD_WriteSpecialCharProcess(void)
{
    switch (G_UserReq[CLCD_CurrentBuffer].State) // Check current state of the write request
    {
    case CLCD_ReqStart:
        if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
        {
            CLCD_SendCommandProcess((G_UserReq[CLCD_CurrentBuffer].CGRAMBlockNumber)*(CLCD_NUMBER_OF_CGRAM_BYTES)+CLCD_SET_CGR_ADDRESS);
            CLCD_EnablePin=ENABLE; // Enable LCD
            CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

        }
        else
        {
            CLCD_EnablePin=DISABLE; // Disable LCD
            CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
            if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
            {
                G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress; // Move to in-progress state
                if(CLCD_PartCount==CLCD_SECOND_PART)
                {
                    CLCD_PartCount=CLCD_FIRST_PART;
                }

            }

            
        }


    break;    
    case CLCD_ReqInprogress:
        if(G_UserWriteReq[CLCD_CurrentBuffer].CurrPos<G_UserReq[CLCD_CurrentBuffer].Len) // Check if there are more characters to write
        {
            if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
            {
                CLCD_ITER=G_UserWriteReq[CLCD_CurrentBuffer].CurrPos; // Set current position
                CLCD_WriteCharProcess(G_UserReq[CLCD_CurrentBuffer].Str[CLCD_ITER]); // Write character to LCD
                CLCD_EnablePin=ENABLE; // Enable LCD
                CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

            }
            else
            {
                CLCD_EnablePin=DISABLE; // Disable LCD
                CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
                if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
                {
                    G_UserWriteReq[CLCD_CurrentBuffer].CurrPos++; // Move to next character position
                    if(CLCD_PartCount==CLCD_SECOND_PART)
                    {
                        CLCD_PartCount=CLCD_FIRST_PART;
                    }

                }

                
            }
        }

        else
        {
           G_UserReq[CLCD_CurrentBuffer].State=CLCD_SetDDRAM; // Set request state to done

        }
        break;
    case CLCD_SetDDRAM:
        if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
        {
            CLCD_SendCommandProcess(START_ADDREESS);
            CLCD_EnablePin=ENABLE; // Enable LCD
            CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

        }
        else
        {
            CLCD_EnablePin=DISABLE; // Disable LCD
            CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
            if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
            {
                G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
                G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Set request state to done
                if(CLCD_PartCount==CLCD_SECOND_PART)
                {
                    CLCD_PartCount=CLCD_FIRST_PART;
                }

            }
            

        }    

        break; 
    default:
        break;
    }
}


/***********************************************************************************************/
void CLCD_WriteCharProcess(u8 Copy_Char){
    u8 idx=0; // Declare index variable
   
    MGPIO_SetPin(HLCD.R_S_pin.Port,HLCD.R_S_pin.Pin,GPIO_High); // Set RS pin high to indicate data mode
    MGPIO_SetPin(HLCD.R_W_pin.Port,HLCD.R_W_pin.Pin,GPIO_Low); // Set RW pin low to indicate write mode
    #if HLCD_MODE == HLCD_MODE_8_BIT
    for (idx=0 ; idx <HLCD_PINS_NUMBER; idx++) // Loop through data pins
    {
        MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,(Copy_Char>>idx)&1); // Set data pins according to character
    }
    #elif HLCD_MODE == HLCD_MODE_4_BIT
    if(CLCD_PartCount==CLCD_FIRST_PART)
    {
        for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++) // Loop through data pins
        {
            MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,((Copy_Char>>(idx+4))&0x01)); // Set data pins according to command
        }
        CLCD_PartCount=CLCD_FIRST_PART_SEND;
    }

    else if(CLCD_PartCount==CLCD_FIRST_PART_SEND)
    {
        for(idx=0 ; idx<HLCD_PINS_NUMBER ; idx++) // Loop through data pins
        {
            MGPIO_SetPin(HLCD.LCD_data_pins[idx].Port,HLCD.LCD_data_pins[idx].Pin,((Copy_Char>>(idx))&0x01)); // Set data pins according to command
        }
        CLCD_PartCount=CLCD_SECOND_PART;
    }

    #endif
}
/***********************************************************************************************/
void CLCD_SendCommandHlp(u8 Copy_Command)
{
    switch (G_UserReq[CLCD_CurrentBuffer].State) // Check current state of the request
    {
    case CLCD_ReqStart:
        G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress; // Move to in-progress state
        break;
    case CLCD_ReqInprogress:

            if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
            {
                CLCD_SendCommandProcess(Copy_Command); // Send command to LCD
                CLCD_EnablePin=ENABLE; // Enable LCD
                CLCD_ControlEnablePin(GPIO_High); // Set enable pin high
            }
            else
            {
                CLCD_EnablePin=DISABLE; // Disable LCD
                CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
                if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
                {
                    G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
                    G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Set request state to done
                    if(CLCD_PartCount==CLCD_SECOND_PART)
                    {
                        CLCD_PartCount=CLCD_FIRST_PART;
                    }

                }                

            }

        break;

    default:
        break;
    }
}
/***********************************************************************************************/
void CLCD_GoToXYProcess(void)
{
    switch (G_UserReq[CLCD_CurrentBuffer].State) // Check the state of the current request
    {
    case CLCD_ReqStart: // If request has just started
        	G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress; // Update request state to in-progress
        break;
    case CLCD_ReqInprogress: // If request is in progress
    	if(CLCD_EnablePin==DISABLE) // Check if LCD is not busy
    	 {
    		if (G_UserWriteReq[CLCD_CurrentBuffer].CLCD_PositionX == FIRST_LINE && G_UserWriteReq[CLCD_CurrentBuffer].CLCD_PositionY < COLOUM_NUMBER) // Check if cursor position is on the first line and within column bounds
    		{
    			CLCD_SendCommandProcess((G_UserWriteReq[CLCD_CurrentBuffer].CLCD_PositionY-1)+128); // Send command to set cursor position on the first line
    		}
    		else if (G_UserWriteReq[CLCD_CurrentBuffer].CLCD_PositionX == SECOND_LINE && G_UserWriteReq[CLCD_CurrentBuffer].CLCD_PositionY < COLOUM_NUMBER) // Check if cursor position is on the second line and within column bounds
    		{
    			CLCD_SendCommandProcess((G_UserWriteReq[CLCD_CurrentBuffer].CLCD_PositionY-1)+192); // Send command to set cursor position on the second line
    		}
            else{

            }

    	    CLCD_EnablePin=ENABLE; // Enable LCD control pins
    	    CLCD_ControlEnablePin(GPIO_High); // Set control pin to high
    	 }
       else // If LCD is busy
    	{
    	    CLCD_EnablePin=DISABLE; // Disable LCD control pins
    	    CLCD_ControlEnablePin(GPIO_Low); // Set control pin to low

            if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
            {
                G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
                G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Set request state to done
                if(CLCD_PartCount==CLCD_SECOND_PART)
                {
                  CLCD_PartCount=CLCD_FIRST_PART;
                }

            }  
    	}
    	break;
    default:
        break;
    }
}
/***********************************************************************************************/
void CLCD_WriteNumProcess(void) {
    static s32 Local_Reversed = 1; // Initialize outside the function
    switch (G_UserReq[CLCD_CurrentBuffer].State) {
    case CLCD_ReqStart:
       	if(G_UserReq[CLCD_CurrentBuffer].Number==0)

       	{  if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
               {

                   CLCD_WriteCharProcess('0'); // Write character to LCD
                   CLCD_EnablePin=ENABLE; // Enable LCD
                   CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

               }
               else
               {
                   CLCD_EnablePin=DISABLE; // Disable LCD
                   CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
                    if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
                    {
                        G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
                        G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Set request state to done
                        if(CLCD_PartCount==CLCD_SECOND_PART)
                        {
                            CLCD_PartCount=CLCD_FIRST_PART;
                        }

                    }  

               }
       	}
       	else
       	{
            if(G_UserReq[CLCD_CurrentBuffer].Number<0)
            {
              if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
               {

                   CLCD_WriteCharProcess('-');  // Write character to LCD
                   G_UserReq[CLCD_CurrentBuffer].Number*=-1; 
                   CLCD_EnablePin=ENABLE; // Enable LCD
                   CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

               }
               else
               {
                   CLCD_EnablePin=DISABLE; // Disable LCD
                   CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
                    if(CLCD_PartCount==CLCD_SECOND_PART||CLCD_PartCount==CLCD_FIRST_SECOND_PART)
                    {
                        G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
                        G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Set request state to done
                        if(CLCD_PartCount==CLCD_SECOND_PART)
                        {
                            CLCD_PartCount=CLCD_FIRST_PART;
                        }

                    }  

               }
 
            }
            else
            {
                G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqInprogress; // Move to in-progress state
            }
            
       	}

       break;
       case CLCD_ReqInprogress:

       		while(G_UserReq[CLCD_CurrentBuffer].Number!=0)
       		{
       			Local_Reversed=(Local_Reversed*10)+(G_UserReq[CLCD_CurrentBuffer].Number%10);
       			G_UserReq[CLCD_CurrentBuffer].Number/=10;
       		}
       		if(Local_Reversed!=1)

           	{  if(CLCD_EnablePin==DISABLE) // Check if LCD enable pin is disabled
                {

                   CLCD_WriteCharProcess((Local_Reversed%10)+'0'); // Write character to LCD
                   Local_Reversed/=10;
                   CLCD_EnablePin=ENABLE; // Enable LCD
                   CLCD_ControlEnablePin(GPIO_High); // Set enable pin high

                }
               else
               {
                   CLCD_EnablePin=DISABLE; // Disable LCD
                   CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low

                    if(CLCD_PartCount==CLCD_SECOND_PART)
                    {
                        CLCD_PartCount=CLCD_FIRST_PART;
                    }
               }
           	}
       		else
       		{
                G_UserReq[CLCD_CurrentBuffer].Type=ReqDone; // Set request type to done
                G_UserReq[CLCD_CurrentBuffer].State=CLCD_ReqDone; // Update request state to done
                CLCD_EnablePin=DISABLE; // Disable LCD
                CLCD_ControlEnablePin(GPIO_Low); // Set enable pin low
       		}


           break;

       default:
           break;
       }
}
/******************************************user Functions*****************************************************/
tenu_ErrorStatus CLCD_WriteStringAsynch(char * Add_pStr , u8 Copy_len){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK; // Initialize local error status
    u8 idx_Buffer=0; // Declare buffer index variable
    if(Add_pStr==NULL) // Check if string pointer is NULL
    {
        Local_ErrorStatus = LBTY_ErrorNullPointer; // Set error status to null pointer error
    }
    else
    {
    for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++) // Loop through LCD buffers
    {
        if (G_UserReq[idx_Buffer].BufferState==NotBuffered) // Check if buffer is not currently buffered
        {
            G_UserReq[idx_Buffer].Str=Add_pStr; // Set string pointer
            G_UserWriteReq[idx_Buffer].CurrPos=0; // Reset current position
            G_UserReq[idx_Buffer].BufferState=Buffered; // Set buffer state to buffered
            G_UserReq[idx_Buffer].Len=Copy_len; // Set string length
            G_UserReq[idx_Buffer].Type=WriteReq; // Set request type to write
            G_UserReq[idx_Buffer].State=CLCD_ReqStart; // Set request state to request start

            break; // Exit loop
        }        
    }   
    if (idx_Buffer==LCD_BUFFERSIZE) // Check if buffer index reaches buffer size
    {
        Local_ErrorStatus=LBTY_NOK; // Set error status to not ok
    }

    }
    return Local_ErrorStatus; // Return error status
}
/******************************************user Functions*****************************************************/
tenu_ErrorStatus CLCD_GoToXYAsynch(u8 Copy_X , u8 Copy_Y)
{
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK; // Initialize local error status
    u8 idx_Buffer=0; // Declare buffer index variable
	if (Copy_X <= SECOND_LINE && Copy_Y < COLOUM_NUMBER) // Check if row and column numbers are within valid range
	{
	    for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++) // Loop through LCD buffers
	    {
	        if (G_UserReq[idx_Buffer].BufferState==NotBuffered) // Check if buffer is not currently buffered
	        {

	        		G_UserWriteReq[idx_Buffer].CLCD_PositionX=Copy_X; // Set current row
	                G_UserWriteReq[idx_Buffer].CLCD_PositionY=Copy_Y; // Set current column
	                G_UserReq[idx_Buffer].Type=SetPositionReq; // Set request type to set position
	                G_UserReq[idx_Buffer].BufferState=Buffered; // Set buffer state to buffered
	                G_UserReq[idx_Buffer].State=CLCD_ReqStart; // Set request state to request start
	                break; // Exit loop
	        }
	    }

	}
    else
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput; // Set error status to invalid input
    }

    if (idx_Buffer==LCD_BUFFERSIZE) // Check if buffer index reaches buffer size
    {
    	Local_ErrorStatus=LBTY_NOK; // Set error status to not ok
    }

    return Local_ErrorStatus; // Return error status
}
/******************************************user Functions*****************************************************/
tenu_ErrorStatus CLCD_ClearScreenAsynch(void)
{
    tenu_ErrorStatus Local_ErrorStatus= LBTY_OK;
    u8 idx_Buffer=0; // Declare buffer index variable
	for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++) // Loop through LCD buffers
    {
		if (G_UserReq[idx_Buffer].BufferState==NotBuffered) // Check if buffer is not currently buffered
		{
			G_UserReq[idx_Buffer].Type=ClearReq; // Set request type to clear screen
			G_UserReq[idx_Buffer].BufferState=Buffered; // Set
            G_UserReq[idx_Buffer].State=CLCD_ReqStart; // Set request state to request start
            break; // Exit loop


		}
	}
    if (idx_Buffer==LCD_BUFFERSIZE) // Check if buffer index reaches buffer size
    {
        Local_ErrorStatus=LBTY_NOK; // Set error status to not ok
    }
    return Local_ErrorStatus;


}
/******************************************user Functions*****************************************************/
tenu_ErrorStatus CLCD_WriteCommandAsynch(u8 Copy_Command){
    tenu_ErrorStatus Local_ErrorStatus =LBTY_OK;
    u8 idx_Buffer=0; // Declare buffer index variable
     for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++) // Loop through LCD buffers
     {
        if (G_UserReq[idx_Buffer].BufferState==NotBuffered) // Check if buffer is not currently buffered
         {
             G_UserReq[idx_Buffer].Command=Copy_Command; // Set command to be written
             G_UserReq[idx_Buffer].Type=CommandReq; // Set request type to command
             G_UserReq[idx_Buffer].BufferState=Buffered; // Set buffer state to buffered
             G_UserReq[idx_Buffer].State=CLCD_ReqStart; // Set request state to request start
              break; // Exit loop
         }
     }
    if (idx_Buffer==LCD_BUFFERSIZE) // Check if buffer index reaches buffer size
    {
        Local_ErrorStatus=LBTY_NOK; // Set error status to not ok
    }
     return Local_ErrorStatus;
}
/******************************************user Functions*****************************************************/
tenu_ErrorStatus CLCD_WriteNumberAsynch(s32 Copy_Number)
{
	tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 idx_Buffer=0;
        for(idx_Buffer=0;idx_Buffer<LCD_BUFFERSIZE;idx_Buffer++) // Loop through LCD buffers
    {
        if (G_UserReq[idx_Buffer].BufferState==NotBuffered) // Check if buffer is not currently buffered
        {
            G_UserReq[idx_Buffer].Number=Copy_Number; // Copy the string to the buffer
            G_UserReq[idx_Buffer].BufferState=Buffered; // Set buffer state to buffered
            G_UserReq[idx_Buffer].Type=WriteNumReq; // Set request type to write
            G_UserReq[idx_Buffer].State=CLCD_ReqStart; // Set request state to request start

            break; // Exit loop
        }
    }
    if (idx_Buffer==LCD_BUFFERSIZE) // Check if buffer index reaches buffer size
    {
        Local_ErrorStatus=LBTY_NOK; // Set error status to not ok
    }



	return Local_ErrorStatus;
}
/******************************************user Functions*****************************************************/

tenu_ErrorStatus CLCD_WriteSpecialCharAsynch(char * Add_Pattern, u8 Copy_CGRAMBlockNumber)
{
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 idx_Buffer=0;
    if(Add_Pattern==NULL)
    {
        Local_ErrorStatus =LBTY_ErrorNullPointer;
    }
    else if (Copy_CGRAMBlockNumber>CLCD_NUMBER_OF_CGRAM_BYTES)
    {
        Local_ErrorStatus =LBTY_ErrorInvalidInput;
    }
    else
    {
        for(idx_Buffer=0 ; idx_Buffer<LCD_BUFFERSIZE ; idx_Buffer++)
        {
            if(G_UserReq[idx_Buffer].BufferState==NotBuffered)
            {
            G_UserReq[idx_Buffer].Str=Add_Pattern; // Set string pointer
            G_UserReq[idx_Buffer].CGRAMBlockNumber=Copy_CGRAMBlockNumber;
            G_UserWriteReq[idx_Buffer].CurrPos=0; // Reset current position
            G_UserReq[idx_Buffer].BufferState=Buffered; // Set buffer state to buffered
            G_UserReq[idx_Buffer].Len=CLCD_NUMBER_OF_CGRAM_BYTES; // Set string length
            G_UserReq[idx_Buffer].Type=WriteSpecialChar; // Set request type to write
            G_UserReq[idx_Buffer].State=CLCD_ReqStart; // Set request state to request start
            break;


            }
         }
        if (idx_Buffer==LCD_BUFFERSIZE) // Check if buffer index reaches buffer size
        {
    	    Local_ErrorStatus=LBTY_NOK; // Set error status to not ok
        }
    }
    

    return Local_ErrorStatus;

}
tenu_ErrorStatus CLCD_DisplaySpecialCharAsynch( u8 Copy_CGRAMBlockNumber)
{
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 idx_Buffer=0;
    if (Copy_CGRAMBlockNumber>CLCD_NUMBER_OF_CGRAM_BYTES)
    {
        Local_ErrorStatus =LBTY_ErrorInvalidInput;
    }
    else
    {
        for(idx_Buffer=0 ; idx_Buffer<LCD_BUFFERSIZE ; idx_Buffer++)
        {
            if(G_UserReq[idx_Buffer].BufferState==NotBuffered)
            {

            G_UserReq[idx_Buffer].CGRAMBlockNumber=Copy_CGRAMBlockNumber;
            G_UserReq[idx_Buffer].Type=DisplaySpecialChar; // Set request type to write
            G_UserReq[idx_Buffer].State=CLCD_ReqStart; // Set request state to request start
            G_UserReq[idx_Buffer].BufferState=Buffered; // Set buffer state to buffered
            break;


            }
         }
        if (idx_Buffer==LCD_BUFFERSIZE) // Check if buffer index reaches buffer size
        {
    	    Local_ErrorStatus=LBTY_NOK; // Set error status to not ok
        }
    }
    

    return Local_ErrorStatus;

}