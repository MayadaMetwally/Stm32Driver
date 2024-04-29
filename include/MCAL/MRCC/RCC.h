/**************************************************************************************************************/
/* File Name:	RCC.h                                                                                         */
/* Module	: 	RCC						                                                                      */
/* Layer	:	MCAL										                                                  */
/* AUTHOR   :   Lina Nassar                                                                                   */
/* Date     :   10/2/2024                                                                                     */
/**************************************************************************************************************/

#ifndef RCC_H_
#define RCC_H_

#include "Masks.h"
#include "STD_Types.h"


/*Peripherals Codes*/
#define AHB1_Code       1         
#define AHB2_Code       2
#define APB1_Code       3
#define APB2_Code       4 

/*Availabe Clocks*/
#define Clock_HSI           Reg_Mask_bit0
#define Clock_HSE           Reg_Mask_bit16
#define Clock_PLL           Reg_Mask_bit24
#define Clock_PLLI2S        Reg_Mask_bit26
#define Clock_RTC           Reg_Mask_bit15

/*PLL Source Clock*/
#define PLLSrc_HSE          0x0
#define PLLSrc_HSI          Reg_Mask_bit22

/*PLLP Values*/
#define PLLP_2              0x0
#define PLLP_4              0x1
#define PLLP_6              0x2
#define PLLP_8              0x3

/*System Clocks*/
#define SysClock_HSI        0x0
#define SysClock_HSE        0x1
#define SysClock_PLL        0x2

/*Clock Status*/
#define ClockStatus_Disable 0 
#define ClockStatus_Enable  1

/*Peripheral Status*/
#define PeriStatus_Disable  0 
#define PeriStatus_Enable   1

/*Power Mode*/
#define PowerMode_Normal    1
#define PowerMode_LowPwr    0


/*Peripherals*/

/*AHB1 Peripherals*/
#define Peri_GPIOA          CONCAT(AHB1_Code, Reg_Mask_bit0)
#define Peri_GPIOB          CONCAT(AHB1_Code, Reg_Mask_bit1)
#define Peri_GPIOC          CONCAT(AHB1_Code, Reg_Mask_bit2)
#define Peri_GPIOD          CONCAT(AHB1_Code, Reg_Mask_bit3)
#define Peri_GPIOE          CONCAT(AHB1_Code, Reg_Mask_bit4)   
#define Peri_GPIOH          CONCAT(AHB1_Code, Reg_Mask_bit7)
#define Peri_CRC            CONCAT(AHB1_Code, Reg_Mask_bit12)
#define Peri_DMA2           CONCAT(AHB1_Code, Reg_Mask_bit21)
#define Peri_DMA1           CONCAT(AHB1_Code, Reg_Mask_bit22)

/*AHB2 Peripherals*/
#define Peri_OTGFS          CONCAT(AHB2_Code, Reg_Mask_bit7)

/*APB1 Peripherals*/
#define Peri_TIM2           CONCAT(APB1_Code, Reg_Mask_bit0)   
#define Peri_TIM3           CONCAT(APB1_Code, Reg_Mask_bit1)
#define Peri_TIM4           CONCAT(APB1_Code, Reg_Mask_bit2)
#define Peri_TIM5           CONCAT(APB1_Code, Reg_Mask_bit3) 
#define Peri_WWDG           CONCAT(APB1_Code, Reg_Mask_bit11)
#define Peri_SPI2           CONCAT(APB1_Code, Reg_Mask_bit14)
#define Peri_SPI3           CONCAT(APB1_Code, Reg_Mask_bit15)
#define Peri_USART2         CONCAT(APB1_Code, Reg_Mask_bit17)
#define Peri_I2C1           CONCAT(APB1_Code, Reg_Mask_bit21)
#define Peri_I2C2           CONCAT(APB1_Code, Reg_Mask_bit22)
#define Peri_I2C3           CONCAT(APB1_Code, Reg_Mask_bit23)
#define Peri_PWR            CONCAT(APB1_Code, Reg_Mask_bit28)               

/*APB2 Peripherals*/
#define Peri_TIM1           CONCAT(APB2_Code, Reg_Mask_bit0)
#define Peri_USART1         CONCAT(APB2_Code, Reg_Mask_bit4)
#define Peri_USART6         CONCAT(APB2_Code, Reg_Mask_bit5)
#define Peri_ADC1           CONCAT(APB2_Code, Reg_Mask_bit8)
#define Peri_SDIO           CONCAT(APB2_Code, Reg_Mask_bit11)
#define Peri_SPI1           CONCAT(APB2_Code, Reg_Mask_bit12)
#define Peri_SPI4           CONCAT(APB2_Code, Reg_Mask_bit13)
#define Peri_SYSCFG         CONCAT(APB2_Code, Reg_Mask_bit14)
#define Peri_TIM9           CONCAT(APB2_Code, Reg_Mask_bit16)
#define Peri_TIM10          CONCAT(APB2_Code, Reg_Mask_bit17)
#define Peri_TIM11          CONCAT(APB2_Code, Reg_Mask_bit18)



