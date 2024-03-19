/************************************************************************************************************
 *************************            File Name: LED.h                               ************************
 *************************            Layer	   : HAL                                 ************************
 *************************            Module   : LED                                 ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/16/2023                           ************************
 *************************            Author   : Mayada Metwally                     ************************
 *************************    Brief: This header file defines  functions for LED driver**********************.
 ************************************************************************************************************/

#ifndef HAL_HLED_LED_H_
#define HAL_HLED_LED_H_

#include "HLED/LED_Cfg.h"

#define LED_ACTIVE_HIGH         0
#define LED_ACTIVE_LOW          1

#define LED_ON                  1
#define LED_OFF                 0

typedef struct {
      void * Port;
      u32 Pin;
      u8 Connection;
      u8 DefaultState;
}LEDSCfg_tstr;

/**
 * @brief	 :Initializes the LED based on the provided configuration.
 * @param[in]:void.
 * @return	 :tenu_ErrorStatus status indicating the success or failure of the initialization.
 */
tenu_ErrorStatus HLED_Init(void);
tenu_ErrorStatus HLED_SetStatus(u32 Copy_Led, u8 Copy_status);
tenu_ErrorStatus HLED_GetStatus(u32 Copy_Led, u8 *ADD_status);
tenu_ErrorStatus HLED_Toggle(u32 Copy_Led);


#endif /* HAL_HLED_LED_H_ */
