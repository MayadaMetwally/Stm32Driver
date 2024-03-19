/************************************************************************************************************
 *************************            File Name: SWITCH.h                             ************************
 *************************            Layer	   : HAL                                  ************************
 *************************            Module   : SWITCH                               ************************
 *************************            Version  : V 0.0                                ************************
 *************************            Created  : 2/16/2023                            ************************
 *************************            Author   : Mayada Metwally                      ************************
 *************************    Brief: This header file defines  functions for SWITCH driver********************.
 ************************************************************************************************************/
#include "HSWITCH/SWITCH_Cfg.h"

#ifndef HAL_HSWITCH_SWITCH_H_
#define HAL_HSWITCH_SWITCH_H_

#define SWITCH_PULL_UP                    1
#define SWITCH_PULL_DOWN                  0

#define SWITCH_PRESSED                    1
#define SWITCH_NOT_PRESSED                0

#define SWITCH_OFFSET_4                   4

typedef struct{
	void *PORT;
	u32  PIN;
	u32  MODE;
}SWITCH_tstr;


tenu_ErrorStatus HSWITCH_Init(void);
tenu_ErrorStatus HSWITCH_GetSwitchStatus(u8 Copy_SwitchName, u8 *ADD_SwitchStatus);



#endif /* HAL_HSWITCH_SWITCH_H_ */
