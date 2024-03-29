
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_TYPES.h"  // Include standard types header file
#include "MUSART/USART.h"  // Include USART module header file

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

// External Interrupt State Structure
typedef struct {
    USART_enu_Enable Transmit;      // Transmission Enable
    USART_enu_Enable TX_DR_Empty;  // Transmitter Data Register Empty
    USART_enu_Enable RX_DR_Empty;  // Receiver Data Register Empty
} Interrupt_State;

// Definition of USART register bits and values
#define F_CLK                16000000   // System clock frequency
#define USART_NUMBERS        3          // Number of USART channels in use
#define OVERSAMPLING_BIT     15         // Bit position for oversampling control
#define WORDLENGTH_BIT       12         // Bit position for word length control
#define MANTISSA_BIT         4          // Bit position for mantissa in baud rate calculation
#define FRACTION_MSK         0x0F       // Mask for extracting fraction bits in baud rate calculation
#define USART_ENABLE_BIT     13         // Bit position for USART enable control
#define PARITY_CONTROL_BIT   10         // Bit position for parity control
#define TX_DATA_EMPTY_BIT    7          // Bit position for transmitter data empty interrupt enable
#define TRANSMIT_COMPLETE_BIT 6         // Bit position for transmit complete interrupt enable
#define RX_DATA_NOT_EMPTY_BIT 5         // Bit position for receiver data not empty interrupt enable
#define TX_ENABLE_BIT        3          // Bit position for transmitter enable control
#define RX_ENABLE_BIT        2          // Bit position for receiver enable control
#define USART_1              0          // USART channel 1 index
#define USART_2              1          // USART channel 2 index
#define USART_6              2          // USART channel 6 index

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

// Definition of USART register structure
typedef struct {
    volatile u32 SR;   // Status register
    volatile u32 DR;   // Data register
    volatile u32 BRR;  // Baud rate register
    volatile u32 CR1;  // Control register 1
    volatile u32 CR2;  // Control register 2
    volatile u32 CR3;  // Control register 3
    volatile u32 GTPR; // Guard time and prescaler register
} USART_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

// Array to store callback functions for USART modes
static CallBack USART_pvCallBackFunc[16] = {NULL};

// Array to store flags indicating the busy state of USART channels during transmission
static u8 Uart_prvTX_BuzyFlag[USART_NUMBERS];

// Array to store the index of the current byte being transmitted for each USART channel
static u8 Uart_prvTX_BufferIndex[USART_NUMBERS];

// Array to store the size of the data buffer being transmitted for each USART channel
static u8 Uart_prvTX_BufferSize[USART_NUMBERS];

// Array to store pointers to the data buffer being transmitted for each USART channel
static u8 *Uart_prvTX_BufferSend[USART_NUMBERS];

// Interrupt state structure for USART
static Interrupt_State USART_Interrupt;

// Array to store flags indicating the busy state of USART channels during reception
static u8 Uart_prvRx_BuzyFlag[USART_NUMBERS];

// Array to store the index of the current byte being received for each USART channel
static u8 Uart_prvRx_BufferIndex[USART_NUMBERS];

// Array to store the size of the data buffer being received for each USART channel
static u8 Uart_prvRx_BufferSize[USART_NUMBERS];

// Array to store pointers to the data buffer being received for each USART channel
u8 *Uart_prvRx_BufferReceive[USART_NUMBERS];

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

// Function prototype for determining USART channel index
static tenu_ErrorStatus USART_InputUsart(void *USART_channel, u8 *Channel_idx);


/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

