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
#include "Emm_V5.h"
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
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal,
};
/* Definitions for LED_spark */
osThreadId_t LED_sparkHandle;
const osThreadAttr_t LED_spark_attributes = {
    .name = "LED_spark",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime5,
};
/* Definitions for motor_test */
osThreadId_t motor_testHandle;
const osThreadAttr_t motor_test_attributes = {
    .name = "motor_test",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityNormal7,
};
/* Definitions for get_print_PWM */
osThreadId_t get_print_PWMHandle;
const osThreadAttr_t get_print_PWM_attributes = {
    .name = "get_print_PWM",
    .stack_size = 128 * 4,
    .priority = (osPriority_t)osPriorityRealtime3,
};
/* Definitions for PWM_Value */
osMessageQueueId_t PWM_ValueHandle;
const osMessageQueueAttr_t PWM_Value_attributes = {
    .name = "PWM_Value"};
/* Definitions for LED */
osSemaphoreId_t LEDHandle;
const osSemaphoreAttr_t LED_attributes = {
    .name = "LED"};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void LED_onAndOff(void *argument);
void motor_test_Task(void *argument);
void get_and_print_PWM_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void)
{
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

  /* Create the queue(s) */
  /* creation of PWM_Value */
  PWM_ValueHandle = osMessageQueueNew(2, sizeof(uint16_t), &PWM_Value_attributes);

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

  /* creation of get_print_PWM */
  get_print_PWMHandle = osThreadNew(get_and_print_PWM_Task, NULL, &get_print_PWM_attributes);

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
  for (;;)
  {
    // 获取消息队列中的值
    uint16_t pwm_value;
    osMessageQueueGet(PWM_ValueHandle, &pwm_value, NULL, 3);
    Emm_V5_Pos_Control(1, 0, 1000, 0, pwm_value*10, true, false); // 正转
    osMessageQueueReset(PWM_ValueHandle); // 重置消息队列
    osDelay(3);
  }
  /* USER CODE END motor_test_Task */
}

/* USER CODE BEGIN Header_get_and_print_PWM_Task */
/**
 * @brief Function implementing the get_print_PWM thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_get_and_print_PWM_Task */
void get_and_print_PWM_Task(void *argument)
{
  /* USER CODE BEGIN get_and_print_PWM_Task */
  /* Infinite loop */
  for (;;)
  {
    // 读取TIM3的CCR2
    uint32_t pwm1_value = __HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_2);
    // 读取TIM5的CCR2
    uint32_t pwm2_value = __HAL_TIM_GET_COMPARE(&htim5, TIM_CHANNEL_2);
    // 把这两个值放入消息队列中
    osMessageQueuePut(PWM_ValueHandle, &pwm2_value, 0, 3);
    osMessageQueuePut(PWM_ValueHandle, &pwm1_value, 0, 3);
    // 打印PWM值
    static uint16_t Print_count = 0;
    if (Print_count++ >= 100) // 每100次打印一次
    {
      printf("PWM1 Value: %lu, PWM2 Value: %lu\r\n", pwm1_value, pwm2_value);
      Print_count = 0; // 重置计数器
    }
    osDelay(3);
  }
  /* USER CODE END get_and_print_PWM_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
