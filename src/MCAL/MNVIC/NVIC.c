/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MNVIC/MNVIC.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/* Macros defining base addresses and an offset used in the code for NVIC and SCB configurations.*/
#define NVIC_BASE_ADDRESS  0xE000E100
#define SCB_BASE_ADDRESS   0xE000ED00
#define NVIC_OFFSET_4       4

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
/* Structures defining the register layout for NVIC and SCB, providing an organized representation of
the memory-mapped registers in the hardware.*/
typedef struct
{
    volatile u32 ISER[8];
    volatile u32 Reserved1[24];
    volatile u32 ICER[8];
    volatile u32 Reserved2[24];
    volatile u32 ISPR[8];
    volatile u32 Reserved3[24];
    volatile u32 ICPR[8];
    volatile u32 Reserved4[24];
    volatile u32 IABR[8];
    volatile u32 Reserved5[56];
    volatile u8 IPR[240];
    volatile u32 Reserved6[644];
    volatile u32 STIR;
} NVIC_t;

typedef struct
{
    volatile u32 CPUID;
    volatile u32 ICSR;
    volatile u32 VTOR;
    volatile u32 AIRCR;
    volatile u32 SCR;
    volatile u32 CCR;
    volatile u32 SHPR[3];
    volatile u32 SHCSR;
    volatile u32 CFSR;
    volatile u32 HFSR;
    volatile u32 Reserved2;
    volatile u32 MMAR;
    volatile u32 BFAR;
    volatile u32 AFSR;

} SCB_t;

/*Pointers to NVIC and SCB structures, mapped to their respective base addresses.*/
NVIC_t *const NVIC  = (NVIC_t *)NVIC_BASE_ADDRESS;
SCB_t  *const SCB   = (SCB_t *)SCB_BASE_ADDRESS;



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/
void MNVIC_Init()
{
    SCB->AIRCR=NVIC_GROUP_SUP_DIST;

}
tenu_ErrorStatus MNVIC_EnableInterrupt(u8 Copy_InterruptID){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 Local_RegNum=0;
    u8 Local_BitNum=0;
    if(Copy_InterruptID>=_NVIC_IRQ_NUM)
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }
    else
    {
        Local_RegNum = (Copy_InterruptID)/(32);
        Local_BitNum = (Copy_InterruptID)%(32);
        NVIC->ISER[Local_RegNum]=(1<<Local_BitNum);

    }

    return Local_ErrorStatus;
}

tenu_ErrorStatus MNVIC_DisableInterrupt(u8 Copy_InterruptID){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
        u8 Local_RegNum=0;
    u8 Local_BitNum=0;
    if(Copy_InterruptID>=_NVIC_IRQ_NUM)
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }
    else
    {
        Local_RegNum = (Copy_InterruptID)/(32);
        Local_BitNum = (Copy_InterruptID)%(32);
        NVIC->ICER[Local_RegNum]=(1<<Local_BitNum);

    }


    return Local_ErrorStatus;
}
tenu_ErrorStatus MNVIC_SetPanding(u8 Copy_InterruptID){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
        u8 Local_RegNum=0;
    u8 Local_BitNum=0;
    if(Copy_InterruptID>=_NVIC_IRQ_NUM)
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }
    else
    {
        Local_RegNum = (Copy_InterruptID)/(32);
        Local_BitNum = (Copy_InterruptID)%(32);
        NVIC->ISPR[Local_RegNum]=(1<<Local_BitNum);

    }


    return Local_ErrorStatus;
}
tenu_ErrorStatus MNVIC_Clearpanding(u8 Copy_InterruptID){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
        u8 Local_RegNum=0;
    u8 Local_BitNum=0;
    if(Copy_InterruptID>=_NVIC_IRQ_NUM)
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }
    else
    {
        Local_RegNum = (Copy_InterruptID)/(32);
        Local_BitNum = (Copy_InterruptID)%(32);
        NVIC->ICPR[Local_RegNum]=(1<<Local_BitNum);

    }


    return Local_ErrorStatus;
}
tenu_ErrorStatus MNVIC_GetActiveStatus(u8 Copy_InterruptID,u8 * Add_Status){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    u8 Local_RegNum=0;
    u8 Local_BitNum=0;
    if(Copy_InterruptID>=_NVIC_IRQ_NUM)
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }
    else if(Add_Status==NULL)
    {
        Local_ErrorStatus = LBTY_ErrorNullPointer;
    }
    else
    {
        Local_RegNum = (Copy_InterruptID)/(32);
        Local_BitNum = (Copy_InterruptID)%(32);
        *Add_Status=(NVIC->IABR[Local_RegNum]&(1<<Local_BitNum));

    }


    return Local_ErrorStatus;
}
tenu_ErrorStatus MNVIC_SetPriority(u8 Copy_InterruptID,u8 Copy_Priority){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
    if(Copy_InterruptID>=_NVIC_IRQ_NUM)
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }
    else
    {
        NVIC->IPR[Copy_InterruptID]=(Copy_Priority<<NVIC_OFFSET_4);
    }

    return Local_ErrorStatus;
}
tenu_ErrorStatus MNVIC_GenerateSWInterrupt(u8 Copy_InterruptID){
    tenu_ErrorStatus Local_ErrorStatus = LBTY_OK;
        if(Copy_InterruptID>=_NVIC_IRQ_NUM)
    {
        Local_ErrorStatus = LBTY_ErrorInvalidInput;
    }
    else
    {
        NVIC->STIR=Copy_InterruptID;
    }


    return Local_ErrorStatus;
}
