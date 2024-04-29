/***************************************************************************************************************************/
/* File Name:	RCC.c                                                                                                      */
/* Module	: 	RCC						                                                                                   */
/* Layer	:	MCAL										                                                               */
/* AUTHOR   :   Lina Nassar                                                                                                */
/* Date     :   10/2/2024                                                                                                  */
/***************************************************************************************************************************/


#include "MRCC/RCC.h"

/*Rcc base address*/
#define RCC_Add              0x40023800

/*System Clock*/
#define SysClock_Mask        0x00000003

/*PLL Source Clock*/
#define PLLSrc_Mask          Reg_Mask_bit22

/*Clock Read bit masks*/
#define ClockRdy_HSI_Mask    Reg_Mask_bit1
#define ClockRdy_HSE_Mask    Reg_Mask_bit17
#define ClockRdy_PLL_Mask    Reg_Mask_bit25

/*PLL Parameters Shift Bits*/
#define PLLM_ShiftBits       Reg_bit0
#define PLLN_ShiftBits       Reg_bit6
#define PLLP_ShiftBits       Reg_bit16
#define PLLQ_ShiftBits       Reg_bit24

/*PLL Parameters Masks*/
#define PLLM_Mask           0x0000001F
#define PLLN_Mask           0x000003E0
#define PLLP_Mask           0x00030000
#define PLLQ_Mask           0x0F000000


/*PLL Parameter Boundaries*/
#define PLL_MMin        2
#define PLL_MMax        63
#define PLL_NMin        192
#define PLL_NMax        432
#define PLL_PMin        0
#define PLL_PMax        3
#define PLL_QMin        2
#define PLL_QMax        15

/*Clock Frequencies*/
#define HSI_Freq        16   
#define HSE_Freq        25




/*Peripherals Prescalers Mask*/
#define PreSc_AHB_Mask      0xF0  
#define PreSc_APB1_Mask     0x1C00
#define PreSc_APB2_Mask     0xE000
#define PreSc_RTC_Mask      0x1F0000
#define PreSc_MCO1_Mask     0x7000000
#define PreSc_MCO2_Mask     0x38000000

/*Prescalers Parameters Shift Bits*/
#define PeriPresc_AHB_Mask     Reg_bit4   
#define PeriPresc_APB1_Mask    Reg_bit10
#define PeriPresc_APB2_Mask    Reg_bit13
#define PeriPresc_RTC_Mask     Reg_bit16 
#define PeriPresc_MCO1_Mask    Reg_bit24 
#define PeriPresc_MCO2_Mask    Reg_bit27



typedef struct
{
    uint32_t CR;   
    uint32_t PLLCFGR;
    uint32_t CFGR;    
    uint32_t CIR;   
    uint32_t AHB1RSTR; 
    uint32_t AHB2RSTR; 
    uint32_t reserved0;    
    uint32_t reserved1;
    uint32_t APB1RSTR; 
    uint32_t APB2RSTR; 
    uint32_t reserved2; 
    uint32_t reserved3; 
    uint32_t AHB1ENR;    
    uint32_t AHB2ENR;
    uint32_t reserved4;
    uint32_t reserved5;
    uint32_t APB1ENR; 
    uint32_t APB2ENR; 
    uint32_t reserved6;
    uint32_t reserved7;    
    uint32_t AHB1LPENR;    
    uint32_t AHB2LPENR;
    uint32_t reserved8;
    uint32_t reserved9;
    uint32_t APB1LPENR;
    uint32_t APB2LPENR;
    uint32_t reserved10;
    uint32_t reserved11;
    uint32_t BDCR;
    uint32_t CSR;
    uint32_t reserved12;
    uint32_t reserved13;
    uint32_t SSCGR;
    uint32_t PLLI2SCFGR;
    uint32_t reserved14;
    uint32_t DCKCFGR;
}RCC_Reg_t;
 

/*Global Variables*/
static RCC_Reg_t * const RCC = (volatile RCC_Reg_t * const)(RCC_Add);


/*Static Functions Prototypes*/
//static void RCC_GetSysClock(uint32_t * SysClock);


