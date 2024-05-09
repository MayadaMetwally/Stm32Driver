#include "STD_TYPES.h"
#include "MRCC/RCC.h"
#include "HLED/LED.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define FREERTOS                1

// Define the periods for each task in milliseconds
#define TASK1_PERIOD 2000
#define TASK2_PERIOD 3000
#define TASK3_PERIOD 5000


#if FREERTOS
// Function prototypes for tasks
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);


// Entry point
int main(void) {
  /// enable GPIOB periph clock for led and UART TX & RX

	MRCC_ControlClockAHP1Peripheral(RCC_AHB1_GPIOA,RCC_ENABLE);
	MRCC_ControlClockAHP1Peripheral(RCC_AHB1_GPIOB,RCC_ENABLE);

	 //LED INIT
    HLED_Init();


    // Create task handles
    TaskHandle_t xTask1Handle, xTask2Handle, xTask3Handle;

    // Create task 1
    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, 3, &xTask1Handle);

    // Create task 2
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, 2 , &xTask2Handle);

    // Create task 3
    xTaskCreate(vTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, 1 , &xTask3Handle);

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach here
    for (;;);
    return 0;
}

// Task 1 - Toggle RED LED every 2 seconds
void vTask1(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK1_PERIOD);
    for (;;) {
        // Toggle LED here
    	HLED_Toggle(LED_GREEN);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

// Task 2 - Toggle GREEN LED every 4 seconds
void vTask2(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK2_PERIOD);
    for (;;) {
        // Toggle LED here
    	HLED_Toggle(LED_YELLOW);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

// Task 3 - Toggle YELLOW LED every 6 seconds
void vTask3(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_PERIOD);
    for (;;) {
        // Toggle LED here
      
        HLED_Toggle(LED_RED);
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}


#endif
