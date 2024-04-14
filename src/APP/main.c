#include "STD_TYPES.h"
#include "MRCC/RCC.h"
#include "MGPIO/GPIO.h"
#include "MNVIC/MNVIC.h"
#include "HLED/LED.h"
#include "HSWITCH/SWITCH.h"
#include "SERVICE/SCHED/SCHED.h"
#include "HCLCD/LCD.h"
#include "HKPD/KYD.h"
#include "MUSART/USART.h"

//extern USART_RXBuffer rx_buff;

int main()
{
    u8 Loc_Smile[8]=HCLCD_SMILY_FACE;
    MRCC_ControlClockAHP1Peripheral(RCC_AHB1_GPIOA,RCC_ENABLE);
    MRCC_ControlClockAHP1Peripheral(RCC_AHB1_GPIOB,RCC_ENABLE);
    MRCC_ControlClockABP2Peripheral(RCC_APB2_USART1,RCC_ENABLE);
    MNVIC_EnableInterrupt(NVIC_IRQ_USART1);
    USART_strCfg_t Usart1_Config;
    Usart1_Config.BaudRate=9600;
    Usart1_Config.Oversampling=OVERSAMPLING_16;
    Usart1_Config.ParityControl=USART_DisableParity;
    Usart1_Config.pUartInstance=USART1;
    Usart1_Config.ReceiverControl=USART_EnableRX;
    Usart1_Config.RXNE_Enable=USART_Enable;
    Usart1_Config.Stop_bits=USART_1StopBit;
    Usart1_Config.TransmitterControl=USART_EnableTX;
    Usart1_Config.TXCE_Enable=USART_Enable;
    Usart1_Config.TXE_Enable=USART_Enable;
    Usart1_Config.UartEnable=USART_Enable;
    Usart1_Config.Word_bits=USART_8Bits;

    GPIO_Pin_tstr TX_PIN = 
	{
		.Mode = GPIO_MODE_AF_PP,
		.Speed = GPIO_SPEED_HIGH,
		.Port = GPIOA,
		.Pin = GPIO_PIN_9,
		.AF = GPIO_AF_USART1_2
	};

	GPIO_Pin_tstr RX_PIN = 
	{
		.Mode = GPIO_MODE_AF_PP_PU,
		.Speed = GPIO_SPEED_HIGH,
		.Port = GPIOA,
		.Pin = GPIO_PIN_10,
		.AF = GPIO_AF_USART1_2
	};

	MGPIO_InitPinAF(&TX_PIN);
	MGPIO_InitPinAF(&RX_PIN);
    CLCD_InitAsynch();
    CLCD_WriteSpecialCharAsynch(&Loc_Smile,1);
    KPD_INIT();
    //HLED_Init();
	USART_Init(&Usart1_Config);
    //USART_ReceiveBufferAsynchronous(&rx_buff);
	//USART_RegisterCallBackFunction(UART1_RECEIVE,Receive_RunnableFunc);
    Sched_Init();
    StartSched();
	


}

// ----------------------------------------------------------------------------