RCC_errors_t RCC_ControlClock(uint32_t Clock, uint8_t CLockStatus)
{
    //uint8_t  Loc_SysClockVal, Loc_AllowDisableFlag=1;
    uint32_t Loc_RegisterTempVal;
    RCC_errors_t Ret_ControlClock = RCC_enuOk;
    
    if ((Clock == Clock_HSI) || (Clock == Clock_HSE) || (Clock == Clock_PLL) || (Clock == Clock_PLLI2S) || (Clock == Clock_RTC))
    {
        switch (CLockStatus)
        {
        case ClockStatus_Enable:
            if (Clock == Clock_RTC){
                Loc_RegisterTempVal= RCC->BDCR;     /*Make a copy of Reg value*/
                Loc_RegisterTempVal &= (~Clock);    /*Clear Bit*/
                Loc_RegisterTempVal |= (Clock);     /*Set bit*/
                RCC->BDCR= Loc_RegisterTempVal;     /*Apply value to Reg*/
            }
            else{
                Loc_RegisterTempVal= RCC->CR;
                Loc_RegisterTempVal &= (~Clock);
                Loc_RegisterTempVal |= (Clock);
                RCC->CR= Loc_RegisterTempVal;
            }
            break;

        case ClockStatus_Disable:
            if (Clock == Clock_RTC)
            {
                Loc_RegisterTempVal= RCC->BDCR;
                Loc_RegisterTempVal &= (~Clock);
                RCC->BDCR= Loc_RegisterTempVal;
            }
            else
            {
                Loc_RegisterTempVal= RCC->CR;
                Loc_RegisterTempVal &= (~Clock);
                RCC->CR= Loc_RegisterTempVal;
            }
            break;

        default:
            Ret_ControlClock= RCC_enuWrongStatus;
            break;
        }
    }
    else
    {
        Ret_ControlClock = RCC_enuClockNotFound;
    }
    
    return Ret_ControlClock;
}

RCC_errors_t RCC_ConfigurePLL(uint32_t PLLSrc, uint32_t PLLM, uint32_t PLLN, uint16_t PLLP, uint32_t PLLQ )
{
    RCC_errors_t Ret_CfgPLL= RCC_enuOk;
    uint32_t Loc_RegTempValue;
    uint8_t Loc_PLLSrcFreq, f_VCO_IP, f_VCO_OP, f_PLL_Gen, f_USB;
    

    /*Check if PLL is on*/
    if (RCC->CR & Clock_PLL)
    {
        Ret_CfgPLL= RCC_enuPLLisEnabled;
    }
    else
    {
        /*Check Input Parameters*/
        if(!(PLLSrc == PLLSrc_HSE || PLLSrc == PLLSrc_HSI))
        {
            Ret_CfgPLL = RCC_enuParameterOutofRange;
        }
        else if(!((PLLM >= PLL_MMin) && (PLLM <= PLL_MMax)))
        {
            Ret_CfgPLL= RCC_enuParameterOutofRange;
        }
        else if(!((PLLN >= PLL_NMin) && (PLLN <= PLL_NMax)))
        {
            Ret_CfgPLL= RCC_enuParameterOutofRange;
        }
        else if(!((PLLP >= PLL_PMin) && (PLLP <= PLL_PMax)))
        {
            Ret_CfgPLL= RCC_enuParameterOutofRange;
        }
        else if(!((PLLQ >= PLL_QMin) && (PLLQ <= PLL_QMax)))
        {
            Ret_CfgPLL= RCC_enuParameterOutofRange;
        }
        else
        {
            switch (PLLSrc)
            {
            case PLLSrc_HSE:
                Loc_PLLSrcFreq = HSE_Freq;
                break;

            case PLLSrc_HSI:
                Loc_PLLSrcFreq = HSI_Freq;
                break;

            default:
                break;
            }         

            /*Calcualte Frequencies*/
            f_VCO_IP = Loc_PLLSrcFreq/PLLM;
            f_VCO_OP = f_VCO_IP * PLLN;
            f_PLL_Gen = f_VCO_OP / PLLP;
            f_USB = f_VCO_OP / PLLQ;

            /*Check Frequency Ranges*/
            /* f(VCO I/P) */
            if( (f_VCO_IP<1) || (f_VCO_IP>2) )
            {
                Ret_CfgPLL= RCC_enuFreqOutofRange;
            }
            /* f(VCO O/P) */
            else if( (f_VCO_OP<192) || (f_VCO_OP>432) )
            {
                Ret_CfgPLL= RCC_enuFreqOutofRange;
            }
            /*  f(PLL general clock output) */
            else if(f_PLL_Gen>84)
            {
                Ret_CfgPLL= RCC_enuFreqOutofRange;
            }
            /*  f(USB OTG FS, SDIO, RNG clock output) */
            else if(f_USB!=48)
            {
                Ret_CfgPLL= RCC_enuFreqOutofRange;
            }
        }
    }

    if (Ret_CfgPLL== RCC_enuOk) /*Configure PLL*/
    {
        /*Set PLL Source*/
        Loc_RegTempValue= RCC->PLLCFGR;
        Loc_RegTempValue &= (~PLLSrc_Mask);
        Loc_RegTempValue |= PLLSrc;
        RCC->CFGR= Loc_RegTempValue;

        /*Set PLLM, PLLN, PLLP, PLLQ*/
        Loc_RegTempValue= RCC->PLLCFGR;
        Loc_RegTempValue &= (~PLLM_Mask);
        Loc_RegTempValue &= (~PLLN_Mask);
        Loc_RegTempValue &= (~PLLP_Mask);
        Loc_RegTempValue &= (~PLLQ_Mask);
        Loc_RegTempValue |= ((PLLM << PLLM_ShiftBits) | (PLLN << PLLN_ShiftBits) | (PLLQ << PLLQ_ShiftBits) | (PLLP << PLLP_ShiftBits));

    }
    return Ret_CfgPLL; 
}  

