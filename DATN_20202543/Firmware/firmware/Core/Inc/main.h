/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VBAT_ADC_Pin GPIO_PIN_0
#define VBAT_ADC_GPIO_Port GPIOA
#define DAC_CS_Pin GPIO_PIN_4
#define DAC_CS_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_13
#define LED_R_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_14
#define LED_G_GPIO_Port GPIOB
#define ADC_CS_Pin GPIO_PIN_8
#define ADC_CS_GPIO_Port GPIOA
#define LDAC_Pin GPIO_PIN_11
#define LDAC_GPIO_Port GPIOA
#define SW_A_Pin GPIO_PIN_4
#define SW_A_GPIO_Port GPIOB
#define SW_B_Pin GPIO_PIN_5
#define SW_B_GPIO_Port GPIOB
#define SW_C_Pin GPIO_PIN_6
#define SW_C_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern ADC_HandleTypeDef hadc1;
extern SPI_HandleTypeDef hspi1;
//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart1; //log
extern UART_HandleTypeDef huart2; //esp32
extern DMA_HandleTypeDef hdma_usart2_rx;
extern ADC_HandleTypeDef hadc1;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
