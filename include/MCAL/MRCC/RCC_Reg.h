/************************************************************************************************************
 *************************            File Name: RCC_Reg.h                           ************************
 *************************            Layer	   : MCAL                                ************************
 *************************            Module   : RCC                                 ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/4/2023                            ************************
 *************************            Author   : Mayada Metwally                     ************************
 *************************Brief: This Header file defines the register for RCC driver************************.
 ************************************************************************************************************/

#ifndef MCAL_RCC_REG_H_
#define MCAL_RCC_REG_H_



#define RCC_CR                       *((u32*)(RCC_BASE_ADD+0x00))
#define RCC_PLLCFGR                  *((u32*)(RCC_BASE_ADD+0x04))
#define RCC_CFGR                     *((u32*)(RCC_BASE_ADD+0x08))
#define RCC_CIR                      *((u32*)(RCC_BASE_ADD+0x0C))
#define RCC_AHB1RSTR                 *((u32*)(RCC_BASE_ADD+0x10))
#define RCC_APB1RSTP                 *((u32*)(RCC_BASE_ADD+0x20))
#define RCC_APB2RSTR                 *((u32*)(RCC_BASE_ADD+0x24))
#define RCC_AHB1ENR                  *((u32*)(RCC_BASE_ADD+0x30))
#define RCC_AHB2ENR                  *((u32*)(RCC_BASE_ADD+0x34))
#define RCC_APB1ENR                  *((u32*)(RCC_BASE_ADD+0x40))
#define RCC_APB2ENR                  *((u32*)(RCC_BASE_ADD+0x44))
#define RCC_AHB1LPENR                *((u32*)(RCC_BASE_ADD+0x50))
#define RCC_AHB2LPENR                *((u32*)(RCC_BASE_ADD+0x54))
#define RCC_APB1LPENR                *((u32*)(RCC_BASE_ADD+0x60))
#define RCC_APB2LPENR                *((u32*)(RCC_BASE_ADD+0x64))
#define RCC_BDCR                     *((u32*)(RCC_BASE_ADD+0x70))
#define RCC_CSR                      *((u32*)(RCC_BASE_ADD+0x74))
#define RCC_SSCGR                    *((u32*)(RCC_BASE_ADD+0x80))
#define RCC_PLLI2SCFGR               *((u32*)(RCC_BASE_ADD+0x84))
#define RCC_DCKCFGR                  *((u32*)(RCC_BASE_ADD+0x8C))




#endif /* MCAL_RCC_REG_H_ */
