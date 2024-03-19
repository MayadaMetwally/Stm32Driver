/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"
#include "MNVIC_Config.h"
#include "STM32F401CC.h"








/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define GROUP_4_BIT_SUB_0_BIT  0x05FA0000
#define GROUP_3_BIT_SUB_1_BIT  0x05FA0400
#define GROUP_2_BIT_SUB_2_BIT  0x05FA0500
#define GROUP_1_BIT_SUB_3_BIT  0x05FA0600
#define GROUP_0_BIT_SUB_4_BIT  0x05FA0700


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/




/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
void MNVIC_Init(void);
/**
 * @brief Enables a specific interrupt in the NVIC by setting the corresponding bit in the ISER register.
 *
 * This function takes the interrupt ID as input and enables the corresponding interrupt by manipulating the ISER register.
 * It checks for the validity of the input interrupt ID and returns an error status if the ID is invalid.
 *
 * @param Copy_InterruptID ID of the interrupt to be enabled.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MNVIC_EnableInterrupt(u8 Copy_InterruptID);
/**
 * @brief Disables a specific interrupt in the NVIC by setting the corresponding bit in the ICER register.
 *
 * This function takes the interrupt ID as input and disables the corresponding interrupt by manipulating the ICER register.
 * It checks for the validity of the input interrupt ID and returns an error status if the ID is invalid.
 *
 * @param Copy_InterruptID ID of the interrupt to be disabled.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MNVIC_DisableInterrupt(u8 Copy_InterruptID);
/**
 * @brief Sets a specific interrupt as pending in the NVIC by setting the corresponding bit in the ISPR register.
 *
 * This function takes the interrupt ID as input and sets the corresponding interrupt as pending by manipulating the ISPR register.
 * It is part of interrupt control and handling.
 *
 * @param Copy_InterruptID ID of the interrupt to be set as pending.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MNVIC_SetPanding(u8 Copy_InterruptID);

/**
 * @brief Clears the pending status of a specific interrupt in the NVIC by setting the corresponding bit in the ICPR register.
 *
 * This function takes the interrupt ID as input and clears the pending status of the corresponding interrupt by manipulating the ICPR register.
 * It is part of interrupt control and handling.
 *
 * @param Copy_InterruptID ID of the interrupt to clear pending status.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */

tenu_ErrorStatus MNVIC_Clearpanding(u8 Copy_InterruptID);
/**
 * @brief Gets the active status of a specific interrupt in the NVIC.
 *
 * This function takes the interrupt ID as input and retrieves the active status of the corresponding interrupt by reading the IABR register.
 * The active status (1 if active, 0 if not) is stored in the provided memory location pointed to by Add_Status.
 *
 * @param Copy_InterruptID ID of the interrupt to check active status.
 * @param Add_Status Pointer to store the active status.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MNVIC_GetActiveStatus(u8 Copy_InterruptID,u8 * Add_Status);
/**
 * @brief Sets the priority of a specific interrupt in the NVIC by writing the priority value to the IPR register.
 *
 * This function takes the interrupt ID and priority value as input and sets the priority of the corresponding interrupt
 * by manipulating the IPR register. It is part of interrupt control and handling.
 *
 * @param Copy_InterruptID ID of the interrupt to set the priority.
 * @param copy_Priority Priority value to set.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MNVIC_SetPriority(u8 Copy_InterruptID,u8 copy_Priority);
/**
 * @brief Generates a software interrupt in the NVIC by writing the interrupt ID to the STIR register.
 *
 * This function takes the interrupt ID as input and generates a software interrupt by writing the ID to the STIR register.
 * It is used for testing or triggering software-induced interrupts.
 *
 * @param Copy_InterruptID ID of the interrupt to generate.
 * @return tenu_ErrorStatus Error status indicating success or failure.
 */
tenu_ErrorStatus MNVIC_GenerateSWInterrupt(u8 Copy_InterruptID);
