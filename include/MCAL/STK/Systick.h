/**************************************************************************************************************/
/* File Name:	Systick.h                                                                                     */
/* Module	: 	Systick						                                                                  */
/* Layer	:	MCAL										                                                  */
/* AUTHOR   :   Lina Nassar                                                                                   */
/* Date     :   23/3/2024                                                                                     */
/**************************************************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

#include "STD_Types.h"
#include "Masks.h"
#include "Systick_Config.h"

/*Systick Source CLock*/
#define STK_SrcClk_Processor  Reg_Mask_bit2
#define STK_SrcClk_ExtRef     0x0

/*Systick Start Mode*/
#define STK_Mode_Periodic   0x01
#define STK_Mode_OneTime    0x00

typedef enum
{
    STK_enuOk,
    STK_enuNOk,
    STK_enuNullPtr,
    STK_enuCountOverFlow,
    STK_enuWrongParameter,
}STK_errors_t;
#endif


typedef void (*systickcbf_t) (void);  

/**
 *@brief  : Configures the Systick Source clock to be either Processor of External Reference Clock (AHB/8)
 *@param  : (STK_SrcClk) -> STK_SrcClk_Processor: for processor clock 
 *        : (STK_SrcClk) -> STK_SrcClk_ExtRef
 *@return : Systick Error Status (STK_errors_t)             
 */
STK_errors_t SysTick_ConfigureClk(uint32_t STK_SrcClk);  

/**
 *@brief  : Sets the Systick counter to the required time interval
 *@param  : (TimeMs) -> Time in milliseconds 
 *@return : Systick Error Status (STK_errors_t)             
 */
STK_errors_t SysTick_SetTimeMs(uint32_t TimeMs);   

/**
 *@brief  : Starts the Systick Timer countdown & enables the Systick IRQ, for a single time or continuously
 *@param  : (STK_Mode) -> STK_Mode_Periodic: Enables the Systick timer to run and fire an IRQ periodically,
 *          with the period that was set by SysTick_SetTimeMs function previously.  
 *        : (STK_Mode) -> STK_Mode_OneTime: Enables the Systick timer to run and fire an IRQ for a single
 *          period that was set by SysTick_SetTimeMs function previously, then the Systick counter and IRq
 *          are disabled. 
 *@return : RCC Error Status               
 */
STK_errors_t SysTick_Start(uint32_t STK_Mode);

/**
 *@brief  : Disables the SysTickr and IRQ
 *@param  : Void
 *@return : Void          
 */
void SysTick_Stop(void);

/**
 *@brief  : To use the SysTick timer in polling mode. The polling period should be previously set by 
 *          SysTick_SetTimeMs function, then automatically the SysTick will be disabled.
 *@param  : Void
 *@return : Void           
 */
void SysTick_IsExpired(void);

/**
 *@brief  : Assigns the pointer of the function to be executed when the Exception request of the
 *          Systick is fired.
 *@param  : (cbf) -> Function address
 *@return : Void           
 */
STK_errors_t SysTick_SetCallBack(systickcbf_t cbf);


