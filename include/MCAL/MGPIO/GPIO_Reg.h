/************************************************************************************************************
 *************************            File Name: GPIO_Reg.h                          ************************
 *************************            Layer	   : MCAL                                ************************
 *************************            Module   : GPIO                                ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/13/2023                           ************************
 *************************            Author   : Mayada Metwally                     ************************
 *************************Brief: This Header file defines the register for GPIO driver***********************.
 ************************************************************************************************************/

#ifndef MCAL_MGPIO_GPIO_REG_H_
#define MCAL_MGPIO_GPIO_REG_H_
/***************** Types Definition **************/
typedef struct{
	volatile u32 MODER;
	volatile u32 OTYPER;
	volatile u32 OSPEEDER;
	volatile u32 PUPDR;
	volatile u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 LCKR;
	volatile u32 AFRL;
	volatile u32 AFRH;

}GPIO_Reg;

/***************** Variables Definition **************/


/**               GPIOA BASE ADD               **/
#define GPIOA_BASE_ADD                 0x40020000
/**               GPIOB BASE ADD               **/
#define GPIOB_BASE_ADD                 0x40020400
/**               GPIOC BASE ADD               **/
#define GPIOC_BASE_ADD                 0x40020800
/**               GPIOD BASE ADD               **/
#define GPIOD_BASE_ADD                 0x40020C00
/**               GPIOE BASE ADD               **/
#define GPIOE_BASE_ADD                 0x40021000
/**               GPIOH BASE ADD               **/
#define GPIOH_BASE_ADD                 0x40021C00

#endif /* MCAL_MGPIO_GPIO_REG_H_ */
