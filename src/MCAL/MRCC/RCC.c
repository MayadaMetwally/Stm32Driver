/**************************************************************************************************************
 *************************            File Name: RCC.c                                 ************************
 *************************            Layer	   : MCAL                                  ************************
 *************************            Module   : RCC                                   ************************
 *************************            Version  : V 0.0                                 ************************
 *************************            Created  : 2/4/2023                              ************************
 *************************            Author   : Mayada Metwally                       ************************
 *************************    Brief: This source file implements functions for RCC driver**********************.
 **************************************************************************************************************/

#include "STD_TYPES.h"


#include "MRCC/RCC_Reg.h"
#include "MRCC/RCC_Private.h"
#include "MRCC/RCC_Cfg.h"
#include "MRCC/RCC.h"

/**************************************Enable peripheral clk***********************************************/
tenu_ErrorStatus MRCC_EnablePeripheralClock(u8 BusId,u8 PerId)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	if(PerId<=RCC_LPAHB2)
	{
	    switch(BusId)
	    {
	        case RCC_AHB1   : RCC_AHB1ENR   |=(PerId);  break;
	        case RCC_AHB2   : RCC_AHB2ENR   |=(PerId);  break;
	        case RCC_APB1   : RCC_APB1ENR   |=(PerId);  break;
	        case RCC_APB2   : RCC_APB2ENR   |=(PerId);  break;
	        case RCC_LPAHB1 : RCC_AHB1LPENR |=(PerId);  break;
	        case RCC_LPAHB2 : RCC_AHB2LPENR |=(PerId);  break;
	        case RCC_LPAPB1 : RCC_APB1LPENR |=(PerId);  break;
	        case RCC_LPAPB2 : RCC_APB2LPENR |=(PerId);  break;
	        default: /* invalid input, return error */
	        	Local_tenuErrorStatus = LBTY_NOK;
	    }

	}
	else
	{
		/* invalid input, return error */
		Local_tenuErrorStatus = LBTY_NOK;
	}


	return Local_tenuErrorStatus;
}
/**************************************Disable peripheral clk**********************************************/
tenu_ErrorStatus MRCC_DisablePeripheralClock(u8 BusId,u8 PerId)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	if(PerId< RCC_LPAHB2)
	{
	    switch(BusId)
	    {
        case RCC_AHB1   : RCC_AHB1ENR   &=(~(PerId));  break;
        case RCC_AHB2   : RCC_AHB2ENR   &=(~(PerId));  break;
        case RCC_APB1   : RCC_APB1ENR   &=(~(PerId));  break;
        case RCC_APB2   : RCC_APB2ENR   &=(~(PerId));  break;
        case RCC_LPAHB1 : RCC_AHB1LPENR &=(~(PerId));  break;
        case RCC_LPAHB2 : RCC_AHB2LPENR &=(~(PerId));  break;
        case RCC_LPAPB1 : RCC_APB1LPENR &=(~(PerId));  break;
        case RCC_LPAPB2 : RCC_APB2LPENR &=(~(PerId));  break;
	        default: /* invalid input, return error */
	        	Local_tenuErrorStatus = LBTY_NOK;
	    }

	}
	else
	{
		/* invalid input, return error */
		Local_tenuErrorStatus = LBTY_NOK;
	}

	return Local_tenuErrorStatus;

}
/*************************************Control AHP1 peripheral clk(Enable/Disable) ************************/
tenu_ErrorStatus MRCC_ControlClockAHP1Peripheral(u32 AHB1Peripheral,u8 Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32Temp=RCC_AHB1ENR;  /* Assign register to local value */
	if(AHB1Peripheral<=RCC_AHB1_DMA2) /*Check if Input Peripheral is valid or not */
	{
		switch(Status)
		{
		case RCC_ENABLE:
			//Local_u32Temp &=~(AHB1Peripheral);
			Local_u32Temp |=AHB1Peripheral; /* Enable peripheral */
			RCC_AHB1ENR=Local_u32Temp;    /* update value in register */
			break;
		case RCC_DISABLE:
			Local_u32Temp &=~(AHB1Peripheral);
			RCC_AHB1ENR=Local_u32Temp;    /* update value in register */
			break;
		default:
			Local_tenuErrorStatus = LBTY_OK; /* Input Status is not valid */
		}
	}
	else
	{
		Local_tenuErrorStatus = LBTY_NOK; /* Input Peripheral is not valid */
	}

	return Local_tenuErrorStatus;
}
/*************************************Control AHP2 peripheral clk(Enable/Disable) ************************/
tenu_ErrorStatus MRCC_ControlClockAHP2Peripheral(u32 AHB2Peripheral,u8 Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32Temp=RCC_AHB2ENR;  /* Assign register to local value */
		if(AHB2Peripheral==RCC_AHB2_OTGFS) /*Check if Input Peripheral is valid or not */
		{
			switch(Status)
			{
			case RCC_ENABLE:
				//Local_u32Temp &=~(AHB2Peripheral);
				Local_u32Temp |=AHB2Peripheral; /* Enable peripheral */
				RCC_AHB2ENR=Local_u32Temp;    /* update value in register */
				break;
			case RCC_DISABLE:
				Local_u32Temp &=~(AHB2Peripheral);
				RCC_AHB2ENR=Local_u32Temp;    /* update value in register */
				break;
			default:
				Local_tenuErrorStatus = LBTY_OK; /* Input Status is not valid */
			}
		}
		else
		{
			Local_tenuErrorStatus = LBTY_OK; /* Input Peripheral is not valid */
		}

	return Local_tenuErrorStatus;
}
/*************************************Control ABP1 peripheral clk(Enable/Disable) ************************/
tenu_ErrorStatus MRCC_ControlClockABP1Peripheral(u32 APB1Peripheral,u8 Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32Temp=RCC_APB1ENR;  /* Assign register to local value */
		if(APB1Peripheral<=RCC_APB1_PWR) /*Check if Input Peripheral is valid or not */
		{
			switch(Status)
			{
			case RCC_ENABLE:
				//Local_u32Temp &=~(APB1Peripheral);
				Local_u32Temp |=APB1Peripheral; /* Enable peripheral */
				RCC_APB1ENR=Local_u32Temp;    /* update value in register */
				break;
			case RCC_DISABLE:
				Local_u32Temp &=~(APB1Peripheral);
				RCC_APB1ENR=Local_u32Temp;    /* update value in register */
				break;
			default:
				Local_tenuErrorStatus = LBTY_OK; /* Input Status is not valid */
			}
		}
		else
		{
			Local_tenuErrorStatus = LBTY_OK; /* Input Peripheral is not valid */
		}

	return Local_tenuErrorStatus;

}
/*************************************Control ABP2 peripheral clk(Enable/Disable) ************************/
tenu_ErrorStatus MRCC_ControlClockABP2Peripheral(u32 APB2Peripheral,u8 Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32Temp=RCC_AHB1ENR;  /* Assign register to local value */
		if(APB2Peripheral<=RCC_AHB1_DMA2) /*Check if Input Peripheral is valid or not */
		{
			switch(Status)
			{
			case RCC_ENABLE:
				//Local_u32Temp &=~(APB2Peripheral);
				Local_u32Temp |=APB2Peripheral; /* Enable peripheral */
				RCC_APB2ENR=Local_u32Temp;    /* update value in register */
				break;
			case RCC_DISABLE:
				Local_u32Temp &=~(APB2Peripheral);
				RCC_APB2ENR=Local_u32Temp;    /* update value in register */
				break;
			default:
				Local_tenuErrorStatus = LBTY_OK; /* Input Status is not valid */
			}
		}
		else
		{
			Local_tenuErrorStatus = LBTY_OK; /* Input Peripheral is not valid */
		}

	return Local_tenuErrorStatus;

}
/*************************************Control Clock(Enable/Disable) *************************************/
tenu_ErrorStatus MRCC_ControlClock(u8 SystemName,u8 Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32Var = RCC_CR;                       /* Assign register to local value */
	switch(SystemName)
	{
	case RCC_CLOCK_HSI:
		switch(Status)
		{
		case RCC_ENABLE :
	    	Local_u32Var &=~(RCC_HSI_ENABLE_MASK); /* Clearing specific bit */
	    	Local_u32Var |= RCC_HSI_ENABLE_MASK;   /* Enable peripheral */
	    	RCC_CR=Local_u32Var;                   /* update value in register */
			break;
		case RCC_DISABLE:
	    	Local_u32Var &=~(RCC_HSI_ENABLE_MASK);  /* Clearing specific bit */
	    	RCC_CR=Local_u32Var;                   /* update value in register */
            break;
		default         : Local_tenuErrorStatus = LBTY_NOK;

		}
		break;
   case RCC_CLOCK_HSE_CRUSTAL:
	    switch(Status)
	    {
	    case RCC_ENABLE : RCC_CR |=(1<<RCC_HSE_ENABLE_MASK);	         break;
	    case RCC_DISABLE: RCC_CR &=(~(1<<RCC_HSE_ENABLE_MASK));	         break;
	    default         : Local_tenuErrorStatus = LBTY_NOK;

	    }
        break;
   case RCC_CLOCK_HSE_RC:
	    switch(Status)
	    {
	    case RCC_ENABLE :
	    	Local_u32Var &=~(RCC_HSE_RC_ENABLE_MASK); /* Clearing specific bit */
	    	Local_u32Var |= RCC_HSE_RC_ENABLE_MASK;   /* Enable peripheral */
	    	RCC_CR=Local_u32Var;                      /* update value in register */

	    	break;
	    case RCC_DISABLE:
	    	Local_u32Var &=~(RCC_HSE_RC_ENABLE_MASK); /* Clearing specific bit */
	    	RCC_CR=Local_u32Var;                      /* update value in register */
	    	break;
	    default         : Local_tenuErrorStatus = LBTY_NOK;

	    }
	    break;
	case RCC_CLOCK_PLL:
	 	 switch(Status)
	 	  {
	 	  case RCC_ENABLE :	 RCC_CR |=(1<<RCC_PLL_ENABLE_MASK);       break;
	 	  case RCC_DISABLE:	 RCC_CR &=(~(1<<RCC_PLL_ENABLE_MASK));    break;
	 	  default         :  Local_tenuErrorStatus = LBTY_NOK;

	 	   }
	 	   break;
	default : Local_tenuErrorStatus = LBTY_NOK;
	}
	return Local_tenuErrorStatus;

}

