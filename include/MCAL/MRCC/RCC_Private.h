/************************************************************************************************************
 *************************            File Name: RCC_Private.h                       ************************
 *************************            Layer	   : MCAL                                ************************
 *************************            Module   : RCC                                 ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/4/2023                            ************************
 *************************            Author   : Mayada Metwally                     ************************
 *********************Brief: This Header file defines the user Macros for RCC driver*************************.
 ************************************************************************************************************/

#ifndef MCAL_RCC_PRIVATE_H_
#define MCAL_RCC_PRIVATE_H_

/**                RCC BASE ADD              **/
#define RCC_BASE_ADD                 0x40023800


/**                PLL SOURCE                 **/
#define RCC_PLL_INPUT_HSI                      0
#define RCC_PLL_INPUT_HSE                      1
#define RCC_PLL_SOURCE                         22


#define RCC_NUMBER_OF_PINS                     32

/**                CLOCK  STATUS            **/
#define RCC_CLK_READY                          1
#define RCC_CLK_NOT_READY                      0
/**                PLL INPUT SOURCE            **/
#define RCC_HSI_PLL_SOURCE                     0
#define RCC_HSE_PLL_SOURCE                     1
/* Masks used for Check status functions */
#define RCC_HSE_ENABLE_MASK                    16
#define RCC_PLL_ENABLE_MASK                    24
#define RCC_PLL_SOURCE_MASK                    22
#define RCC_HSE_RC_ENABLE_MASK                 0X00050000
#define RCC_HSI_ENABLE_MASK                    0X00000081
#define RCC_HSI_SYSTEM_CLOCK_MASK              0X00000000
#define RCC_HSE_SYSTEM_CLOCK_MASK              0X00000001
#define RCC_PLL_SYSTEM_CLOCK_MASK              0X00000002
#define RCC_SYSTEM_CLOCK_MASK                  0XFFFFFFFC

#define RCC_SYSTEM_CLK_MASK                    0X0000000C

#define HSI_RDY 		                       1
#define HSE_RDY 		                       17
#define PLL_RDY 		                       25
#define SWS_HSI 		                       0
#define SWS_HSE 	 	                       1
#define SWS_PLL 	 	                       2
#define RCC_SWS_MASK			               2

#define RCC_PLLN_MASK                          0XFFFF803F
#define RCC_PLLM_MASK                          0XFFFFFFC0
#define RCC_PLLP_MASK                          0XFFFCFFFF








#endif /* MCAL_RCC_PRIVATE_H_ */
