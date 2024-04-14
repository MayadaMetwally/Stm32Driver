/*
 * KYD_Config.c
 *
 *  Created on: Jan 6, 2024
 *      Author: DELL
 */

#include"STD_TYPES.h"
#include"HKPD/KYD.h"
#include"MGPIO/GPIO.h"


tstr_KPDPinConfiguration KPD_Conf = {
  .RowPinNumber[0] =GPIO_PIN_0 ,.RowPinNumber[1]=GPIO_PIN_1,.RowPinNumber[2]=GPIO_PIN_2,.RowPinNumber[3]=GPIO_PIN_3,
  .RowPortNumber[0]=GPIOA,.RowPortNumber[1]=GPIOA,.RowPortNumber[2]=GPIOA,.RowPortNumber[3]=GPIOA,
  .RowModePin[0]=GPIO_MODE_IN_PU,.RowModePin[1]=GPIO_MODE_IN_PU,.RowModePin[2]=GPIO_MODE_IN_PU,.RowModePin[3]=GPIO_MODE_IN_PU,

  .ColPinNumber[0] = GPIO_PIN_4,.ColPinNumber[1]=GPIO_PIN_5,.ColPinNumber[2]=GPIO_PIN_6,.ColPinNumber[3]=GPIO_PIN_7,
  .ColPortNumber[0]=GPIOA,.ColPortNumber[1]=GPIOA,.ColPortNumber[2]=GPIOA,.ColPortNumber[3]=GPIOA,
  .ColModePin[0]=GPIO_MODE_OP_PP,.ColModePin[1]=GPIO_MODE_OP_PP,.ColModePin[2]=GPIO_MODE_OP_PP,.ColModePin[3]=GPIO_MODE_OP_PP,


};