RCC_errors_t RCC_ConfigureSysClock(uint32_t SysClock)
{
    RCC_errors_t Ret_CfgSysClk= RCC_enuOk;
    uint32_t Loc_RegTempValue, Loc_AllowSysClkCfg=0;

    switch (SysClock)
    {
    case SysClock_HSI:
        Loc_AllowSysClkCfg= RCC->CR&ClockRdy_HSI_Mask;
        break;
    case SysClock_HSE:
        Loc_AllowSysClkCfg= RCC->CR&ClockRdy_HSE_Mask;
        break;
    case SysClock_PLL:
        Loc_AllowSysClkCfg= RCC->CR&ClockRdy_PLL_Mask;
        break;
    default:
        Ret_CfgSysClk= RCC_enuClockNotFound;
        break;
    }     
    if(Loc_AllowSysClkCfg)
    {
        Loc_RegTempValue= RCC->CFGR;
        Loc_RegTempValue &= (~SysClock_Mask);
        Loc_RegTempValue |= SysClock;
        RCC->CFGR= Loc_RegTempValue;
    }
    else
    {
        Ret_CfgSysClk= RCC_enuClockNotRdy;
    }

    return Ret_CfgSysClk;
}

RCC_errors_t RCC_ControlPeripheralClock(uint64_t Peri, uint32_t PeriStatus, uint32_t PowerMode )
{
    RCC_errors_t Ret_CtrlPeri= RCC_enuOk;
    uint8_t Loc_PerSwitch;
    uint32_t Loc_RegTempValue=0;

    /*Check Peripheral*/
    if(Peri < 0xFFFFFFFF)
    {
        Ret_CtrlPeri= RCC_enuPeriNotFound;
    }
    else if((PeriStatus<0) || (PeriStatus>1))
    {
        Ret_CtrlPeri= RCC_enuWrongStatus;
    }
    else if((PowerMode<0) || (PowerMode>1))
    {
        Ret_CtrlPeri= RCC_enuWrongPwrMod;
    }
    else
    {
        Loc_PerSwitch=(Peri >> 32);

        switch (PowerMode)
        {
        case PowerMode_Normal:
            switch (PeriStatus)
            {
            case PeriStatus_Enable:
                switch (Loc_PerSwitch)
                {
                case AHB1_Code:
                    Loc_RegTempValue=RCC->AHB1ENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->AHB1ENR = Loc_RegTempValue;
                    break;
                case AHB2_Code:
                    Loc_RegTempValue=RCC->AHB2ENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->AHB2ENR = Loc_RegTempValue;
                    break;
                case APB1_Code:
                    Loc_RegTempValue=RCC->APB1ENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->APB1ENR = Loc_RegTempValue;
                    break;
                case APB2_Code:
                    Loc_RegTempValue=RCC->APB2ENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->APB2ENR = Loc_RegTempValue;
                    break;
                default:
                    break;
                }
            break;

            case PeriStatus_Disable:
                
                switch (Loc_PerSwitch)
                {
                case AHB1_Code:
                    Loc_RegTempValue= RCC->AHB1ENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->AHB1ENR= Loc_RegTempValue;
                    break;
                case AHB2_Code:
                    Loc_RegTempValue= RCC->AHB2ENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->AHB2ENR= Loc_RegTempValue;
                    break;
                case APB1_Code:
                    Loc_RegTempValue=RCC->APB1ENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->APB1ENR= Loc_RegTempValue;
                    break;
                case APB2_Code:
                    Loc_RegTempValue=RCC->APB2ENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->APB2ENR= Loc_RegTempValue;
                    break;
                default:
                    break;
                }

            break;
            }

        break;
        
        case PowerMode_LowPwr:
            switch (PeriStatus)
            {
            case PeriStatus_Enable:
                switch (Loc_PerSwitch)
                {
                case AHB1_Code:
                    Loc_RegTempValue=RCC->AHB1LPENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->AHB1LPENR= Loc_RegTempValue;
                    break;
                case AHB2_Code:
                    Loc_RegTempValue=RCC->AHB2LPENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->AHB2LPENR= Loc_RegTempValue;
                    break;
                case APB1_Code:
                    Loc_RegTempValue=RCC->APB1LPENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->APB1LPENR= Loc_RegTempValue;
                    break;
                case APB2_Code:
                    Loc_RegTempValue=RCC->APB2LPENR;
                    Loc_RegTempValue |= ((uint32_t)Peri);
                    RCC->APB2LPENR= Loc_RegTempValue;
                    break;
                default:
                    break;
                }
            break;

            case PeriStatus_Disable:
                
                switch (Loc_PerSwitch)
                {
                case AHB1_Code:
                    Loc_RegTempValue=RCC->AHB1LPENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->AHB1LPENR= Loc_RegTempValue;
                    break;
                case AHB2_Code:
                    Loc_RegTempValue=RCC->AHB2LPENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->AHB2LPENR= Loc_RegTempValue;
                    break;
                case APB1_Code:
                    Loc_RegTempValue=RCC->APB1LPENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->APB1LPENR= Loc_RegTempValue;
                    break;
                case APB2_Code:
                    Loc_RegTempValue=RCC->APB2LPENR;
                    Loc_RegTempValue &= (~((uint32_t)Peri));
                    RCC->APB2LPENR= Loc_RegTempValue;
                    break;
                default:
                    break;
                }

            break;
            }        
        break;
        
        default:
            break;
        }    
    }

    return Ret_CtrlPeri;
}


