/*
 * interrupt.c
 *
 *  Created on: May 30, 2024
 *      Author: DuyTruong
 */
#include "main.h"
#include "common_user.h"

char rxBuffer[512] = {0};
char txBuffer[512]= {0};
systemHandle_t systemHandle = {0};
extern configInfo_obj_t configInfo_t;

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, sizeof(rxBuffer));
    __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART2)
	{
		systemHandle.rxBufAvai = true;
		systemHandle.rxBufSize = Size;
		HAL_UART_Abort_IT(&huart2);
	}
}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//
//	if(htim->Instance == TIM3)
//		{
//
//		}
//}
