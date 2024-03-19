/************************************************************************************************************
 *************************            File Name: GPIO.h                              ************************
 *************************            Layer	   : MCAL                                ************************
 *************************            Module   : GPIO                                ************************
 *************************            Version  : V 0.0                               ************************
 *************************            Created  : 2/13/2023                           ************************
 *************************            Author   : Mayada Metwally                     ************************
 *************************    Brief: This header file defines  functions for GPIO driver*********************.
 ************************************************************************************************************/

#include "MGPIO/GPIO_Reg.h"
#include"MGPIO/GPIO_Private.h"

#ifndef MCAL_MGPIO_GPIO_H_
#define MCAL_MGPIO_GPIO_H_
/* Value */
#define GPIO_High 1
#define GPIO_Low 0

/* GPIO GPIO_Ports */
#define GPIOA                  (void *)(GPIOA_BASE_ADD)
#define GPIOB                  (void *)(GPIOB_BASE_ADD)
#define GPIOC                  (void *)(GPIOC_BASE_ADD)
#define GPIOD                  (void *)(GPIOD_BASE_ADD)
#define GPIOE                  (void *)(GPIOE_BASE_ADD)
#define GPIOH                  (void *)(GPIOH_BASE_ADD)

/* PINS GPIO_*/
#define GPIO_PIN_0                          0x00000000
#define GPIO_PIN_1                          0x00000001
#define GPIO_PIN_2                          0x00000002
#define GPIO_PIN_3                          0x00000003
#define GPIO_PIN_4                          0x00000004
#define GPIO_PIN_5                          0x00000005
#define GPIO_PIN_6                          0x00000006
#define GPIO_PIN_7                          0x00000007
#define GPIO_PIN_8                          0x00000008
#define GPIO_PIN_9                          0x00000009
#define GPIO_PIN_10                         0x0000000A
#define GPIO_PIN_11                         0x0000000B
#define GPIO_PIN_12                         0x0000000C
#define GPIO_PIN_13                         0x0000000D
#define GPIO_PIN_14                         0x0000000E
#define GPIO_PIN_15                         0x0000000F
#define GPIO_16_PINS                        0X1F

/* GPIO pin speeds*/
#define GPIO_SPEED_LOW                      0x00000000
#define GPIO_SPEED_MED                      0x00000001
#define GPIO_SPEED_HIGH                     0x00000002
#define GPIO_SPEED_VHIGH                    0x00000003

/*GPIO pin modes*/
#define GPIO_MODE_OP_PP                     0x00000001
#define GPIO_MODE_OP_PP_PU                  0x00000009
#define GPIO_MODE_OP_PP_PD                  0x00000011
#define GPIO_MODE_OP_OD                     0x00000005
#define GPIO_MODE_OP_OD_PU                  0x0000000D
#define GPIO_MODE_OP_OD_PD                  0x00000015

#define GPIO_MODE_AF_PP                     0x00000002
#define GPIO_MODE_AF_PP_PU                  0x0000000A
#define GPIO_MODE_AF_PP_PD                  0x00000012
#define GPIO_MODE_AF_OD                     0x00000006
#define GPIO_MODE_AF_OD_PU                  0x0000000E
#define GPIO_MODE_AF_OD_PD                  0x00000016

#define GPIO_MODE_IN_FL                     0x00000000
#define GPIO_MODE_IN_PU                     0x00000008
#define GPIO_MODE_IN_PD                     0x00000010

#define GPIO_MODE_ANALOG                    0x00000003



typedef struct
{
    void* Port;
    u32   Pin;
    u32   Mode;
    u32   Speed;
}GPIO_Pin_tstr;

/*--------------------------------------------- APIs Interfaces ------------------------------------------*/

/************************************Function Initializes the Pin************************************/

/**
 * @brief Initializes the Pin Mode and Speed according to the
 * sent Configuration struct
 *
 * @param ADD_PinCfg Pointer to struct of Pin configurations
 * @return tenu_ErrorStatus
 * * @note	 : The function returns an error if a NULL pointer is provided as the configuration .
 */


tenu_ErrorStatus MGPIO_InitPin(GPIO_Pin_tstr* ADD_PinCfg);

/************************************Function Set the Pin*****************************************/

/**
 * @brief Sets the selected pin state as either High or Low
 *
 * @param Copy_Port Port containing the Pin
 * @param Copy_Pin Pin number in the port
 * @param Copy_State State to set the Pin to (High, Low)
 * @return tenu_ErrorStatus
 * * @note	 : The function returns an error if a Wrong Choice is provided as the configuration .
 */


tenu_ErrorStatus MGPIO_SetPin(void* Copy_Port, u8 Copy_Pin, u8 Copy_State);
/************************************Function Toggle the Pin*****************************************/
tenu_ErrorStatus MGPIO_TogglePin(void* Copy_Port, u8 Copy_Pin);

/************************************Function get the Pin state************************************/
/**
 * @brief gets the selected Pin state
 *
 * @param Copy_Port Port containing the Pin
 * @param Copy_Pin Pin number in the port
 * @param ADD_State pointer of the Pin to return state of the pin
 * @return tenu_ErrorStatus
 * * @note	 : The function returns an error if a Wrong Choice or NULL pointer  is provided as the configuration .
 */
tenu_ErrorStatus MGPIO_GetPinState(void* Copy_Port, u8 Copy_Pin, u8 * ADD_State);



#endif /* MCAL_MGPIO_GPIO_H_ */
