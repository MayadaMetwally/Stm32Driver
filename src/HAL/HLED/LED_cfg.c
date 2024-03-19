/************************************************************************************************************
 *************************            File Name: LED_Cfg.h                           ************************
 *************************            Layer	   : HAL                                 ************************
 *************************            Module   : LED                                 ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/16/2023                           ************************
 *************************            Author   : Mayada Metwally                     ************************
 **************Brief: This Source file defines the user configuration externs for LED driver*****************.
 ************************************************************************************************************/
#include "STD_TYPES.h"
#include"HLED/LED.h"
#include "MGPIO/GPIO.h"
 const LEDSCfg_tstr LEDS[_LED_NUM]={
	[LED_GREEN]={.Port=GPIOB,.Pin=GPIO_PIN_1,.Connection=LED_ACTIVE_HIGH,.DefaultState=LED_OFF},
	[LED_RED]={.Port=GPIOB,.Pin=GPIO_PIN_2,.Connection=LED_ACTIVE_HIGH,.DefaultState=LED_OFF},
	[LED_YELLOW]={.Port=GPIOB,.Pin=GPIO_PIN_3,.Connection=LED_ACTIVE_HIGH,.DefaultState=LED_OFF},
	[LED_RED2]={.Port=GPIOB,.Pin=GPIO_PIN_4,.Connection=LED_ACTIVE_HIGH,.DefaultState=LED_OFF},
	[LED_GREEN2]={.Port=GPIOB,.Pin=GPIO_PIN_5,.Connection=LED_ACTIVE_HIGH,.DefaultState=LED_OFF}


};