tenu_ErrorStatus USART_Init(const USART_strCfg_t* ConfigPtr)
{
    tenu_ErrorStatus Local_ErrorStatus = LBTY_NOK;
    u32 Local_TempValue=0;
	u16	Local_TempMantissa=0;
	u16 Local_TempFraction=0;

    if (ConfigPtr==NULL)
    {
        Local_ErrorStatus =LBTY_ErrorNullPointer;
    }
	else
	{
		Local_TempValue=((u64)F_CLK*1000)/(ConfigPtr->BaudRate*(8*(2-ConfigPtr->Oversampling)));
		Local_TempFraction=(Local_TempValue%1000)*(8*(2-ConfigPtr->Oversampling));
		Local_TempFraction/=1000;
		Local_TempMantissa=Local_TempValue/1000;
		if(Local_TempFraction>0xf)
		{
			Local_TempMantissa+=(Local_TempFraction&0xf0);
		}
		//0x4E2

		((USART_t*)(ConfigPtr->pUartInstance))->BRR=(Local_TempMantissa<<MANTISSA_BIT)|(Local_TempFraction&FRACTION_MSK);
		((USART_t*)(ConfigPtr->pUartInstance))->CR1=0;
		((USART_t*)(ConfigPtr->pUartInstance))->CR1|=ConfigPtr->Oversampling<<OVERSAMPLING_BIT;
		((USART_t*)(ConfigPtr->pUartInstance))->CR1|=ConfigPtr->Word_bits<<WORDLENGTH_BIT;
		((USART_t*)(ConfigPtr->pUartInstance))->CR1|=ConfigPtr->ParityControl<<PARITY_CONTROL_BIT;
		//((USART_t*)(ConfigPtr->pUartInstance))->CR1|=ConfigPtr->ParitySelection<<PARITY_SELECTION_BIT;
		((USART_t*)(ConfigPtr->pUartInstance))->CR1|=ConfigPtr->ReceiverControl<<RX_ENABLE_BIT;
		((USART_t*)(ConfigPtr->pUartInstance))->CR1|=ConfigPtr->TransmitterControl<<TX_ENABLE_BIT;
		
		USART_Interrupt.Transmit = ConfigPtr->TXCE_Enable << TRANSMIT_COMPLETE_BIT;
		USART_Interrupt.TX_DR_Empty = ConfigPtr->TXE_Enable << TX_DATA_EMPTY_BIT;
		USART_Interrupt.RX_DR_Empty = ConfigPtr->RXNE_Enable << RX_DATA_NOT_EMPTY_BIT;

		
		((USART_t*)(ConfigPtr->pUartInstance))->CR1|=ConfigPtr->UartEnable<<USART_ENABLE_BIT;

		((USART_t*)(ConfigPtr->pUartInstance))->SR=0;


	}
    return Local_ErrorStatus;    

}
/****************************************************************************************************/
tenu_ErrorStatus USART_SendBytesynchronous(void* Channel, u8 Copy_Data)
{
	tenu_ErrorStatus Local_ErrorStatus = LBTY_NOK;
	
	if(Channel==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}

	else
	{

		((USART_t*)Channel)->DR=Copy_Data;

		while (((((USART_t*)Channel)->SR >> 6)&0x1)== 0);
		
		Local_ErrorStatus=LBTY_OK;

	}

	 return Local_ErrorStatus; 
}
/***************************************************************************************************/
tenu_ErrorStatus USART_ReceiveBytesynchronous(void* Channel, u8 * Copy_Data)
{
	tenu_ErrorStatus Local_ErrorStatus = LBTY_NOK;
			
	if(Channel==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}

	else
	{

		while (((((USART_t*)Channel)->SR >> 5)&0x1)== 0);
		*Copy_Data=((USART_t*)Channel)->DR;
		

	}

	 return Local_ErrorStatus; 

}

