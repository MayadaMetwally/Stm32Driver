/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"
#include "HCLCD/LCD.h"
#include "MGPIO/GPIO.h"



/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
LCD_cfg_t HLCD =
{
    .LCD_data_pins[0].Pin = GPIO_PIN_0,                   /*pin location of the first data pin*/
    .LCD_data_pins[0].Port = GPIOB,                       /*port location of the first data pin*/


    .LCD_data_pins[1].Pin = GPIO_PIN_1,                   /*pin location of the second data pin*/
    .LCD_data_pins[1].Port = GPIOB,                       /*port location of the second data pin*/


    .LCD_data_pins[2].Pin = GPIO_PIN_2,                   /*pin location of the third data pin*/
    .LCD_data_pins[2].Port = GPIOB,                       /*port location of the third data pin*/

    
    .LCD_data_pins[3].Pin = GPIO_PIN_3,                   /*pin location of the fourth data pin*/
    .LCD_data_pins[3].Port = GPIOB,                       /*port location of the fourth data pin*/


    #if MODE_OF_LCD_OPERATION == MODE_8_BIT
    .LCD_data_pins[4].Pin = GPIO_PIN_4,                   /*pin location of the fifth data pin*/
    .LCD_data_pins[4].Port = GPIOB,                       /*port location of the fifth data pin*/


    .LCD_data_pins[5].Pin = GPIO_PIN_5,                   /*pin location of the sixth data pin*/
    .LCD_data_pins[5].Port = GPIOB,                       /*port location of the sixth data pin*/


    .LCD_data_pins[6].Pin = GPIO_PIN_6,                   /*pin location of the seventh data pin*/
    .LCD_data_pins[6].Port = GPIOB,                       /*port location of the seventh data pin*/


    .LCD_data_pins[7].Pin = GPIO_PIN_7,                   /*pin location of the eighth data pin*/
    .LCD_data_pins[7].Port = GPIOB,                       /*port location of the eighth data pin*/

    #endif

    .R_S_pin.Pin = GPIO_PIN_8,                            /*pin location of the register select pin*/
    .R_S_pin.Port = GPIOB,                               /*port location of the register select pin*/


    .R_W_pin.Pin = GPIO_PIN_9,                            /*pin location of the read write pin*/
    .R_W_pin.Port = GPIOB,                               /*port location of the read write pin*/


    .E_pin.Pin = GPIO_PIN_10,                              /*pin location of the enable pin*/
    .E_pin.Port = GPIOB,                                    /*port location of the enable pin*/

};


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/




