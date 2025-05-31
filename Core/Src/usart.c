/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */
    GPIO_InitTypeDef gpio_init_struct;
    if (uartHandle->Instance == USART_UX) /* 如果是串口1，进行串口1 MSP初始化 */
    {
      USART_UX_CLK_ENABLE();      /* USART1 时钟使能 */
      USART_TX_GPIO_CLK_ENABLE(); /* 发送引脚时钟使能 */
      USART_RX_GPIO_CLK_ENABLE(); /* 接收引脚时钟使能 */

      gpio_init_struct.Pin = USART_TX_GPIO_PIN;             /* TX引脚 */
      gpio_init_struct.Mode = GPIO_MODE_AF_PP;              /* 复用推挽输出 */
      gpio_init_struct.Pull = GPIO_PULLUP;                  /* 上拉 */
      gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;        /* 高速 */
      gpio_init_struct.Alternate = USART_TX_GPIO_AF;        /* 复用为USART1 */
      HAL_GPIO_Init(USART_TX_GPIO_PORT, &gpio_init_struct); /* 初始化发送引脚 */

      gpio_init_struct.Pin = USART_RX_GPIO_PIN;             /* RX引脚 */
      gpio_init_struct.Alternate = USART_RX_GPIO_AF;        /* 复用为USART1 */
      HAL_GPIO_Init(USART_RX_GPIO_PORT, &gpio_init_struct); /* 初始化接收引脚 */

#if USART_EN_RX
      HAL_NVIC_EnableIRQ(USART_UX_IRQn);         /* 使能USART1中断通道 */
      HAL_NVIC_SetPriority(USART_UX_IRQn, 3, 3); /* 抢占优先级3，子优先级3 */
#endif
    }
  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