/**************************************CHECK HSI STATUS*************************************************/
tenu_ErrorStatus MRCC_CheckHSIStatus(u8 *Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	s32 Local_u32TimeOut=TIME_OUT;
	while((((RCC_CR>>HSI_RDY)&ONE)==ZERO)|| (Local_u32TimeOut>ZERO))
	{
		Local_u32TimeOut--;
	}
	if((TIME_OUT==ZERO)&&((RCC_CR>>HSI_RDY)&1)==ZERO)
		{
		Local_tenuErrorStatus = LBTY_NOK;
		*Status=RCC_CLK_NOT_READY;

		}
	else
	{
		Local_tenuErrorStatus = LBTY_OK;
		*Status=RCC_CLK_READY;

	}
	return Local_tenuErrorStatus;

}
/**************************************CHECK HSE STATUS*************************************************/
tenu_ErrorStatus MRCC_CheckHSEStatus(u8 *Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	s32 Local_u32TimeOut=TIME_OUT;
	while((((RCC_CR>>HSE_RDY)&1)==ZERO)|| (Local_u32TimeOut>ZERO))
	{
		Local_u32TimeOut--;
	}
	if((TIME_OUT==ZERO)&&((RCC_CR>>PLL_RDY)&1)==ZERO)
		{
		Local_tenuErrorStatus = LBTY_NOK;
		*Status=RCC_CLK_NOT_READY;

		}
	else
	{
		Local_tenuErrorStatus = LBTY_OK;
		*Status=RCC_CLK_READY;

	}


	return Local_tenuErrorStatus;

}
/**************************************CHECK PLL STATUS*************************************************/
tenu_ErrorStatus MRCC_CheckPLLStatus(u8 *Status)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	s32 Local_u32TimeOut=TIME_OUT;
	while((((RCC_CR>>PLL_RDY)&ONE)==ZERO)|| (Local_u32TimeOut>ZERO))
	{
		Local_u32TimeOut--;
	}
	if((TIME_OUT==ZERO)&&((RCC_CR>>PLL_RDY)&1)==ZERO)
		{
		Local_tenuErrorStatus = LBTY_NOK;
		*Status=RCC_CLK_NOT_READY;

		}
	else
	{
		Local_tenuErrorStatus = LBTY_OK;
		*Status=RCC_CLK_READY;

	}


	return Local_tenuErrorStatus;

}

