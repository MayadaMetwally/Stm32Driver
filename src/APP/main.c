#include "STD_TYPES.h"
#include "MRCC/RCC.h"
#include "MGPIO/GPIO.h"
#include "MNVIC/MNVIC.h"
#include "HLED/LED.h"
#include "HSWITCH/SWITCH.h"
//#include "SERVICE/SCHED/SCHED.h"
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
	u8 Loc_Status=0;
	u8 Loc_pinState=1;

	if(rx_buff.Data == 'S')
	{
		MGPIO_GetPinState(GPIOA,GPIO_PIN_3,&Loc_pinState);
		if(Loc_pinState==GPIO_High)
		{
			USART_SendByteAsynchronous(USART1,'H');
		}
		else if(Loc_pinState==GPIO_Low)
		{
			USART_SendByteAsynchronous(USART1,'L');
		}

	}
	
	else if (rx_buff.Data == 'O')
	{
		
		HLED_SetStatus(LED_RED,LED_ON);
		HLED_GetStatus(LED_RED,&Loc_Status);
		if(Loc_Status==LED_ON)
		{
			USART_SendByteAsynchronous(USART1,'O');
		}
		else if(Loc_Status==LED_OFF)
		{
			USART_SendByteAsynchronous(USART1,'F');
		}

		
	}
	else if (rx_buff.Data == 'F')
	{
		
		HLED_SetStatus(LED_RED,LED_OFF);
		HLED_GetStatus(LED_RED,&Loc_Status);
		if(Loc_Status==LED_ON)
		{
			USART_SendByteAsynchronous(USART1,'O');
		}
		else if(Loc_Status==LED_OFF)
		{
			USART_SendByteAsynchronous(USART1,'F');
		}
	}
	else
	{
		
		HLED_Toggle(LED_YELLOW);
	}
}





int main()
{
  
    RCC_ControlPeripheralClock(Peri_GPIOA,PeriStatus_Enable,PowerMode_Normal);
    RCC_ControlPeripheralClock(Peri_GPIOB,PeriStatus_Enable,PowerMode_Normal);
    RCC_ControlPeripheralClock(Peri_USART1,PeriStatus_Enable,PowerMode_Normal);
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
	HSWITCH_Init();
	MNVIC_Init();
    HLED_Init();
	USART_Init(&Usart1_Config);

	
	USART_RegisterCallBackFunction(UART1_RECEIVE,recieve_callback);
	
    while (1)
    {
       USART_ReceiveBufferAsynchronous(&rx_buff); 
    }
    
    /* Sched_init();
    Sched_Start();
	 */


}

// ----------------------------------------------------------------------------