#ifndef STM32F401CC_IRQ_
#define STM32F401CC_IRQ_
#include "STD_TYPES.h"
#define  NVIC_INTERRUPTS_CODE         0xF00000000

/**
 * @brief Enumeration of Interrupt Request (IRQ) IDs for NVIC (Nested Vectored Interrupt Controller).
 *
 * This enumeration defines symbolic names for different interrupt sources in a system. Each symbol corresponds
 * to a specific interrupt source, allowing for more readable and maintainable code when configuring interrupts.
 * The values represent the interrupt numbers as specified by the NVIC (Nested Vectored Interrupt Controller).
 * Note: Additional interrupt sources can be added or modified based on the specific microcontroller or system.
 */
typedef enum
{
    NVIC_IRQ_WWDG,               /*!< Window Watchdog interrupt */
    NVIC_IRQ_EXTI16,              /*!< External Interrupt Line 16 */
    NVIC_IRQ_EXTI21,              /*!< External Interrupt Line 21 */
    NVIC_IRQ_EXTI22,              /*!< External Interrupt Line 22 */
    NVIC_IRQ_FLASH,               /*!< Flash global interrupt */
    NVIC_IRQ_RCC,                 /*!< RCC global interrupt */
    NVIC_IRQ_EXTI0,               /*!< External Interrupt Line 0 */
    NVIC_IRQ_EXTI1,               /*!< External Interrupt Line 1 */
    NVIC_IRQ_EXTI2,               /*!< External Interrupt Line 2 */
    NVIC_IRQ_EXTI3,               /*!< External Interrupt Line 3 */
    NVIC_IRQ_EXTI4,               /*!< External Interrupt Line 4 */
    NVIC_IRQ_DMA1_STREAM0,        /*!< DMA1 Stream 0 global interrupt */
    NVIC_IRQ_DMA1_STREAM1,        /*!< DMA1 Stream 1 global interrupt */
    NVIC_IRQ_DMA1_STREAM2,        /*!< DMA1 Stream 2 global interrupt */
    NVIC_IRQ_DMA1_STREAM3,        /*!< DMA1 Stream 3 global interrupt */
    NVIC_IRQ_DMA1_STREAM4,        /*!< DMA1 Stream 4 global interrupt */
    NVIC_IRQ_DMA1_STREAM5,        /*!< DMA1 Stream 5 global interrupt */
    NVIC_IRQ_DMA1_STREAM6,        /*!< DMA1 Stream 6 global interrupt */
    NVIC_IRQ_ADC,                 /*!< ADC global interrupt */
    NVIC_IRQ_EXTI9 = 23,          /*!< External Interrupt Line 9 */
    NVIC_IRQ_TIM1_BRK_TIM9,       /*!< TIM1 Break interrupt and TIM9 global interrupt */
    NVIC_IRQ_TIM1_UP_TIM10,       /*!< TIM1 Update interrupt and TIM10 global interrupt */
    NVIC_IRQ_TIM1_TRG_COM_TIM11,  /*!< TIM1 Trigger and Commutation interrupts and TIM11 global interrupt */
    NVIC_IRQ_TIM1_CC,             /*!< TIM1 Capture Compare interrupt */
    NVIC_IRQ_TIM2,                /*!< TIM2 global interrupt */
    NVIC_IRQ_TIM3,                /*!< TIM3 global interrupt */
    NVIC_IRQ_TIM4,                /*!< TIM4 global interrupt */
    NVIC_IRQ_I2C1_EV,             /*!< I2C1 event interrupt */
    NVIC_IRQ_I2C1_ER,             /*!< I2C1 error interrupt */
    NVIC_IRQ_I2C2_EV,             /*!< I2C2 event interrupt */
    NVIC_IRQ_I2C2_ER,             /*!< I2C2 error interrupt */
    NVIC_IRQ_SPI1,                /*!< SPI1 global interrupt */
    NVIC_IRQ_SPI2,                /*!< SPI2 global interrupt */
    NVIC_IRQ_USART1,              /*!< USART1 global interrupt */
    NVIC_IRQ_USART2,              /*!< USART2 global interrupt */
    NVIC_IRQ_EXTI15_10 = 40,      /*!< External Interrupt Lines 15 to 10 */
    NVIC_IRQ_EXTI17,              /*!< External Interrupt Line 17 */
    NVIC_IRQ_EXTI18,              /*!< External Interrupt Line 18 */
    NVIC_IRQ_DMA1_STREAM7 = 47,   /*!< DMA1 Stream 7 global interrupt */
    NVIC_IRQ_SDIO = 49,           /*!< SDIO global interrupt */
    NVIC_IRQ_TIM5,                /*!< TIM5 global interrupt */
    NVIC_IRQ_SPI3,                /*!< SPI3 global interrupt */
    NVIC_IRQ_DMA2_STREAM0 = 56,   /*!< DMA2 Stream 0 global interrupt */
    NVIC_IRQ_DMA2_STREAM1,        /*!< DMA2 Stream 1 global interrupt */
    NVIC_IRQ_DMA2_STREAM2,        /*!< DMA2 Stream 2 global interrupt */
    NVIC_IRQ_DMA2_STREAM3,        /*!< DMA2 Stream 3 global interrupt */
    NVIC_IRQ_DMA2_STREAM4,        /*!< DMA2 Stream 4 global interrupt */
    NVIC_IRQ_OTG_FS = 67,         /*!< USB OTG FS global interrupt */
    NVIC_IRQ_DMA2_STREAM5,        /*!< DMA2 Stream 5 global interrupt */
    NVIC_IRQ_DMA2_STREAM6,        /*!< DMA2 Stream 6 global interrupt */
    NVIC_IRQ_DMA2_STREAM7,        /*!< DMA2 Stream 7 global interrupt */
    NVIC_IRQ_USART6,              /*!< USART6 global interrupt */
    NVIC_IRQ_I2C3_EV,             /*!< I2C3 event interrupt */
    NVIC_IRQ_I2C3_ER,             /*!< I2C3 error interrupt */
    NVIC_IRQ_FPU = 81,            /*!< FPU global interrupt */
    NVIC_IRQ_SPI4 = 84,           /*!< SPI4 global interrupt */
    _NVIC_IRQ_NUM                /*!< Total number of NVIC IRQs */
} IRQ_ID_tenu;
#endif // STM32F401CC_IRQ_
