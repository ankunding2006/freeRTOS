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
__IO CAN_t can = {0};
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
  CAN_FilterTypeDef sFilterConfig;
  sFilterConfig.FilterBank = 1;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 13;
  HAL_CAN_ResetError(&hcan1);
  if (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_CAN_Start(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
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
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
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

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

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
  TxMessage.IDE = CAN_ID_EXT; // 扩展帧
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.DLC = 8;
  // Send message and self-check
  while (HAL_CAN_AddTxMessage(&hcan1, &TxMessage, Data, &TxMailbox) != HAL_OK)
    ;
}

/**
 * @brief   CAN发送多个字节
 * @param   无
 * @retval  无
 */
void can_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
  static uint32_t TxMailbox;
  __IO uint8_t i = 0, j = 0, k = 0, l = 0, packNum = 0;

  // 除去ID地址和功能码后的数据长度
  j = len - 2;

  // 发送数据
  while (i < j)
  {
    // 数据个数
    k = j - i;

    // 填充缓存
    can.CAN_TxMsg.StdId = 0x00;
    can.CAN_TxMsg.ExtId = ((uint32_t)cmd[0] << 8) | (uint32_t)packNum;
    can.txData[0] = cmd[1];
    can.CAN_TxMsg.IDE = CAN_ID_EXT;
    can.CAN_TxMsg.RTR = CAN_RTR_DATA;

    // 小于8字节命令
    if (k < 8)
    {
      for (l = 0; l < k; l++, i++)
      {
        can.txData[l + 1] = cmd[i + 2];
      }
      can.CAN_TxMsg.DLC = k + 1;
    }
    // 大于8字节命令，分包发送，每包数据最多发送8个字节
    else
    {
      for (l = 0; l < 7; l++, i++)
      {
        can.txData[l + 1] = cmd[i + 2];
      }
      can.CAN_TxMsg.DLC = 8;
    }

    // 发送数据
    while (HAL_CAN_AddTxMessage((&hcan1), (CAN_TxHeaderTypeDef *)(&can.CAN_TxMsg), (uint8_t *)(&can.txData), (&TxMailbox)) != HAL_OK)
      ;

    // 记录发送的第几包的数据
    ++packNum;
  }
}

/**
 * @brief   初始化滤波器
 * @param   无
 * @retval  无
 */
void USER_CAN1_Filter_Init(void)
{
  // 过滤器结构体
  CAN_FilterTypeDef sFilterConfig;

  // 设置STM32的帧ID - 扩展帧格式 - 不过滤任何数据帧
  __IO uint8_t id_o, im_o;
  __IO uint16_t id_l, id_h, im_l, im_h;
  id_o = (0x00);
  id_h = (uint16_t)((uint16_t)id_o >> 5);               // 高3位
  id_l = (uint16_t)((uint16_t)id_o << 11) | CAN_ID_EXT; // 低5位
  im_o = (0x00);
  im_h = (uint16_t)((uint16_t)im_o >> 5);
  im_l = (uint16_t)((uint16_t)im_o << 11) | CAN_ID_EXT;

  // 过滤器参数
  sFilterConfig.FilterBank = 0;                      // 过滤器1
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;  // 掩码模式
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT; // 32位过滤器位宽
  sFilterConfig.FilterIdHigh = id_h;                 // 过滤器标识符的高16位值
  sFilterConfig.FilterIdLow = id_l;                  // 过滤器标识符的低16位值
  sFilterConfig.FilterMaskIdHigh = im_h;             // 过滤器屏蔽标识符的高16位值
  sFilterConfig.FilterMaskIdLow = im_l;              // 过滤器屏蔽标识符的低16位值
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0; // 指向过滤器的FIFO为0
  sFilterConfig.FilterActivation = ENABLE;           // 使能过滤器
  sFilterConfig.SlaveStartFilterBank = 0;            // 从过滤器配置，用来选择从过滤器的寄存器编号

  // 配置并自检
  while (HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
    ;
}

/* USER CODE END 1 */