/******************************Function to Select System Clock******************************************/
tenu_ErrorStatus MRCC_SelectSystemClock(u8 SystemClock)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32Var = RCC_CFGR;                       /* Assign register to local value */
	u8 Local_u8ClockStatus =RCC_CLK_NOT_READY;
	switch(SystemClock)
	{
	case RCC_SYSTEM_CLOCK_HSI:
		Local_tenuErrorStatus=MRCC_CheckHSIStatus(&Local_u8ClockStatus);
		if(Local_u8ClockStatus==RCC_CLK_READY)
		{
			Local_u32Var &=(RCC_SYSTEM_CLOCK_MASK);
			Local_u32Var |=(RCC_HSI_SYSTEM_CLOCK_MASK);
			RCC_CFGR=Local_u32Var;
		}
		else
		{
			Local_tenuErrorStatus = LBTY_NOK;
		}
		break;
	case RCC_SYSTEM_CLOCK_HSE:
		Local_tenuErrorStatus=MRCC_CheckHSEStatus(&Local_u8ClockStatus);
		if(Local_u8ClockStatus==RCC_CLK_READY)
		{
			Local_u32Var &=(RCC_HSE_SYSTEM_CLOCK_MASK);
			Local_u32Var |=(RCC_HSE_SYSTEM_CLOCK_MASK);
			RCC_CFGR=Local_u32Var;
		}
		else
		{
			Local_tenuErrorStatus = LBTY_NOK;
		}
		break;
	case RCC_SYSTEM_CLOCK_PLL:
		Local_tenuErrorStatus=MRCC_CheckPLLStatus(&Local_u8ClockStatus);
		if(Local_u8ClockStatus==RCC_CLK_READY)
		{
			Local_u32Var &=(RCC_PLL_SYSTEM_CLOCK_MASK);
			Local_u32Var |=(RCC_PLL_SYSTEM_CLOCK_MASK);
			RCC_CFGR=Local_u32Var;
		}
		else
		{
			Local_tenuErrorStatus = LBTY_NOK;
		}
		break;
	default :
		Local_tenuErrorStatus = LBTY_NOK;

	}
	return Local_tenuErrorStatus;

}

