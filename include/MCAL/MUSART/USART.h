#ifndef MCAL_MUSART_USART_H_
#define MCAL_MUSART_USART_H_
/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include"USART_Config.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define BASE_ADDRESS_USART_1         0X40011000
#define BASE_ADDRESS_USART_2         0X40004400
#define BASE_ADDRESS_USART_6         0X40011400  

#define OVERSAMPLING_8				1
#define OVERSAMPLING_16				0

#define USART1       ((void*)BASE_ADDRESS_USART_1)
#define USART2       ((void*)BASE_ADDRESS_USART_2)
#define USART6       ((void*)BASE_ADDRESS_USART_6)


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/*callback function*/
typedef void(*CallBack)(void);
/*Uart Data storage to send*/
typedef struct
{
	u8 * Data;
	u32 Size;
	void* Channel;

}USART_TXBuffer;

/*Uart Data storage to Receive*/
typedef struct
{
	void * Channel;
	u8 * Data;
	u32 Size;
	u32 Index;

}USART_RXBuffer;
typedef enum {
	USART_1StopBit,
	USART_2StopBit,
}USART_enuStopBits;

typedef enum {
	USART_8Bits,
	USART_9Bits,

}USART_enuWordSize;

typedef enum {
    USART_DisableParity,
	USART_EnableParity,	

}USART_enuParityControl;

typedef enum {
    USART_EvenParity,
	USART_OddParity,
	

}USART_enuParitySelection;

typedef enum {
    USART_DisableTX,
	USART_EnableTX,
	
}USART_enuTransmitterControl;

typedef enum {
    USART_DisableRX,
	USART_EnableRX,
	
}USART_enuReceiverControl;


typedef enum {
    USART_Disable,
	USART_Enable,

}USART_enu_Enable;

typedef struct{
    void * pUartInstance;
	USART_enuStopBits Stop_bits;
	USART_enuWordSize Word_bits;
	USART_enuParityControl ParityControl;
	USART_enuParitySelection ParitySelection;
	USART_enuTransmitterControl TransmitterControl;
	USART_enuReceiverControl ReceiverControl;
	USART_enu_Enable TXE_Enable;
	USART_enu_Enable RXNE_Enable;
	USART_enu_Enable TXCE_Enable;
	USART_enu_Enable UartEnable;
	u16 BaudRate;
	u8 Oversampling;
}USART_strCfg_t;

typedef enum
{
	UART1_SEND,
	UART1_RECEIVE,
	UART2_SEND,
	UART2_RECEIVE,
	UART6_SEND,
	UART6_RECEIVE,

}USART_Mode;

typedef enum
{
	USART_IDLE,
	USART_BUSY,
	
}USART_status_tenu;

typedef enum {
	USART_OK,
	USART_BaudRateError,
	USART_WordSizeError,
	USART_ParityControlError,
	USART_ParitySelectionError,
	USART_TransmitterControlError,
	USART_ReceiverControlError,
	USART_ControlEnableError,
	USART_StopBitsError,
	USART_OverSamplingError,
	USART_UsartSelectError,
	USART_NullConfPointer


}USART_enuErrorStatus;


/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
/**
 * @brief Initialize USART channel
 * 
 * @param ConfigPtr Pointer to USART configuration structure
 * @return tenu_ErrorStatus Error status
 */
USART_enuErrorStatus USART_Init(const USART_strCfg_t *ConfigPtr);

/**
 * @brief Send a byte synchronously
 * 
 * @param Channel USART channel
 * @param Copy_Data Byte to be sent
 * @return tenu_ErrorStatus Error status
 */
USART_enuErrorStatus USART_SendBytesynchronous(void *Channel, u8 Copy_Data);

/**
 * @brief Receive a byte synchronously
 * 
 * @param Channel USART channel
 * @param Copy_Data Pointer to store received byte
 * @return tenu_ErrorStatus Error status
 */
USART_enuErrorStatus USART_ReceiveBytesynchronous(void *Channel, u8 *Copy_Data);

/**
 * @brief Send a byte asynchronously
 * 
 * @param Channel USART channel
 * @param Copy_Data Byte to be sent
 * @return tenu_ErrorStatus Error status
 */
USART_enuErrorStatus USART_SendByteAsynchronous(void *Channel, u8 Copy_Data);

/**
 * @brief Receive data into buffer
 * 
 * @param ReceiveBuffer Pointer to receive buffer configuration
 * @return tenu_ErrorStatus Error status
 */
USART_enuErrorStatus USART_ReceiveBufferAsynchronous(USART_RXBuffer *ReceiveBuffer);

/**
 * @brief Send data from buffer with zero-copy
 * 
 * @param Copy_ConfigBuffer Pointer to transmit buffer configuration
 * @return tenu_ErrorStatus Error status
 */
USART_enuErrorStatus USART_SendBufferZeroCopy(USART_TXBuffer *Copy_ConfigBuffer);

/**
 * @brief Register callback function for USART mode
 * 
 * @param Mode USART mode
 * @param CallBackFunction Pointer to callback function
 * @return tenu_ErrorStatus Error status
 */
USART_enuErrorStatus USART_RegisterCallBackFunction(USART_Mode Mode, CallBack CallBackFunction);


USART_enuErrorStatus USART_SendByteSynchByTime(void *Channel, u8 Copy_Data);

USART_enuErrorStatus USART_ReceiveByteSynchByTime(void *Channel, u8 *Copy_Data);

#endif //MCAL_MUSART_USART_H_
