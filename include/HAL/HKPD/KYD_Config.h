/*
 * KYD_Config.h
 *
 *  Created on: Jan 6, 2024
 *      Author: DELL
 */

#ifndef HAL_KYD_KYD_CONFIG_H_
#define HAL_KYD_KYD_CONFIG_H_

/***************************************configuration parameters to keypad***********************************************/
/*number of rows*/
#define KPD_NUMBER_OF_ROWS              4
/*number of columns*/
#define KPD_NUMBER_OF_COLUMNS           4
/*The names of the switches in keypad*/
#define KPD_ARRAY                       {{'X','L','X','M'},{'X','D','U','S'},{'X','R','K','C'},{'X','-','+','E'}}
												// M S C E
												//X U K +
												//L D R -
												//X X X X
//#define KPD_ARRAY                       {{'M','S','C','E'},{'X','U','K','+'},{'L','D','R','-'},{'X','X','X','X'}}
/*The sign that indicates there are no buttons pressed*/
#define KPD_NO_PRESSED_SWITCH           0xff


typedef struct
{
	u8  RowPinNumber[KPD_NUMBER_OF_ROWS];
	void*  RowPortNumber[KPD_NUMBER_OF_ROWS];
	u8  RowModePin[KPD_NUMBER_OF_ROWS];

	u8  ColPinNumber[KPD_NUMBER_OF_COLUMNS];
	void *  ColPortNumber[KPD_NUMBER_OF_COLUMNS];
	u8  ColModePin[KPD_NUMBER_OF_COLUMNS];

}tstr_KPDPinConfiguration;



#endif /* HAL_KYD_KYD_CONFIG_H_ */