/***************************************************************************************************/
tenu_ErrorStatus USART_SendByteAsynchronous(void* Channel, u8 Copy_Data)
{
	tenu_ErrorStatus Local_ErrorStatus = LBTY_NOK;
	u8 Local_ChannelIdx=0;


	if(Channel==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	
	else
	{
		Local_ErrorStatus=USART_InputUsart(Channel,&Local_ChannelIdx);
		if(Uart_prvTX_BuzyFlag[Local_ChannelIdx]==USART_BUSY)
		{
			Local_ErrorStatus=LBTY_Busy;
		}
		else
		{
			((USART_t*)Channel)->DR=Copy_Data;
			Uart_prvTX_BuzyFlag[Local_ChannelIdx] = USART_BUSY;
			Uart_prvTX_BufferIndex[Local_ChannelIdx] = 1;
			Uart_prvTX_BufferSize[Local_ChannelIdx] = 1;
			((USART_t*)Channel)->CR1 |= USART_Interrupt.Transmit;
		}
		
	}
	

	 return Local_ErrorStatus; 
}
/*****************************************************************************************************/

tenu_ErrorStatus USART_ReceiveBuffer(USART_RXBuffer * ReceiveBuffer)
{
	tenu_ErrorStatus Local_ErrorStatus = LBTY_NOK;
	u8 Local_ChannelIdx =0;
	if(ReceiveBuffer==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	else
	{
		Local_ErrorStatus=USART_InputUsart(ReceiveBuffer->Channel,&Local_ChannelIdx);
	
		if(Uart_prvRx_BuzyFlag[Local_ChannelIdx]==USART_BUSY)
		{
			Local_ErrorStatus=LBTY_Busy;
		}
		else
		{
			Uart_prvRx_BuzyFlag[Local_ChannelIdx] = USART_BUSY;
			Uart_prvRx_BufferReceive[Local_ChannelIdx]=(u8 *)&(ReceiveBuffer->Data);
			Uart_prvRx_BufferIndex[Local_ChannelIdx] = ReceiveBuffer->Index;
			Uart_prvRx_BufferSize[Local_ChannelIdx] = ReceiveBuffer->Size;
			((USART_t*)ReceiveBuffer->Channel)->CR1 |= USART_Interrupt.RX_DR_Empty;
		}

	}

	 return Local_ErrorStatus; 
}
/******************************************************************************************************************/
tenu_ErrorStatus USART_SendBufferZeroCopy(USART_TXBuffer* Copy_ConfigBuffer)
{
	tenu_ErrorStatus Local_ErrorStatus = LBTY_NOK;
	u8 Local_ChannelIdx=0;

	Local_ErrorStatus=USART_InputUsart(Copy_ConfigBuffer->Channel,&Local_ChannelIdx);
	if(Uart_prvTX_BuzyFlag[Local_ChannelIdx]==USART_BUSY)
	{
		Local_ErrorStatus=LBTY_Busy;
	}
	else
	{
		Uart_prvTX_BufferSend[Local_ChannelIdx]=Copy_ConfigBuffer->Data;
	}

	if(Copy_ConfigBuffer==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	else
	{
	
		((USART_t*)Copy_ConfigBuffer->Channel)->DR=Copy_ConfigBuffer->Data[0];
		Uart_prvTX_BuzyFlag[Local_ChannelIdx] = USART_BUSY;
		Uart_prvTX_BufferIndex[Local_ChannelIdx] = 1;
		Uart_prvTX_BufferSize[Local_ChannelIdx] = Copy_ConfigBuffer->Size;
		((USART_t*)Copy_ConfigBuffer->Channel)->CR1 |= USART_Interrupt.Transmit;
		

	}

	 return Local_ErrorStatus; 
}
tenu_ErrorStatus USART_RegisterCallBackFunction( USART_Mode Mode, CallBack CallBackFunction)
{
	tenu_ErrorStatus Local_ErrorStatus = LBTY_NOK;
	if (CallBackFunction==NULL)
	{
		Local_ErrorStatus=LBTY_ErrorNullPointer;
	}
	else
	{
		USART_pvCallBackFunc[Mode]=CallBackFunction;

	}

	 return Local_ErrorStatus; 
}
tenu_ErrorStatus USART_InputUsart(void * USART_channel,u8 * Channel_idx)
{
	    tenu_ErrorStatus Loc_ErrorStatus= LBTY_OK;
 		if(USART_channel==USART1)
		{
			*Channel_idx=USART_1;

		}
		else if(USART_channel==USART2)
		{
			*Channel_idx=USART_2;

		}
		else if(USART_channel==USART6)
		{
			*Channel_idx=USART_6;

		}
		else
		{
			Loc_ErrorStatus= LBTY_ErrorInvalidInput;
		}

	return Loc_ErrorStatus;	

}
/***********************Handler Function******************************/

void USART1_IRQHandler(void)
{
		

		/*Read transmitting flag*/
		if((((USART_t*)USART1)->SR >> TRANSMIT_COMPLETE_BIT) & 0x01)
		{
			if(Uart_prvTX_BufferIndex[USART_1] == Uart_prvTX_BufferSize[USART_1])
			{
				/*clear Buzy Tx flag*/
				Uart_prvTX_BuzyFlag[USART_1] = USART_IDLE;
				/*clear Tx Buffer Size*/
				Uart_prvTX_BufferSize[USART_1] = 0;
				if(USART_pvCallBackFunc[UART1_SEND])
				{
					USART_pvCallBackFunc[UART1_SEND]();
				}
	
				/*Clear Tc Flag*/
				((USART_t*)USART1)->SR &= ~(1 << TRANSMIT_COMPLETE_BIT);

			}/*end of if*/
			else
			{
				((USART_t*)USART1)->SR &= ~(1 << TRANSMIT_COMPLETE_BIT);
				((USART_t*)USART1)->DR = Uart_prvTX_BufferSend[USART_1][Uart_prvTX_BufferIndex[USART_1]];
				Uart_prvTX_BufferIndex[USART_1]++;

			}/*end of else*/

		}/*end of if*/
	


		if(Uart_prvRx_BuzyFlag[USART_1] == USART_BUSY && (((((USART_t*)USART1)->SR >> RX_DATA_NOT_EMPTY_BIT) & 0x01)))
		{
			Uart_prvRx_BufferReceive[USART_1][Uart_prvRx_BufferIndex[USART_1]] = ((USART_t*)USART1)->DR;
			Uart_prvRx_BufferIndex[USART_1]++;
			if(Uart_prvRx_BufferSize[USART_1] == Uart_prvRx_BufferIndex[USART_1])
			{
				((USART_t*)USART1)->SR &= ~(1 << RX_DATA_NOT_EMPTY_BIT);
				Uart_prvRx_BuzyFlag[USART_1] = USART_IDLE;
				Uart_prvRx_BufferSize[USART_1] = 0;
				if(USART_pvCallBackFunc[UART1_RECEIVE])
				{
					USART_pvCallBackFunc[UART1_RECEIVE]();
				}
				
			}
			
		}
	
}/*end of function USART1_IRQHandler*/



void USART2_IRQHandler(void)
{
	
		/*Read transmitting flag*/
		if((((USART_t*)USART2)->SR >> TRANSMIT_COMPLETE_BIT) & 0x01)
		{
			if(Uart_prvTX_BufferIndex[USART_2] == Uart_prvTX_BufferSize[USART_2])
			{
				/*clear Buzy Tx flag*/
				Uart_prvTX_BuzyFlag[USART_2] = USART_IDLE;
				/*clear Tx Buffer Size*/
				Uart_prvTX_BufferSize[USART_2] = 0;
				if(USART_pvCallBackFunc[UART2_SEND])
				{
					USART_pvCallBackFunc[UART2_SEND]();
				}

				/*Clear Tc Flag*/
				((USART_t*)USART1)->SR &= ~(1 << TRANSMIT_COMPLETE_BIT);

			}/*end of if*/
			else
			{
				((USART_t*)USART2)->SR &= ~(1 << TRANSMIT_COMPLETE_BIT);
				((USART_t*)USART2)->DR = Uart_prvTX_BufferSend[USART_2][Uart_prvTX_BufferIndex[USART_2]];
				Uart_prvTX_BufferIndex[USART_2]++;

			}/*end of else*/

		}/*end of if*/
	

	
		if(Uart_prvRx_BuzyFlag[USART_2] == USART_BUSY && ((((USART_t*)USART2)->SR >> RX_DATA_NOT_EMPTY_BIT) & 0x01))
		{
			Uart_prvRx_BufferReceive[USART_2][Uart_prvRx_BufferIndex[USART_2]] = ((USART_t*)USART2)->DR;
			Uart_prvRx_BufferIndex[USART_2]++;
			if(Uart_prvRx_BufferSize[USART_2] == Uart_prvRx_BufferIndex[USART_2])
			{
				((USART_t*)USART2)->SR &= ~(1 << RX_DATA_NOT_EMPTY_BIT);
				Uart_prvRx_BuzyFlag[USART_2] = USART_IDLE;
				Uart_prvRx_BufferSize[USART_2] = 0;
				if(USART_pvCallBackFunc[UART2_RECEIVE])
				{
					USART_pvCallBackFunc[UART2_RECEIVE]();
				}
	
				
			}
			
		}
	
}/*end of function USART2_IRQHandler*/

void USART6_IRQHandler(void)
{

		/*Read transmitting flag*/
		if((((USART_t*)USART6)->SR >> TRANSMIT_COMPLETE_BIT) & 0x01)
		{
			if(Uart_prvTX_BufferIndex[USART_6] == Uart_prvTX_BufferSize[USART_6])
			{
				/*clear Buzy Tx flag*/
				Uart_prvTX_BuzyFlag[USART_6] = USART_IDLE;
				/*clear Tx Buffer Size*/
				Uart_prvTX_BufferSize[USART_6] = 0;
				if(USART_pvCallBackFunc[UART6_SEND])
				{
					USART_pvCallBackFunc[UART6_SEND]();
				}
	
				/*Clear Tc Flag*/
				((USART_t*)USART6)->SR &= ~(1 << TRANSMIT_COMPLETE_BIT);

			}/*end of if*/
			else
			{
				((USART_t*)USART6)->SR &= ~(1 << TRANSMIT_COMPLETE_BIT);
				((USART_t*)USART6)->DR = Uart_prvTX_BufferSend[USART_6][Uart_prvTX_BufferIndex[USART_6]];
				Uart_prvTX_BufferIndex[USART_6]++;

			}/*end of else*/

		}/*end of if*/
	


		if(Uart_prvRx_BuzyFlag[USART_6] == USART_BUSY && ((((USART_t*)USART6)->SR >> RX_DATA_NOT_EMPTY_BIT) & 0x01))
		{
			Uart_prvRx_BufferReceive[USART_6][Uart_prvRx_BufferIndex[USART_6]] = ((USART_t*)USART6)->DR;
			Uart_prvRx_BufferIndex[USART_6]++;
			if(Uart_prvRx_BufferSize[USART_6] == Uart_prvRx_BufferIndex[USART_6])
			{
				((USART_t*)USART6)->SR &= ~(1 << RX_DATA_NOT_EMPTY_BIT);
				Uart_prvRx_BuzyFlag[USART_6] = USART_IDLE;
				Uart_prvRx_BufferSize[USART_6] = 0;
			if(USART_pvCallBackFunc[UART6_RECEIVE])
			{
				USART_pvCallBackFunc[UART6_RECEIVE]();
			}
	

			}
			
		}
	
}/*end of function USART2_IRQHandler*/
