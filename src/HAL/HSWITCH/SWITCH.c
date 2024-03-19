/************************************************************************************************************
 *************************            File Name: SWITCH.c                             ************************
 *************************            Layer	   : HAL                                  ************************
 *************************            Module   : SWITCH                               ************************
 *************************            Version  : V 2.0                                ************************
 *************************            Created  : 2/16/2023                            ************************
 *************************            Author   : Mayada Metwally                      ************************
 *************************    Brief: This source file implements functions for LED driver********************.
 ************************************************************************************************************/

#include "STD_TYPES.h"
#include "HSWITCH/SWITCH.h"
#include "MGPIO/GPIO.h"
#include "MRCC/RCC.h"
#define THRESHOLD        5  



extern const SWITCH_tstr SWITCH[_NUM_SWITCH];
static u8 SwitchState[_NUM_SWITCH];
void HSWITCH_Runnable(void);
 
tenu_ErrorStatus HSWITCH_Init(void)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	MRCC_ControlClockAHP1Peripheral(RCC_AHB1_GPIOA,RCC_ENABLE);
	GPIO_Pin_tstr Switch_Pin;
	Switch_Pin.Speed=GPIO_SPEED_HIGH;
	for(u8 idk=0;idk<_NUM_SWITCH;idk++)
	{
		Switch_Pin.Mode=SWITCH[idk].MODE;
		Switch_Pin.Pin=SWITCH[idk].PIN;
		Switch_Pin.Port=SWITCH[idk].PORT;
		MGPIO_InitPin(&Switch_Pin);
	}
	return Local_ErrorStatus;
}
/**
 * @brief Runnable function to handle switch state changes.
 *
 * This function is periodically called to monitor the state changes of each switch.
 * It checks the current state of each switch, compares it with the previous state,
 * and updates the switch counts accordingly. If the count reaches a threshold (5),
 * it updates the switch state and resets the count.
 */

void HSWITCH_Runnable(void)
{
	u8 Local_Current;
	static u8 Local_Prev[_NUM_SWITCH]={0};
	static u8 Local_Counts[_NUM_SWITCH]={0};
	for (u8 idx=0 ; idx<_NUM_SWITCH ; idx++)
	{
		MGPIO_GetPinState(SWITCH[idx].PORT,SWITCH[idx].PIN,&Local_Current);
		if(Local_Current==Local_Prev[idx])
		{
			Local_Counts[idx]++;
		}
		else
		{
			Local_Counts[idx]=0;
		}
		if(Local_Counts[idx]==THRESHOLD)
		{
			SwitchState[idx]=Local_Current;
			Local_Counts[idx]=0;
		}
		Local_Prev[idx]=Local_Current;
	}
	




}
tenu_ErrorStatus HSWITCH_GetSwitchStatus(u8 Copy_SwitchName, u8 *ADD_SwitchStatus)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(ADD_SwitchStatus==NULL||Copy_SwitchName>_NUM_SWITCH)
	{
		Local_ErrorStatus=LBTY_NOK;
	}
	*ADD_SwitchStatus=!(SwitchState[Copy_SwitchName]^(SWITCH[Copy_SwitchName].MODE>>SWITCH_OFFSET_4));

	return Local_ErrorStatus;
}
