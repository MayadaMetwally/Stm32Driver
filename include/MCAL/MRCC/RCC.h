/************************************************************************************************************
 *************************            File Name: RCC.h                               ************************
 *************************            Layer	   : MCAL                                ************************
 *************************            Module   : RCC                                 ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/4/2023                            ************************
 *************************            Author   : Mayada Metwally                     ************************
 *************************    Brief: This header file defines  functions for RCC driver**********************.
 ************************************************************************************************************/
#include "RCC_Private.h"
#include "RCC_Reg.h"

#ifndef MCAL_RCC_H_
#define MCAL_RCC_H_


/* These macros should used with RCC_ControlClockAHP1Peripheral function */
#define RCC_AHB1_GPIOA		0x00000001
#define RCC_AHB1_GPIOB		0x00000002
#define RCC_AHB1_GPIOC		0x00000004
#define RCC_AHB1_GPIOD		0x00000008
#define RCC_AHB1_GPIOE		0x00000010
#define RCC_AHB1_GPIOH		0x00000080
#define RCC_AHB1_CRC	    0x00001000
#define RCC_AHB1_DMA1		0x00200000
#define RCC_AHB1_DMA2		0x00400000

/* These macros should used with RCC_ControlClockAHP2Peripheral function */
#define RCC_AHB2_OTGFS		0x00000080

/* These macros should used with RCC_ControlClockABP1Peripheral function  */

#define RCC_APB1_TIM2       0x00000001
#define RCC_APB1_TIM3       0x00000002
#define RCC_APB1_TIM4       0x00000004
#define RCC_APB1_TIM5       0x00000008
#define RCC_APB1_WWDG       0x00000800
#define RCC_APB1_SPI2       0x00004000
#define RCC_APB1_SPI3       0x00008000
#define RCC_APB1_USART2     0x00020000
#define RCC_APB1_I2C1       0x00200000
#define RCC_APB1_I2C2       0x00400000
#define RCC_APB1_I2C3       0x00800000
#define RCC_APB1_PWR        0x10000000

/* These macros should used with RCC_ControlClockABP2Peripheral function  */

#define RCC_APB2_TIM1       0x00000001
#define RCC_APB2_USART1     0x00000010
#define RCC_APB2_USART6     0x00000020
#define RCC_APB2_ADC1       0x00000100
#define RCC_APB2_SDIO       0x00000800
#define RCC_APB2_SPI1       0x00001000
#define RCC_APB2_SPI4       0x00002000
#define RCC_APB2_SYSCFG     0x00004000
#define RCC_APB2_TIM9       0x00010000
#define RCC_APB2_TIM10      0x00020000
#define RCC_APB2_TIM11      0x00040000

/* These macros should used with MRCC_SelectPLLPFactor function */
#define RCC_PLLP_2         0x00000000
#define RCC_PLLP_4         0x00010000
#define RCC_PLLP_6         0x00020000
#define RCC_PLLP_8         0x00030000

/* These macros should used with MRCC_SelectPLLMFactor function */
#define RCC_PLLM_MIN       0x00000002
#define RCC_PLLM_MAX       0x0000003F

/* These macros should used with MRCC_SelectPLLNFactor function */
#define RCC_PLLN_MIN       0x00003000
#define RCC_PLLN_MAX       0x00006C00
/* These macros should used with RCC_EnablePeripheralClock and RCC_DisablePeripheralClock functions  */
#define RCC_AHB1                               1
#define RCC_AHB2                               2
#define RCC_APB1                               3
#define RCC_APB2                               4
#define RCC_LPAPB1                             5
#define RCC_LPAPB2                             6
#define RCC_LPAHB1                             7
#define RCC_LPAHB2                             8

/* These macros should used with RCC_SelectSystemClock function  */
#define RCC_SYSTEM_CLOCK_HSI                   0
#define RCC_SYSTEM_CLOCK_HSE                   1
#define RCC_SYSTEM_CLOCK_PLL                   2
/* These macros should used with RCC_SelectPLLInputClock function  */
#define RCC_CLOCK_HSE_CRUSTAL                  3
#define RCC_CLOCK_HSE_RC                       4
#define RCC_CLOCK_HSI                          5
#define RCC_CLOCK_PLL                          6
/* These macros should used with RCC_EnablePeripheralClock and RCC_DisablePeripheralClock functions  */
#define RCC_ENABLE                             1
#define RCC_DISABLE                            2




