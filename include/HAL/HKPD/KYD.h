/*
 * KYD.h
 *
 *  Created on: Jan 6, 2024
 *      Author: DELL
 */

#ifndef HAL_KYD_KYD_H_
#define HAL_KYD_KYD_H_
#include"KYD_Config.h"

/* @brief	 : Initializes the KPD module based on the provided configuration.
 * @param[in]: void.
 * @return	 : tenu_ErrorStatus indicating the success or failure of the initialization.
 * @details	 : This function init all row pins as input pullup and init all col pins as output high.
 */
tenu_ErrorStatus KPD_INIT(void);
/****************************************************************************************/
/* @brief	 : get which key is pressed.
 * @param[in]: pointer to key which is pressed .
 * @return	 : tenu_ErrorStatus indicating the success or failure of the function to Get Pressed Key.
 * @details	 : This function tell me which button is pressed by define col pin value as low,
 *  and you can see the values ​​of the rows. If one of them is zero, then this is the one pressed, even if it is not found .
 *  and then define col pin value as High and shifted to the next col pin
 * @note	 : The function returns an error if a NULL pointer is provided as input.
 */
tenu_ErrorStatus KPD_GetPressedKey(u8 *Copy_pu8Key);


#endif /* HAL_KYD_KYD_H_ */
