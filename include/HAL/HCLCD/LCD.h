#ifndef HCLCD_LCD_H_
#define HCLCD_LCD_H_
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LCD_Config.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/************************************************************************************************************************************************/
/* Operation modes available for the LCD                                                                                                        */
/************************************************************************************************************************************************/
#define HLCD_MODE_4_BIT     1
#define HLCD_MODE_8_BIT     2


/************************************************************************************************************************************************/
/* LCD Options states                                                                                                                           */
/************************************************************************************************************************************************/
#define HLCD_DISABLE     0
#define HLCD_ENABLE      1

/************************************************************************************************************************************************/
/* Increment and Decrement the Register Counter after write operation                                                                           */
/************************************************************************************************************************************************/
#define HLCD_DECREMENT      0
#define HLCD_INCREMENT      1

/************************************************************************************************************************************************/
/* Display Options for the LCD                                                                                                                  */
/************************************************************************************************************************************************/
#define HLCD_DISPLAY_OFF             0
#define HLCD_DISPLAY_ON              1

/************************************************************************************************************************************************/
/* Line Modes                                                                                                                                   */
/************************************************************************************************************************************************/
#define HLCD_ONE_LINE       0
#define HLCD_TWO_LINES      1


/************************************************************************************************************************************************/
/* Character formats                                                                                                                            */
/************************************************************************************************************************************************/
#define HLCD_5X8_FORMAT     0
#define HLCD_5X11_FORMAT    1
/******************************************************************************/
/* COMMANDS     */      
/*****************************************************************************/
#define HLCD_CLEAR_DISPLAY              0x01
#define HLCD_SET_DDR_ADDRESS            0X80
#define HLCD_SET_CGR_ADDRESS            0X40
#define HLCD_SHIFT_SCREEN_LEFT          0x18
#define HLCD_SHIFT_SCREEN_RIGHT         0x1C


#define HLCD_SMILY_FACE     {0b00000,0b01010,0b01010,0b00000,0b10001,0b01110,0b00000,0b00000}
#define HLCD_HEART          {0b00000,0b00000,0b01010,0b10101,0b10001,0b01110,0b00100,0b00000}
#define HLCD_PERSON         {0b01110,0b01110,0b00100,0b01110,0b10101,0b00100,0b01010,0b10001}
#define HLCD_PERSON2        {0b00100,0b01110,0b11111,0b11111,0b01110,0b01110,0b01010,0b01010}
#define HLCD_LOCK           {0b01110,0b10001,0b10001,0b11111,0b11011,0b11011,0b11111,0b00000}
#define HLCD_UNLOCK         {0b01110,0b10000,0b10000,0b11111,0b11011,0b11011,0b11111,0b00000}
#define HLCD_FACE1          {0b00000,0b10001,0b01010,0b10001,0b00100,0b01110,0b10001,0b00000}
#define HLCD_FACE_MASK      {0b11111,0b11111,0b10101,0b11011,0b11011,0b11111,0b10001,0b11111}
#define HLCD_BELL           {0b00100,0b01110,0b01010,0b01010,0b01010,0b11111,0b00000,0b00100}


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
/**
 * @brief This function initializes the asynchronous initialization of the LCD.
 *
 * This function initializes the GPIO pins used for the LCD. It sets their modes and speeds
 * according to the configurations provided. It sets the CLCD state to initialization state.
 *
 * @return None
 */
void CLCD_InitAsynch(void);
/**
 * @brief Clears the LCD screen.
 *
 * This function clears the entire content displayed on the LCD screen.
 * It buffers the clear screen request for asynchronous processing.
 *
 * @return None
 */
void CLCD_ClearScreenAsynch(void);
/**
 * @brief Sets the cursor position on the LCD.
 *
 * This function sets the cursor position on the LCD display to the specified row and column.
 * It buffers the set position request for asynchronous processing.
 *
 * @param Copy_X: Row number (1 or 2).
 * @param Copy_Y: Column number (0 to 15).
 *
 * @return tenu_ErrorStatus: Status of the set position operation (LBTY_OK if successful, LBTY_ErrorInvalidInput if the input parameters are invalid, LBTY_NOK if unable to buffer the request).
 */
tenu_ErrorStatus CLCD_GoToXYAsynch(u8 Copy_x , u8 Copy_y); 
/**
 * @brief Writes a string to the LCD.
 *
 * This function writes a string of characters to the LCD display. It takes the string and its length as input
 * parameters and buffers the write request for asynchronous processing.
 *
 * @param Add_pStr: Pointer to the string to be written.
 * @param Copy_len: Length of the string to be written.
 *
 * @return tenu_ErrorStatus: Status of the write operation (LBTY_OK if successful, LBTY_ErrorNullPointer if the string pointer is NULL, LBTY_NOK if unable to buffer the request).
 */
tenu_ErrorStatus CLCD_WriteStringAsynch(char * Add_pStr , u8 Copy_len);
/**
 * @brief Writes a command to the LCD.
 *
 * This function writes a command to the LCD. It buffers the command write request for asynchronous processing.
 *
 * @param Copy_Command: The command to be written to the LCD.
 *
 * @return None
 */
void CLCD_WriteCommandAsynch(u8 Copy_Command);


#endif// HCLCD_LCD_H_