/*Peripheral Prescalers*/
#define PeriPresc_AHB            Reg_bit4       
#define PeriPresc_APB1           Reg_bit10
#define PeriPresc_APB2           Reg_bit13
#define PeriPresc_RTC            Reg_bit16
#define PeriPresc_MCO1           Reg_bit24
#define PeriPresc_MCO2           Reg_bit27

/*Peripheral Prescalers Options */
#define Prescaler_AHB_Div1       0b0111
#define Prescaler_AHB_Div2       0b1000
#define Prescaler_AHB_Div4       0b1001
#define Prescaler_AHB_Div8       0b1010
#define Prescaler_AHB_Div16      0b1011
#define Prescaler_AHB_Div64      0b1100
#define Prescaler_AHB_Div128     0b1101
#define Prescaler_AHB_Div256     0b1110
#define Prescaler_AHB_Div512     0b1111

#define Prescaler_APB1_Div1      0b011
#define Prescaler_APB1_Div2      0b100
#define Prescaler_APB1_Div4      0b101
#define Prescaler_APB1_Div8      0b110
#define Prescaler_APB1_Div16     0b111

#define Prescaler_APB2_Div1      0b011
#define Prescaler_APB2_Div2      0b100
#define Prescaler_APB2_Div4      0b101
#define Prescaler_APB2_Div8      0b110
#define Prescaler_APB2_Div16     0b111

#define Prescaler_MCO1_Div1      0b011
#define Prescaler_MCO1_Div2      0b100
#define Prescaler_MCO1_Div4      0b101
#define Prescaler_MCO1_Div8      0b110
#define Prescaler_MCO1_Div16     0b111

#define Prescaler_MCO2_Div1       0b011
#define Prescaler_MCO2_Div2       0b100
#define Prescaler_MCO2_Div4       0b101
#define Prescaler_MCO2_Div8       0b110
#define Prescaler_MCO2_Div16      0b111


/*Return Errors*/
typedef enum
{
    RCC_enuOk,
    RCC_enuNOk,
    RCC_enuClockNotFound,
    RCC_enuWrongStatus,
    RCC_enuWrongPLLSrc,
    RCC_enuParameterOutofRange,
    RCC_enuFreqOutofRange,
    RCC_enuPLLisEnabled,
    RCC_enuClockisDisabled,
    RCC_enuClockNotRdy,
    RCC_enuPeriNotFound,
    RCC_enuWrongPwrMod
    
}RCC_errors_t;


/*Function Prototypes*/

/**
 *@brief  : Let's the user Enable/Disable a clock. A clock that is selected as a system clock cannot be 
 *          disabled before switching to another system clock.
 *@param  : Clock name -> CLock_HSE, CLock_HSI, CLock_PLL, CLock_PLLI2S, CLock_RTC
 *        : The status required -> ClockStatus_Enable or ClockStatus_Disable
 *@return : RCC Error Status               
 */
 RCC_errors_t RCC_ControlClock(uint32_t Clock, uint8_t CLockStatus);

/**
 *@brief  : 
 *   Frequency Equations:
 *   ---------------------
 *      1. f(VCO I/P) = f(PLL Src Clk) / PLLM
 *      2. f(VCO O/P) = f(VCO I/P) * PLLN
 *      3. f(PLL general clock output) = f(VCO O/P) / PLLP
 *      4. f(USB OTG FS, SDIO, RNG clock output) = f(VCO clock) / PLLQ
 *      
 *    Parameter Constraints:
 *   -----------------------
 *      1. 2 <= PLLQ <= 15
 *      2. PLLP: 2 or 4 or 6 or 8
 *      3. 192 <= PLLN <= 432
 *      4. 2 <= PLLM <= 53
 *   
 *   Frequency Cnstraints:
 *   ----------------------
 *      1. 1MHz <= f(VCO I/P) <= 2MHz 
 *      2. 192MHz <= f(VCO O/P) <= 432MHz 
 *      3. f(PLL general clock output) <= 84MHz 
 *      4. f(USB OTG FS, SDIO, RNG clock output) = 48MHz 
 *   
 *@param  : PLLSrc, PLLM, PLLN, PLLP, PLLQ
 *@return : Error Status               
 */
 RCC_errors_t RCC_ConfigurePLL(uint32_t PLLSrc, uint32_t PLLM, uint32_t PLLN, uint16_t PLLP, uint32_t PLLQ );

/**
 *@brief  : Configures the system clock. To select a clock as a system clock, it must be enabled & ready
 *@param  : SysClock
 *@return : Error Status               
 */
 RCC_errors_t RCC_ConfigureSysClock(uint32_t SysClock );

/**
 *@brief  : Enables and disables a Peripheral in the selected power mode.
 *@param  : Peripheral, Enable/Disable, Powermode
 *@return : Error Status               
 */
 RCC_errors_t RCC_ControlPeripheralClock(uint64_t Peri, uint32_t PeriStatus, uint32_t PowerMode );

/**
 *@brief  : Sets the prescaler for each Peripheral 
 *@param  : Perpheral, Division factor
 *@return : Error Status               
 */
 RCC_errors_t RCC_ConfigurePrescaler(uint32_t PeriPreSc, uint32_t PreScaler);

 RCC_errors_t RCC_NewControlPeripheralClock(uint64_t Peri, uint32_t PeriStatus, uint32_t PowerMode );

#endif /* RCC_H_ */
