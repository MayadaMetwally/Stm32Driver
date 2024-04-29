/**************************************************************************************************************/
/* File Name:	Systick.c                                                                                     */
/* Module	: 	Systick						                                                                  */
/* Layer	:	MCAL										                                                  */
/* AUTHOR   :   Lina Nassar                                                                                   */
/**************************************************************************************************************/

#include "STK/Systick.h"
#include "STK/Systick_Config.h"

/*Definitions*/
#define STK_Add    0xE000E010

#define STK_ExtRefClk_Freq    (STK_ProcessorClk_Freq/8)
#define STK_MaxReloadVal      0xFFFFFF


#define STK_Systick_Disable   0x0  
#define STK_Counter_Enable    Reg_Mask_bit0
#define STK_SrcClk_Mask       Reg_Mask_bit2
#define STK_Int_Enable        Reg_Mask_bit1
#define STK_CountFlag         Reg_Mask_bit16
#define STK_Flag_NotSet       0 
#define STK_Val_Reset         0x0


/*Type Definitions*/
typedef struct{
    uint32_t CTRL;
    uint32_t LOAD;
    uint32_t VAL;
    uint32_t CALIB;

}Systick_Reg_t;


/*Global Variables*/
static volatile Systick_Reg_t * const Systick = (volatile Systick_Reg_t * const)(STK_Add);
static uint8_t OneTime_Flag=0;
static systickcbf_t Systickcbf=NULL;

static STK_errors_t SysTick_GetCounts_Static(uint32_t TimeMs, uint32_t *Reload_Value)
{
    STK_errors_t Ret_GetCounts = STK_enuOk;
    volatile uint32_t Loc_Freq=0;

    if ((Systick->CTRL & STK_SrcClk_Mask) == STK_SrcClk_Processor)
    {
        Loc_Freq = STK_ProcessorClk_Freq;
    }
    else 
    {
        Loc_Freq = STK_ExtRefClk_Freq;
    }

    *Reload_Value = ((TimeMs * Loc_Freq) /1000) -1;

    return Ret_GetCounts;
}

static void SysTick_Stop_Static(void)
{
    Systick->CTRL = STK_Systick_Disable;
}

STK_errors_t SysTick_ConfigureClk(uint32_t STK_SrcClk)
{
    STK_errors_t Ret_SetTimeMs = STK_enuOk;
    uint32_t Loc_RegisterTempVal;

    if(!((STK_SrcClk == STK_SrcClk_Processor) || (STK_SrcClk == STK_SrcClk_ExtRef)))
    {
        Ret_SetTimeMs = STK_enuWrongParameter;         
    }

    if(Ret_SetTimeMs == STK_enuOk)
    {
        Loc_RegisterTempVal = Systick->CTRL ;
        Loc_RegisterTempVal &= (~STK_SrcClk_Mask);
        Loc_RegisterTempVal |= STK_SrcClk;
        Systick->CTRL = Loc_RegisterTempVal;
    }

    return Ret_SetTimeMs;

}

STK_errors_t SysTick_SetTimeMs(uint32_t TimeMs)
{
    STK_errors_t Ret_SetTimeMs = STK_enuOk;
    uint32_t Reload_Value;

    Ret_SetTimeMs = SysTick_GetCounts_Static(TimeMs, &Reload_Value);

    if(Reload_Value> STK_MaxReloadVal)
    {
        Ret_SetTimeMs = STK_enuCountOverFlow;
    }
    else if(Ret_SetTimeMs == STK_enuOk)
    {
        SysTick_Stop_Static();
        Systick->VAL = STK_Val_Reset;           // Clear current value as well as count flag
        Systick->LOAD = Reload_Value;           // Count from Reload_Value to 0 
    }
    return Ret_SetTimeMs;
}    

STK_errors_t SysTick_Start(uint32_t STK_Mode)
{
    STK_errors_t Ret_Start = STK_enuOk;
    
    if(!((STK_Mode == STK_Mode_Periodic) || (STK_Mode == STK_Mode_OneTime)))
    {
        Ret_Start = STK_enuWrongParameter;
    }
    else if(Ret_Start == STK_enuOk)
    {
        Systick->CTRL |= STK_Int_Enable;
        Systick->CTRL |= STK_Counter_Enable;
    }

    if(STK_Mode == STK_Mode_OneTime)
    {
        OneTime_Flag = 1;
    }

    return Ret_Start;
}

void SysTick_Stop(void)
{
    SysTick_Stop_Static();
}

void SysTick_IsExpired(void)
{
    Systick->CTRL |= STK_Counter_Enable;
    while ( (Systick->CTRL & STK_CountFlag) == STK_Flag_NotSet); // Wait until count flag is set
    SysTick_Stop_Static();
}

STK_errors_t SysTick_SetCallBack(systickcbf_t cbf)
{
    STK_errors_t Ret_SetCb = STK_enuOk;

    if(cbf == NULL)
    {
        Ret_SetCb = STK_enuNullPtr;
    }

    if(Ret_SetCb == STK_enuOk)
    {
        Systickcbf = cbf;
    }

    return Ret_SetCb;
}

void SysTick_Handler(void)
{
    if(Systickcbf)
    {
       Systickcbf();

       if(OneTime_Flag)
       {
         SysTick_Stop_Static();
         OneTime_Flag=0;
       }
    }

}