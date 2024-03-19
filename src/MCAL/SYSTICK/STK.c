/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"
#include "STK/STK.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define STK_BASE_ADDRESS                      0xE000E010
#define STK_CLR_REG                           0X0
#define STK_ENABLE_BIT                        0
#define STK_TICKINT_BIT                       1
#define STK_CLKSRC_BIT                        2
#define STK_COUNT_FLAG_BIT                    16
#define STK_CLK_OFFSET                        2

#define STK_AHB_DIV_8_ENABLE                  0x0
#define STK_AHB_ENABLE                        0x4

#define STK_MAX_PREELOAD_VALUE                167777215
#define STK_MAX_TIME_MS                       (STK_MAX_PREELOAD_VALUE*(u32)1000)/STK_CLOCK_SOURCE

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct {
	volatile u32  STK_CTRL;
	volatile u32  STK_LOAD;
	volatile u32  STK_VAL;
	volatile u32  STK_CALIB;

}STK_t;


static u32 Clock_source ;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
STK_t *const STK  = (STK_t *)STK_BASE_ADDRESS;   /* Pointer to the STK peripheral registers*/
u8 STK_Periodic = 0;              /*Flag to indicate whether the interval is periodic or single*/


/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static STK_CBF_t CallBack;   /*Pointer to a function to be called back during the interval*/


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

/*********************************************MSTK_Init Implementation****************************************/
void MSTK_Init (void){
	STK->STK_CTRL=STK_CLR_REG;         /*Clear control register*/
    #if STK_CLOCK_SOURCE==STK_AHB
	     STK->STK_CTRL=STK_AHB_ENABLE;   /*Set the clock source to AHB*/
	     Clock_source=STK_AHB;
    #elif STK_CLOCK_SOURCE==STK_AHB_DIV_8
	     STK->STK_CTRL=STK_AHB_DIV_8_ENABLE;  /*Set the clock source to AHB/8*/
	     Clock_source=STK_AHB_DIV_8 ;

    #else
         #error "invalid option"
#endif

}
/*********************************************MSTK_SetTimerMS Implementation****************************************/
tenu_ErrorStatus MSTK_SetTimerMS (u32 Copy_MSTime){
	tenu_ErrorStatus Local_EroorStatus=LBTY_OK;
	if(Copy_MSTime>STK_MAX_TIME_MS)
	{
		Local_EroorStatus=LBTY_ErrorInvalidInput;
	}
	else
	{
		STK->STK_LOAD=((Copy_MSTime*Clock_source)/1000UL)-1;
	}

	return Local_EroorStatus;
}
/*********************************************MSTK_StartTimerMSSingle Implementation*********************************/
tenu_ErrorStatus MSTK_StartTimerMSSingle (){
	tenu_ErrorStatus Local_EroorStatus=LBTY_OK;
	STK_Periodic=0;                     /*Set the interval as single*/
	STK->STK_CTRL|=(1<<STK_ENABLE_BIT); /*Enable the timer*/
	STK->STK_CTRL|=(1<<STK_TICKINT_BIT);/*Enable the SysTick interrupt*/


	return Local_EroorStatus;

}
/*********************************************MSTK_StartTimerMSPeriodic Implementation****************************************/
tenu_ErrorStatus MSTK_StartTimerMSPeriodic (){
	tenu_ErrorStatus Local_EroorStatus=LBTY_OK;
	STK_Periodic=1;                     /*Set the interval as periodic*/
	STK->STK_CTRL|=(1<<STK_ENABLE_BIT); /*Enable the timer*/
	STK->STK_CTRL|=(1<<STK_TICKINT_BIT);/*Enable the SysTick interrupt*/


	return Local_EroorStatus;
}
/*********************************************MSTK_SetSTKCallBack Implementation****************************************/
tenu_ErrorStatus MSTK_SetSTKCallBack(STK_CBF_t Fptr){
	tenu_ErrorStatus Local_EroorStatus=LBTY_OK;
	if(Fptr==NULL)
	{
		Local_EroorStatus=LBTY_ErrorNullPointer;
	}
	else
	{
		CallBack=Fptr;                      /*Set the callback function*/
	}
	return Local_EroorStatus;
}
/*********************************************MSTK_SetBusyWaitMS Implementation*********************************/

