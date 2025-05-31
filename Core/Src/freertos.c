/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LED_spark */
osThreadId_t LED_sparkHandle;
const osThreadAttr_t LED_spark_attributes = {
  .name = "LED_spark",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityRealtime5,
};
/* Definitions for motor_test */
osThreadId_t motor_testHandle;
const osThreadAttr_t motor_test_attributes = {
  .name = "motor_test",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal7,
};
/* Definitions for LED */
osSemaphoreId_t LEDHandle;
const osSemaphoreAttr_t LED_attributes = {
  .name = "LED"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void LED_onAndOff(void *argument);
void motor_test_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  printf("FreeRTOS Init starting...\r\n");
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of LED */
  LEDHandle = osSemaphoreNew(1, 1, &LED_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LED_spark */
  LED_sparkHandle = osThreadNew(LED_onAndOff, NULL, &LED_spark_attributes);

  /* creation of motor_test */
  motor_testHandle = osThreadNew(motor_test_Task, NULL, &motor_test_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  printf("Default task started\r\n");
  /* Infinite loop */
  for (;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LED_onAndOff */
/**
 * @brief Function implementing the LED_spark thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_LED_onAndOff */
void LED_onAndOff(void *argument)
{
  /* USER CODE BEGIN LED_onAndOff */
  printf("LED task started\r\n");
  /* Infinite loop */
  for (;;)
  {
    led1_on();
    vTaskDelay(1000);
    led1_off();
    vTaskDelay(1000);
    led2_on();
    vTaskDelay(1000);
    led2_off();
    vTaskDelay(1000);
    led3_on();
    vTaskDelay(1000);
    led3_off();
    vTaskDelay(1000);
  }
  /* USER CODE END LED_onAndOff */
}

/* USER CODE BEGIN Header_motor_test_Task */
/**
* @brief Function implementing the motor_test thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_motor_test_Task */
void motor_test_Task(void *argument)
{
  /* USER CODE BEGIN motor_test_Task */
  /* Infinite loop */
  for(;;)
  {

  }
  /* USER CODE END motor_test_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

