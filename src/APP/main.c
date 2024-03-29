#include "STD_TYPES.h"
#include "MRCC/RCC.h"
#include "MGPIO/GPIO.h"
#include "MNVIC/MNVIC.h"
#include "HLED/LED.h"
#include "HSWITCH/SWITCH.h"
#include "SERVICE/SCHED/SCHED.h"
#include "HCLCD/LCD.h"
#include "MUSART/USART.h"


USART_RXBuffer rx_buff = 
{
	.Channel = USART1,
	.Size = 1,
	.Index = 0
};

USART_TXBuffer tx_buff = 
{
	.Channel = USART1,
	.Data = " Hello my friend ",
	.Size = 17
};

USART_TXBuffer tx_buff2 = 
{
	.Channel = USART1,
	.Data = " take care ",
	.Size = 11
};
USART_TXBuffer tx_buff3 = 
{
	.Channel = USART1,
	.Data = " Bye my friend ",
	.Size = 15
};
void recieve_callback(void)
{
	if (rx_buff.Data == 'M')
	{
		USART_SendBufferZeroCopy(&tx_buff);
		HLED_Toggle(LED_RED2);
	}
	else if (rx_buff.Data == 'm')
	{
		USART_SendBufferZeroCopy(&tx_buff2);
		HLED_Toggle(LED_RED);
	}
	else
	{
		USART_SendBufferZeroCopy(&tx_buff3);
		HLED_Toggle(LED_YELLOW);
	}
}

int main()
{
    MRCC_ControlClockAHP1Peripheral(RCC_AHB1_GPIOA,RCC_ENABLE);
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
    HLED_Init();
	USART_Init(&Usart1_Config);
	USART_RegisterCallBackFunction(UART1_RECEIVE,recieve_callback);
	


	
    

    //u8 loc_r1=0;
	while (1)
	{
		USART_ReceiveBuffer(&rx_buff);
		/* USART_SendBytesynchronous(USART1,'M');
		USART_SendBytesynchronous(USART1,'A');
		USART_SendBytesynchronous(USART1,'Y');
		USART_SendBytesynchronous(USART1,'A');
		USART_SendBytesynchronous(USART1,'D');
		USART_SendBytesynchronous(USART1,'A');
		USART_ReceiveBytesynchronous(USART1,&loc_r1);
		if(loc_r1=='1')
		{
			HLED_Toggle(LED_RED);
		}
		else if(loc_r1=='2')
		{
			HLED_Toggle(LED_RED2);
		}
		USART_SendBytesynchronous(USART1,'D');  */
		




	}

return 0;

}

// ----------------------------------------------------------------------------
