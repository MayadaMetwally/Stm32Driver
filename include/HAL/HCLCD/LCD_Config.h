/************************************************************************************************************************************************/
/* File             :       HLCD_Config.h                                                                                                       */
/* AUTHOR           :       Mayada Metwally                                                                                                         */
/* Origin Date      :       3/20/2024                                                                                                            */
/* SWC              :       Character LCD                                                                                                       */
/************************************************************************************************************************************************/

#ifndef HLCD_HLCD_CONFIG_H_
#define HLCD_HLCD_CONFIG_H_

/************************************************************************************************************************************************/
/* Set the Operation mode of the LCD                                                                                                            */
/* Options:                                                                                                                                     */
/*      1-  HLCD_MODE_8_BIT                                                                                                                     */
/*      2-  HLCD_MODE_4_BIT                                                                                                                      */
/************************************************************************************************************************************************/
#define HLCD_MODE       HLCD_MODE_8_BIT


/************************************************************************************************************************************************/
/*                                              Initialization Options for the LCD                                                              */
/************************************************************************************************************************************************/


/* Lines mode for the LCD                                                                                                                       */
/* Options:                                                                                                                                     */  
/*      1- HLCD_ONE_LINE                                                                                                                         */
/*      2- HLCD_TWO_LINES                                                                                                                        */
#define HLCD_LINES_NUMBER       HLCD_TWO_LINES


/* Character formats                                                                                                                            */
/* Options:                                                                                                                                     */  
/*      1- HLCD_5X8_FORMAT                                                                                                                       */
/*      2- HLCD_5X11_FORMAT                                                                                                                      */
#define HLCD_FORMAT       HLCD_5X8_FORMAT


/* Increment and Decrement the Register Counter after write operation                                                                           */
/* Options:                                                                                                                                     */  
/*      1- HLCD_DECREMENT ------->  The Pointer counter decrements (Moves from right to left)                                                       */
/*      2- HLCD_INCREMENT ------->  The Pointer counter increments (Moves from left to right)                                                       */
#define HLCD_INCREMENT_DECREMENT    HLCD_INCREMENT


/* Shifts the entire display when enabled, shifts to the Left when Pointer Counter Decrements                                                
* and to the right when it Increments  
* Options:                                                                                                                                    
*      1- HLCD_DISABLE                                                     
*      2- HLCD_ENABLE                                                                                                        */
#define HLCD_SHIFTING               HLCD_DISABLE


/* Display Options for the LCD                                                                                                                  */
/* Options:                                                                                                                                     */  
/*      1- HLCD_DISPLAY_OFF                                                                                                                      */
/*      2- HLCD_DISPLAY_ON                                                                                                                       */
#define HLCD_DISPLAY                HLCD_DISPLAY_ON


/* Enable or Disable the LCD curser   
* Options:                                                                                                                                    
*      1- HLCD_DISABLE                                                     
*      2- HLCD_ENABLE                                                                                                           */
#define HLCD_CURSER                 HLCD_ENABLE


/* Enable or Disable the blinking of the curser 
* Options:                                                                                                                                    
*      1- HLCD_DISABLE                                                     
*      2- HLCD_ENABLE                                                                                                 */
#define HLCD_BLINKING               HLCD_ENABLE


#define LCD_BUFFERSIZE               30


typedef struct {
      void * Port;
      u32 Pin;     
}LCDSCfg_tstr;



typedef struct
{
    #if HLCD_MODE == HLCD_MODE_8_BIT
    LCDSCfg_tstr LCD_data_pins[8];
    #elif HLCD_MODE == HLCD_MODE_4_BIT
    LCDSCfg_tstr LCD_data_pins[4];
    #endif
    LCDSCfg_tstr  R_S_pin;
    LCDSCfg_tstr   R_W_pin;
    LCDSCfg_tstr    E_pin;
} LCD_cfg_t; /* to define the pins of the LCD*/

#endif /* HLCD_HLCD_CONFIG_H_ */