/******************************Function to Get System Clock*********************************************/
tenu_ErrorStatus MRCC_GetSystemClock(u8 *SystemClock)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32SystemClock = RCC_CFGR;
	Local_u32SystemClock &=RCC_SYSTEM_CLK_MASK;
	Local_u32SystemClock = Local_u32SystemClock >> RCC_SWS_MASK;
	switch(Local_u32SystemClock)
	{
	case RCC_SYSTEM_CLOCK_HSI: *SystemClock=RCC_SYSTEM_CLOCK_HSI; break;
	case RCC_SYSTEM_CLOCK_HSE: *SystemClock=RCC_SYSTEM_CLOCK_HSE; break;
	case RCC_SYSTEM_CLOCK_PLL: *SystemClock=RCC_SYSTEM_CLOCK_PLL; break;
	default :     Local_tenuErrorStatus = LBTY_NOK;

	}
	return Local_tenuErrorStatus;

}
/******************************Function to PLL INPUT Clock**********************************************/
tenu_ErrorStatus MRCC_GetPLLInputClock(u8 *PLLInputClock)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_u32SystemClock = ((RCC_PLLCFGR>>RCC_PLL_SOURCE_MASK)&0X01);
	switch(Local_u32SystemClock)
	{
	case RCC_HSI_PLL_SOURCE: *PLLInputClock=RCC_HSI_PLL_SOURCE; break;
	case RCC_HSE_PLL_SOURCE: *PLLInputClock=RCC_HSE_PLL_SOURCE; break;
	default :     Local_tenuErrorStatus = LBTY_NOK;
	}
	return Local_tenuErrorStatus;
}
/******************************Function to Select PLL INPUT Clock****************************************/
tenu_ErrorStatus MRCC_SelectPLLInputClock(u8 PLLInputClock)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u8 Local_StausHSI=RCC_CLK_NOT_READY;
	u8 Local_StausHSE=RCC_CLK_NOT_READY;

	if(PLLInputClock>RCC_PLL_INPUT_HSE)
	{
		Local_tenuErrorStatus = LBTY_NOK;
	}
	else
	{
		switch(PLLInputClock)
		{
		case RCC_PLL_INPUT_HSI:
			Local_tenuErrorStatus=MRCC_CheckHSIStatus(&Local_StausHSI);
			if(Local_StausHSI==RCC_CLK_NOT_READY)
			{
				Local_tenuErrorStatus = LBTY_NOK;
			}
			else
			{
				RCC_PLLCFGR &=(~(1<<RCC_PLL_SOURCE));
			}
			break;
		case RCC_PLL_INPUT_HSE:
			Local_tenuErrorStatus=MRCC_CheckHSEStatus(&Local_StausHSE);
			if(Local_StausHSE==RCC_CLK_NOT_READY)
			{
				Local_tenuErrorStatus = LBTY_NOK;
			}
			else
			{
				RCC_PLLCFGR |=(1<<RCC_PLL_SOURCE);
			}
			break;
		default: Local_tenuErrorStatus = LBTY_NOK;
		}

	}


	return Local_tenuErrorStatus;
}
/******************************Function to Select P factor for PLL****************************************/
tenu_ErrorStatus MRCC_SelectPLLPFactor(u32 PLLPFactor)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_temp=RCC_PLLCFGR;
	u8 Local_SystemClock=ZERO;
	MRCC_GetSystemClock(&Local_SystemClock);
	if(PLLPFactor>RCC_PLLP_8||Local_SystemClock==RCC_SYSTEM_CLOCK_PLL)
	{
		Local_tenuErrorStatus = LBTY_NOK;
	}
	else
	{
		Local_temp &=(RCC_PLLP_MASK);
		Local_temp |=PLLPFactor;
		RCC_PLLCFGR=Local_temp;
	}

	return Local_tenuErrorStatus;

}
/******************************Function to Select M factor for PLL****************************************/
tenu_ErrorStatus MRCC_SelectPLLMFactor(u32 PLLMFactor)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_temp=RCC_PLLCFGR;
	u8 Local_SystemClock=ZERO;
	MRCC_GetSystemClock(&Local_SystemClock);
	if(PLLMFactor>RCC_PLLM_MAX||PLLMFactor<RCC_PLLM_MIN||Local_SystemClock==RCC_SYSTEM_CLOCK_PLL)
	{
		Local_tenuErrorStatus = LBTY_NOK;
	}
	else
	{
		Local_temp &=RCC_PLLM_MASK;
		Local_temp |=PLLMFactor;
		RCC_PLLCFGR=Local_temp;
	}
	return Local_tenuErrorStatus;

}

/******************************Function to Select N factor for PLL****************************************/
tenu_ErrorStatus MRCC_SelectPLLNFactor(u32 PLLNFactor)
{
	tenu_ErrorStatus Local_tenuErrorStatus = LBTY_OK;
	u32 Local_temp=RCC_PLLCFGR;
	u8 Local_SystemClock=ZERO;
	MRCC_GetSystemClock(&Local_SystemClock);
	if(PLLNFactor>RCC_PLLN_MAX||PLLNFactor<RCC_PLLN_MIN||Local_SystemClock==RCC_SYSTEM_CLOCK_PLL)
	{
		Local_tenuErrorStatus = LBTY_NOK;
	}
	else
	{
		Local_temp &=(RCC_PLLN_MASK);
		Local_temp |=PLLNFactor;
		RCC_PLLCFGR=Local_temp;
	}


	return Local_tenuErrorStatus;

}

















