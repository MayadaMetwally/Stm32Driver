/**************************************************************************************************************
 *************************            File Name: GPIO.c                                ************************
 *************************            Layer	   : MCAL                                  ************************
 *************************            Module   : GPIO                                  ************************
 *************************            Version  : V 0.0                                 ************************
 *************************            Created  : 2/13/2023                             ************************
 *************************            Author   : Mayada Metwally                       ************************
 *************************    Brief: This source file implements functions for GPIO driver*********************.
 **************************************************************************************************************/
#include "STD_TYPES.h"
#include "MGPIO/GPIO_Private.h"
#include "MGPIO/GPIO_Reg.h"
#include "MGPIO/GPIO.h"

#define GPIO_BSRR_RESET_OFFSET      0x00000010

/************************************Function Initializes the Pin************************************/
tenu_ErrorStatus MGPIO_InitPin(GPIO_Pin_tstr* ADD_PinCfg)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	u32 Local_Temp=0X0;
	if (ADD_PinCfg->Pin>GPIO_PIN_15||ADD_PinCfg->Port==NULL)
	{
		Local_ErrorStatus=LBTY_NOK;
	}
	else
	{
		Local_Temp=((GPIO_Reg*)(ADD_PinCfg->Port))->MODER;
		Local_Temp &=~((GPIO_CLEAR_MASK)<<((ADD_PinCfg->Pin)*GPIO_PIN_OFFSET_2));
		Local_Temp|=((ADD_PinCfg->Mode&GPIO_MODE_MASK)<<((ADD_PinCfg->Pin)*GPIO_PIN_OFFSET_2));
		((GPIO_Reg*)(ADD_PinCfg->Port))->MODER=Local_Temp;

		Local_Temp=((GPIO_Reg*)ADD_PinCfg->Port)->OTYPER;
		Local_Temp&=~(1<<ADD_PinCfg->Pin);
		Local_Temp|=(((GPIO_OTYPE_MASK&ADD_PinCfg->Mode)>>GPIO_PIN_OFFSET_2)<<(ADD_PinCfg->Pin));
		((GPIO_Reg*)ADD_PinCfg->Port)->OTYPER=Local_Temp;

		Local_Temp=(((GPIO_Reg*)ADD_PinCfg->Port))->PUPDR;
		Local_Temp&=~(GPIO_CLEAR_MASK<<ADD_PinCfg->Pin*GPIO_PIN_OFFSET_2);
		Local_Temp|=(((GPIO_PUPD_MASK&ADD_PinCfg->Mode)>>GPIO_PIN_OFFSET_3)<<ADD_PinCfg->Pin*GPIO_PIN_OFFSET_2);
		(((GPIO_Reg*)ADD_PinCfg->Port))->PUPDR=Local_Temp;

		Local_Temp=(((GPIO_Reg*)ADD_PinCfg->Port)->OSPEEDER);
		Local_Temp&=~(GPIO_CLEAR_MASK<<ADD_PinCfg->Pin*GPIO_PIN_OFFSET_2);
		Local_Temp|=(ADD_PinCfg->Speed<<ADD_PinCfg->Pin*GPIO_PIN_OFFSET_2);
		(((GPIO_Reg*)ADD_PinCfg->Port)->OSPEEDER)=Local_Temp;


	}

	return Local_ErrorStatus;
}
/************************************Function Set the Pin*****************************************/
tenu_ErrorStatus MGPIO_SetPin(void* Copy_Port, u8 Copy_Pin, u8 Copy_State)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if (Copy_Port==NULL || Copy_Pin>GPIO_PIN_15 || Copy_State>GPIO_High)
	{
		Local_ErrorStatus=LBTY_NOK;
	}
	else
	{
		switch(Copy_State)
		{
		    case GPIO_High:
		        ((GPIO_Reg*)Copy_Port)->BSRR|=(1<<Copy_Pin);
		    	 break;
		    case GPIO_Low:
		    	((GPIO_Reg*)Copy_Port)->BSRR|=(1<<(Copy_Pin+GPIO_BSRR_RESET_OFFSET));
		    	break;
		    default:
		    	Local_ErrorStatus=LBTY_NOK;

		}


	}

	return Local_ErrorStatus;
}
/************************************Function Toggle the Pin*****************************************/
tenu_ErrorStatus MGPIO_TogglePin(void* Copy_Port, u8 Copy_Pin)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if (Copy_Port==NULL || Copy_Pin>GPIO_PIN_15)
	{
		Local_ErrorStatus=LBTY_NOK;
	}
	else
	{
		 ((GPIO_Reg*)Copy_Port)->ODR^=(1<<Copy_Pin);


	}

	return Local_ErrorStatus;
}
/************************************Function get the Pin state************************************/
tenu_ErrorStatus MGPIO_GetPinState(void* Copy_Port, u8 Copy_Pin, u8 * ADD_State)
{
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if (ADD_State==NULL || Copy_Pin>GPIO_PIN_15 || Copy_Port==NULL)
		{
			Local_ErrorStatus=LBTY_NOK;
		}
		else
		{
			*ADD_State= ((((GPIO_Reg*)Copy_Port)->IDR >> Copy_Pin)&0x01);
		}


	return Local_ErrorStatus;
}