RCC_errors_t RCC_ConfigurePrescaler(uint32_t PeriPreSc, uint32_t Prescaler)
{   
    RCC_errors_t Ret_CfgPreSc= RCC_enuOk;
    uint32_t Loc_RegTempValue;

    Loc_RegTempValue = RCC->CFGR;
    switch (PeriPreSc)
    {
    case PeriPresc_AHB:
        Loc_RegTempValue &= (~(PeriPresc_AHB_Mask));
        break;
    case PeriPresc_APB1:
        Loc_RegTempValue &= (~(PeriPresc_APB1_Mask));
        break;
    case PeriPresc_APB2:
        Loc_RegTempValue &= (~(PeriPresc_APB2_Mask));
        break;
    case PeriPresc_RTC:
        Loc_RegTempValue &= (~(PeriPresc_RTC_Mask));
        break;
    case PeriPresc_MCO1:
        Loc_RegTempValue &= (~(PeriPresc_MCO1_Mask));
        break;
    case PeriPresc_MCO2:
        Loc_RegTempValue &= (~(PeriPresc_MCO2_Mask));
        break;
    
    default:
        Ret_CfgPreSc = RCC_enuPeriNotFound;
        break;
    }

    Loc_RegTempValue |= (Prescaler << PeriPreSc );
    RCC->CFGR = Loc_RegTempValue;

    return Ret_CfgPreSc;
}