/*--------------------------------------------- APIs Interfaces ------------------------------------------*/

/************************************Function Enable Peripheral Clock************************************/

/*
 *@brief      : Function to Enable Peripheral Clock based on the provided input.
 *@param[in1] : Copy_u8BusId, is a Bus ID (AHB1,AHB2,APB1,APB2)
 *@param[in2] : Copy_u8PerId is The pin number of the peripheral in reg
 *@return     : tenu_ErrorStatus indicating the success or failure of the Function.
 *@details	  : This function Enables Peripheral Clock based on input parameter type of bus and number
 *of peripheral pin in Register   .
 * @note	  : The function returns an error if user enters a number greater than ٌRCC_NUMBER_OF_PINS(31) .
 */
tenu_ErrorStatus MRCC_EnablePeripheralClock(u8 BusId,u8 PerId);

/************************************Function Disable Peripheral Clock************************************/
/*
 *@brief      : Function to Disable Peripheral Clock based on the provided input.
 *@param[in1] : Copy_u8BusId, is a Bus ID (AHB1,AHB2,APB1,APB2)
 *@param[in2] : Copy_u8PerId is The pin number of the peripheral in reg
 *@return     : tenu_ErrorStatus indicating the success or failure of the Function.
 *@details	  : This function Disables Peripheral Clock based on input parameter type of bus and number
 *of peripheral pin in Register   .
 * @note	  : The function returns an error if user enters a number greater than ٌRCC_NUMBER_OF_PINS(31) .
 */
tenu_ErrorStatus MRCC_DisablePeripheralClock(u8 BusId,u8 PerId);
/************************************Functions Control Peripheral Clock************************************/
tenu_ErrorStatus MRCC_ControlClockAHP1Peripheral(u32 AHB1Peripheral,u8 Status);
tenu_ErrorStatus MRCC_ControlClockAHP2Peripheral(u32 AHB2Peripheral,u8 Status);
tenu_ErrorStatus MRCC_ControlClockABP1Peripheral(u32 APB1Peripheral,u8 Status);
tenu_ErrorStatus MRCC_ControlClockABP2Peripheral(u32 APB2Peripheral,u8 Status);

/******************************Function to Control Clock (Enable/Disable)*******************************/
tenu_ErrorStatus MRCC_ControlClock(u8 SystemName,u8 Status);
/******************************Function to CHECK Clock (Ready/Not Ready)*******************************/
tenu_ErrorStatus MRCC_CheckHSIStatus(u8 *Status);
tenu_ErrorStatus MRCC_CheckHSEStatus(u8 *Status);
tenu_ErrorStatus MRCC_CheckPLLStatus(u8 *Status);
/******************************Function to Select System Clock****************************************/
tenu_ErrorStatus MRCC_SelectSystemClock(u8 SystemClock);
/******************************Function to Get System Clock****************************************/
tenu_ErrorStatus MRCC_GetSystemClock(u8 *SystemClock);
/******************************Function to Get PLL INPUT Clock****************************************/
tenu_ErrorStatus MRCC_GetPLLInputClock(u8 *PLLInputClock);
/******************************Function to Select PLL INPUT Clock****************************************/
tenu_ErrorStatus MRCC_SelectPLLInputClock(u8 PLLInputClock);
/******************************Function to Select P factor for PLL****************************************/
tenu_ErrorStatus MRCC_SelectPLLPFactor(u32 PLLPFactor);
/******************************Function to Select M factor for PLL****************************************/
tenu_ErrorStatus MRCC_SelectPLLMFactor(u32 PLLMFactor);
/******************************Function to Select N factor for PLL****************************************/
tenu_ErrorStatus MRCC_SelectPLLNFactor(u32 PLLNFactor);


#endif /* MCAL_RCC_H_ */
