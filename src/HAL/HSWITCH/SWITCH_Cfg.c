/*************************************************************************************************************
*************************            File Name: SWITCH_cfg.c                          ************************
 *************************            Layer	   : HAL                                  ************************
 *************************            Module   : SWITCH                               ************************
 *************************            Version  : V 0.0                                ************************
 *************************            Created  : 2/16/2023                            ************************
 *************************            Author   : Mayada Metwally                      ************************
 **************Brief: This Source file defines the user configuration externs for switch driver***************.
 ************************************************************************************************************/
#include "STD_TYPES.h"
#include "HSWITCH/SWITCH.h"
#include "MGPIO/GPIO.h"
const SWITCH_tstr SWITCH[_NUM_SWITCH]={
		[SWITCH_01]={.PORT=GPIOA,.PIN=GPIO_PIN_1,.MODE=GPIO_MODE_IN_PU},
		[SWITCH_02]={.PORT=GPIOA,.PIN=GPIO_PIN_2,.MODE=GPIO_MODE_IN_PU},
		[SWITCH_03]={.PORT=GPIOA,.PIN=GPIO_PIN_3,.MODE=GPIO_MODE_IN_PU},
		[SWITCH_04]={.PORT=GPIOA,.PIN=GPIO_PIN_4,.MODE=GPIO_MODE_IN_PU},

};