tenu_ErrorStatus MSTK_SetBusyWaitMS(u32 Copy_MSTime){
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(Copy_MSTime>STK_MAX_TIME_MS)
	{
		Local_ErrorStatus=LBTY_ErrorInvalidInput;
	}
	else
	{
		STK->STK_LOAD=((Copy_MSTime*Clock_source)/1000UL)-1;       /*Load the number of ticks (converted from microseconds to milliseconds)*/
		STK->STK_CTRL|=(1<<STK_ENABLE_BIT);  /*Enable the timer*/
		while(((STK->STK_CTRL>>STK_COUNT_FLAG_BIT)&0X01)==0); /*Wait until the count flag is set*/
		STK->STK_CTRL&=(~(1<<STK_ENABLE_BIT));                /*Disable the timer*/

	}

	return Local_ErrorStatus;

}
/*********************************************MSTK_SetIntervalSingleMS Implementation*********************************/

tenu_ErrorStatus MSTK_SetIntervalSingleMS(u32 Copy_MSTime, STK_CBF_t Fptr){
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(Fptr==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	else if(Copy_MSTime>STK_MAX_TIME_MS)
	{
		Local_ErrorStatus=LBTY_ErrorInvalidInput;
	}
	else
	{
		STK_Periodic=0;                     /*Set the interval as single*/
		STK->STK_LOAD=((Copy_MSTime*Clock_source)/1000UL)-1;      /*Load the number of ticks (converted to microseconds)*/
		CallBack=Fptr;                      /*Set the callback function*/
		STK->STK_CTRL|=(1<<STK_ENABLE_BIT); /*Enable the timer*/
		STK->STK_CTRL|=(1<<STK_TICKINT_BIT);/*Enable the SysTick interrupt*/
	}

	return Local_ErrorStatus;

}
/*********************************************MSTK_SetIntervalPeriodicMS Implementation*********************************/

tenu_ErrorStatus MSTK_SetIntervalPeriodicMS(u32 Copy_MSTime, STK_CBF_t Fptr ){
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(Fptr==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	else if(Copy_MSTime>STK_MAX_TIME_MS)
	{
		Local_ErrorStatus=LBTY_ErrorInvalidInput;
	}
	else
	{
		STK_Periodic=1;                     /*Set the interval as periodic*/
		STK->STK_LOAD=((Copy_MSTime*Clock_source)/1000)-1;      /*Load the number of ticks (converted to microseconds)*/
		CallBack=Fptr;                      /*Set the callback function*/
		STK->STK_CTRL|=(1<<STK_ENABLE_BIT); /*Enable the timer*/
		STK->STK_CTRL|=(1<<STK_TICKINT_BIT);/*Enable the SysTick interrupt*/
	}

	return Local_ErrorStatus;

}
/*********************************************MSTK_StopInterval Implementation**************************************/

void MSTK_StopInterval(void){

	STK->STK_CTRL&=(~(1<<STK_ENABLE_BIT));

}
tenu_ErrorStatus  MSTK_GetElapsedTimeMS(u32 *ADD_ElasedTime){
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(ADD_ElasedTime==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	else
	{
		/*Calculate the elapsed time in milliseconds by subtracting STK_VAL from STK_LOAD, then dividing by 1000*/
		*ADD_ElasedTime=(((STK->STK_LOAD-STK->STK_VAL)/Clock_source)*1000)+1; /*to convert to ms*/

	}

	return Local_ErrorStatus;

}
/*********************************************MSTK_GetRemainingTimeMS Implementation*********************************/

tenu_ErrorStatus  MSTK_GetRemainingTimeMS(u32 *ADD_RemainingTime){
	tenu_ErrorStatus Local_ErrorStatus=LBTY_OK;
	if(ADD_RemainingTime==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	else
	{
		/*Calculate the remaining time in milliseconds by dividing STK_VAL by 1000*/
		*ADD_RemainingTime=((STK->STK_VAL/Clock_source)*1000)+1; /*to convert to ms*/
	}

	return Local_ErrorStatus;

}

/**
 * @brief SysTick Timer Interrupt Handler.
 *
 * This function is the interrupt handler for the SysTick timer. It is automatically called when the SysTick
 * timer reaches zero, indicating the end of the programmed interval. The behavior of the handler depends on
 * whether the interval is periodic or single.
 *
 * For periodic intervals, it executes the user-defined callback function (if set) and keeps the timer running.
 * For a single interval, it resets the SysTick timer by clearing the LOAD and VAL registers.
 */
void SysTick_Handler(void)
{

    CallBack();  // Execute the user-defined callback function (if set) --> ptr() --> user function

    if(STK_Periodic == 0)
    {
        // Single Interval: Reset the SysTick timer by clearing the LOAD and VAL registers
        STK->STK_LOAD = STK_CLR_REG;
        STK->STK_VAL  = STK_CLR_REG;
    }
    else
    {
        // Periodic Interval: Do nothing, as the timer continues to run for the next interval
    }
}
