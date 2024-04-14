/************************************************************************************************************
 *************************            File Name: LED.c                               ************************
 *************************            Layer	   : HAL                                 ************************
 *************************            Module   : LED                                 ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/16/2023                           ************************
 *************************            Author   : Mayada Metwally                     ************************
 *************************    Brief: This source file implements functions for LED driver********************.
 ************************************************************************************************************/

#include "STD_TYPES.h"
#include "HLED/LED.h"
#include "MGPIO/GPIO.h"
#include "MRCC/RCC.h"

extern  const LEDSCfg_tstr LEDS[_LED_NUM];
tenu_ErrorStatus HLED_Init(void)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	GPIO_Pin_tstr PIN;
	PIN.Mode=GPIO_MODE_OP_PP;
	PIN.Speed=GPIO_SPEED_HIGH;
	for(u8 idk=0; idk<_LED_NUM;idk++)
	{
		PIN.Pin=LEDS[idk].Pin;
		PIN.Port=LEDS[idk].Port;
		MGPIO_InitPin(&PIN);
	}
	return Local_ErrorStatus;
}


tenu_ErrorStatus HLED_SetStatus(u32 Copy_Led, u8 Copy_status)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(Copy_Led>(_LED_NUM -1)|| Copy_status>LED_ACTIVE_LOW)
	{
		Local_ErrorStatus=LBTY_NOK;
	}
	else
	{
		MGPIO_SetPin(LEDS[Copy_Led].Port,LEDS[Copy_Led].Pin,(LEDS[Copy_Led].Connection)^Copy_status);
	}

	return Local_ErrorStatus;
}

tenu_ErrorStatus HLED_Toggle(u32 Copy_Led)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(Copy_Led>_LED_NUM )
	{
		Local_ErrorStatus=LBTY_NOK;
	}
	else
	{
		Local_ErrorStatus=MGPIO_TogglePin(LEDS[Copy_Led].Port,LEDS[Copy_Led].Pin);
	}

	return Local_ErrorStatus;
}

tenu_ErrorStatus HLED_GetStatus(u32 Copy_Led, u8 *ADD_status)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	u8 Local_status=0;
	if(Copy_Led>_LED_NUM || ADD_status==NULL)
	{
		Local_ErrorStatus=LBTY_NOK;
	}
	else
	{
		Local_ErrorStatus=MGPIO_GetPinState(LEDS[Copy_Led].Port,LEDS[Copy_Led].Pin,&Local_status);
		*ADD_status=Local_status^LEDS[Copy_Led].Connection;
	}

	return Local_ErrorStatus;
}
