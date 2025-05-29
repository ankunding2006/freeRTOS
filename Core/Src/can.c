/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    can.c
 * @brief   This file provides code for the configuration
 *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
uint8_t CAN1_Rx_data[8] = {0};
uint8_t CAN2_Rx_data[8] = {0};
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 12;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
  // 过滤器结构体
  CAN_FilterTypeDef sFilterConfig;
  // 过滤器参数
  sFilterConfig.FilterBank = 1;                      // 过滤器序号
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  // 掩码模式
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; // 32位掩码
  sFilterConfig.FilterIdHigh = 0x0000;               //
  sFilterConfig.FilterIdLow = 0x0000;                //
  sFilterConfig.FilterMaskIdHigh = 0x0000;           //
  sFilterConfig.FilterMaskIdLow = 0x0000;            //
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; // 给邮箱0配置的过滤器
  sFilterConfig.FilterActivation = ENABLE;           // 滤波器使能
  sFilterConfig.SlaveStartFilterBank = 13;           // 分配滤波器资源
  // 配置并自检
  while (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
    ;
  HAL_CAN_Start(&hcan1);
  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
  /* USER CODE END CAN1_Init 2 */
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (canHandle->Instance == CAN1)
  {
    /* USER CODE BEGIN CAN1_MspInit 0 */

    /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
    /* USER CODE BEGIN CAN1_MspInit 1 */

    /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef *canHandle)
{

  if (canHandle->Instance == CAN1)
  {
    /* USER CODE BEGIN CAN1_MspDeInit 0 */

    /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11 | GPIO_PIN_12);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    /* USER CODE BEGIN CAN1_MspDeInit 1 */

    /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void CAN1_Transmit(uint32_t ID, uint8_t *Data)
{
  static uint32_t TxMailbox;
  static CAN_TxHeaderTypeDef TxMessage;
  TxMessage.StdId = ID;
  TxMessage.ExtId = ID;
  TxMessage.IDE = CAN_ID_EXT; // 扩展ID
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.DLC = 8;
  // Send message and self-check
  while (HAL_CAN_AddTxMessage(&hcan1, &TxMessage, Data, &TxMailbox) != HAL_OK)
    ;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  static CAN_RxHeaderTypeDef CAN_RxHeader;
  while (HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &CAN_RxHeader, (uint8_t *)CAN1_Rx_data) != HAL_OK)
    ;
  if (CAN_RxHeader.IDE == CAN_ID_STD)
  {
    CAN1_Transmit(CAN_RxHeader.StdId, (uint8_t *)CAN1_Rx_data);
  }
  else if (CAN_RxHeader.IDE == CAN_ID_EXT)
  {
    CAN1_Transmit(CAN_RxHeader.ExtId, (uint8_t *)CAN1_Rx_data);
  }
}
/* USER CODE END 1 */
