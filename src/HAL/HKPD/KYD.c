
#include"STD_TYPES.h"
#include"HKPD/KYD.h"
#include"MGPIO/GPIO.h"

#define THRESHOLD          5
#define SWITCH_OFFSET_4    4
#define SWITCH_PRESSED     1
#define SWITCH_NOT_PRESSED 0 
#define INIT               0
#define PRESSED            1
#define NOT_PRESSED        2
#define NOT_SW_PRESSED     0
#define NEW_VALUE          1
#define OLD_VLAUE          0

extern tstr_KPDPinConfiguration KPD_Conf;
static char KPDPressedKey=0;
static u8 NewValueflag=0;

tenu_ErrorStatus KPD_INIT(void)
{

	GPIO_Pin_tstr Switch_Pin;
	Switch_Pin.Speed=GPIO_SPEED_HIGH;
	/*define variable to indicating the success or failure of the function */
	tenu_ErrorStatus Local_u8ErrorStatus = LBTY_OK;
	/*define variable to loop on all col pins and row pins */
	u8 Local_u8Index=0;
	/*define all col pin as output high */
	for(Local_u8Index=0;Local_u8Index<KPD_NUMBER_OF_COLUMNS;Local_u8Index++)
	{
		Switch_Pin.Mode=KPD_Conf.ColModePin[Local_u8Index];
		Switch_Pin.Pin=KPD_Conf.ColPinNumber[Local_u8Index];
		Switch_Pin.Port=KPD_Conf.ColPortNumber[Local_u8Index];
		MGPIO_InitPin(&Switch_Pin);
		MGPIO_SetPin(Switch_Pin.Port,Switch_Pin.Pin,GPIO_High);
	}
	/*define all row pin as input pull up */
	for(Local_u8Index=0;Local_u8Index<KPD_NUMBER_OF_ROWS;Local_u8Index++)
	{
		Switch_Pin.Mode=KPD_Conf.RowModePin[Local_u8Index];
		Switch_Pin.Pin=KPD_Conf.RowPinNumber[Local_u8Index];
		Switch_Pin.Port=KPD_Conf.RowPortNumber[Local_u8Index];
		MGPIO_InitPin(&Switch_Pin);
	}
	/*return value of Local_u8ErrorStatus variable */
	return Local_u8ErrorStatus;
}
/******************************************************************************************************************************/
void KPD_Runnable(void)
{
	u8 Local_Current=2;
	
	u8 Local_CurrentPreesedKey=0;
	/*define variable to loop on all col pins */
	u8 Local_u8Index1=0;
	/*define variable to loop on all row pins */
	u8 Local_u8Index2=0;
	

	static u8 Local_State[KPD_NUMBER_OF_ROWS*KPD_NUMBER_OF_COLUMNS]={INIT};
	
	/*local array to store keypad name that user entered it in configuration*/
	u8 Local_u8KpdArr[KPD_NUMBER_OF_ROWS][KPD_NUMBER_OF_COLUMNS] = KPD_ARRAY;
	for(Local_u8Index1=0;(Local_u8Index1<KPD_NUMBER_OF_COLUMNS&&Local_CurrentPreesedKey==NOT_SW_PRESSED);Local_u8Index1++)
	{
		/*define col pin as output low */
		MGPIO_SetPin(KPD_Conf.ColPortNumber[Local_u8Index1],KPD_Conf.ColPinNumber[Local_u8Index1],GPIO_Low);
		/*read the current row (pressed--->low) (not pressed--->high)*/
		for(Local_u8Index2=0;(Local_u8Index2<KPD_NUMBER_OF_ROWS&&Local_CurrentPreesedKey==NOT_SW_PRESSED);Local_u8Index2++)
		{

			/*check if switch is pressed */
			switch (Local_State[(Local_u8Index1*KPD_NUMBER_OF_ROWS)+Local_u8Index2])
			{
			case INIT:
				/*get pin value of current row */
				MGPIO_GetPinState(KPD_Conf.RowPortNumber[Local_u8Index2],KPD_Conf.RowPinNumber[Local_u8Index2],&Local_Current);
				if(Local_Current==GPIO_Low)
				{
					Local_State[(Local_u8Index1*KPD_NUMBER_OF_ROWS)+Local_u8Index2]=PRESSED;
				}
				else
				{
					Local_State[(Local_u8Index1*KPD_NUMBER_OF_ROWS)+Local_u8Index2]=NOT_PRESSED;
				}
				break;
			case PRESSED:
				/*get pin value of current row */
				MGPIO_GetPinState(KPD_Conf.RowPortNumber[Local_u8Index2],KPD_Conf.RowPinNumber[Local_u8Index2],&Local_Current);
				if(Local_Current==GPIO_Low)
				{
					Local_CurrentPreesedKey=Local_u8KpdArr[Local_u8Index1][Local_u8Index2];
				}
				else
				{
					Local_State[(Local_u8Index1*KPD_NUMBER_OF_ROWS)+Local_u8Index2]=NOT_PRESSED;
				}
				break;	
			case NOT_PRESSED:
				/*get pin value of current row */
				MGPIO_GetPinState(KPD_Conf.RowPortNumber[Local_u8Index2],KPD_Conf.RowPinNumber[Local_u8Index2],&Local_Current);
				if(Local_Current==GPIO_Low)
				{
					Local_State[(Local_u8Index1*KPD_NUMBER_OF_ROWS)+Local_u8Index2]=PRESSED;
				}

				break;								
			
			default:
				break;
			}
				
		}
		/*define col pin as output high */
		MGPIO_SetPin(KPD_Conf.ColPortNumber[Local_u8Index1],KPD_Conf.ColPinNumber[Local_u8Index1],GPIO_High);


	}
	if(Local_CurrentPreesedKey!=KPDPressedKey)
	{
		KPDPressedKey=Local_CurrentPreesedKey;
		NewValueflag=NEW_VALUE;
	}
	else
	{
		/*do nothing*/
	}	

}
tenu_ErrorStatus KPD_GetPressedKey(u8 *Copy_pu8Key)
{

	/*define variable to indicating the success or failure of the function */
	tenu_ErrorStatus Local_u8ErrorStatus = LBTY_OK;

	/*check if a NULL pointer is provided as input*/
	if(Copy_pu8Key!=NULL)
	{
		if((NewValueflag==NEW_VALUE)&&(KPDPressedKey!=NOT_SW_PRESSED))
		{
			*Copy_pu8Key=KPDPressedKey;
			NewValueflag=OLD_VLAUE;
		}
		else
		{
			*Copy_pu8Key=NOT_SW_PRESSED;
		}

	}
		
	
	else
	{
		/*if a NULL pointer is provided as input return NOK*/
		Local_u8ErrorStatus=LBTY_ErrorNullPointer;
	}
     /*return value of Local_u8ErrorStatus variable */
       return Local_u8ErrorStatus;
}
