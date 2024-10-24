/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "log.h"
#include "stimer.h"
#include "common_user.h"
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
ADC_HandleTypeDef hadc1;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
extern DAC_DeviceTypes_t dev1;
extern ADC_DeviceTypes_t dev2;

extern char txBuffer[512];
extern char rxBuffer[512];

extern systemHandle_t systemHandle;
extern deviceInfo_obj_t deviceInfo_t;
configInfo_obj_t configInfo_t = {0};

measureData_t measureData = {0};
double collect_volt = 0;
uint16_t adc_value = 0;
uint16_t adc_value1 = 0;
volatile uint8_t scan_direction = 1; // 1 for forward, 0 for reverse
volatile uint8_t current_cycle = 0;
double Vref = 5.12;

uint16_t adc_pin_value = 0;
float volt_pin = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
extern void appMeasureInit(void);
extern void Config_dev(configInfo_obj_t *configInfo_t);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef __GNUC__
int __io_putchar(int ch)
#else
int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 1);
  return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  appMeasureInit();

  Logi("START DEVICE");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(systemHandle.devCfg == true){

		  HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
		  HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 1);
		  HAL_ADC_Start(&hadc1);
		  HAL_ADC_PollForConversion(&hadc1, 1000);
		  adc_pin_value = HAL_ADC_GetValue(&hadc1);
		  volt_pin = 4.0/3*(3.3*adc_pin_value)/4095;
		  HAL_ADC_Stop(&hadc1);
		  // Logic cho chế độ quét CV
		  if (deviceInfo_t.mode_dev == 2) {
		  	// Logic cho chế độ quét CV

		  	// Gửi điện áp đến DAC
		  	AD5441A_SetVotage(&dev1, 2.5 - deviceInfo_t.current_volt + 0.2);
		  	measureData.potential = deviceInfo_t.current_volt*1000;
		  	HAL_Delay(125);
		  	adc_value = AD4008_single_conversion(&dev2, adc_value);

		  	// Chuyển đổi giá trị ADC sang giá trị áp dụng(V)
		  	collect_volt = (adc_value*Vref / 65536);

		  	if(deviceInfo_t.res_dev == 1){
		  		measureData.current = (2.5-collect_volt)*1000.0/1500.0;
		  	} else if (deviceInfo_t.res_dev == 2) {
		  		measureData.current = (2.5-collect_volt)*1000000.0/14600.0;
		  	} else if (deviceInfo_t.res_dev == 3) {
		  		measureData.current = (2.5-collect_volt)*1000000000/2000000.0;
		  	}
		  	// �?i�?u chỉnh điện áp cho bước tiếp theo
		  	if (scan_direction == 1) {
		  		deviceInfo_t.current_volt += deviceInfo_t.step_volt;
		  		if (deviceInfo_t.current_volt > deviceInfo_t.target_volt - 0.0001) {
		  			scan_direction = 0; // �?ảo chi�?u quét
		  		}
		  	} else if(scan_direction == 0){
		  			deviceInfo_t.current_volt -= deviceInfo_t.step_volt;
		  			if (deviceInfo_t.current_volt < deviceInfo_t.start_volt + 0.0001) {
		  				scan_direction = 1; // �?ảo chi�?u quét
		  				current_cycle++;
		  					if (current_cycle >= deviceInfo_t.num_cycle) {
								// �?ã hoàn thành tất cả các chu kỳ, dừng Timer
								systemHandle.devCfg = false;
								systemHandle.startDev = false;
								configInfo_t.config_done = false;
								current_cycle =0;
		  					}
		  				}
		  			}
		 }

		  // Logic cho chế độ quét LSV
		  else if (deviceInfo_t.mode_dev == 1) {
			  // Logic cho chế độ quét LSV

			  // Gửi điện áp đến DAC
			  AD5441A_SetVotage(&dev1, 2.5 - deviceInfo_t.current_volt + 0.2);
			  measureData.potential = deviceInfo_t.current_volt*1000;
			  HAL_Delay(125);
			  adc_value = AD4008_single_conversion(&dev2, adc_value);

			  // Chuyển đổi giá trị ADC sang giá trị áp dụng(V)
			  collect_volt = (adc_value*Vref / 65536);

			  if(deviceInfo_t.res_dev == 1){
				  measureData.current = (2.5-collect_volt)*1000.0/1500.0;
			  } else if (deviceInfo_t.res_dev == 2) {
				  measureData.current = (2.5-collect_volt)*1000000.0/14600.0;
			  } else if (deviceInfo_t.res_dev == 3) {
				  measureData.current = (2.5-collect_volt)*1000000000/2000000.0;
			  }
			  // Tăng điện áp
			  deviceInfo_t.current_volt += deviceInfo_t.step_volt;
			  if (deviceInfo_t.current_volt > deviceInfo_t.target_volt*1.0001) {
				  // �?ã đạt điện thế kết thúc, dừng quét
				  systemHandle.devCfg = false;
				  configInfo_t.config_done = false;
				  systemHandle.startDev = false;
				  current_cycle = 0;
			  }

		  }
		  sprintf(txBuffer, "{\"start\":%d,\"potential\":%d,\"current\":%d}", systemHandle.startDev, (int)round(measureData.potential),(int)round(measureData.current*100));
//		  Logi("%s", txBuffer);
		  Logi("VRE-VWE:%.3f - Current:%.5f - VOLT_ADC:%.5f - VOLT_PIN:%d", measureData.potential, measureData.current,collect_volt,adc_pin_value);
		  HAL_UART_Transmit(&huart2, (uint8_t *)txBuffer, strlen(txBuffer), 1000);
		  HAL_Delay(875);
	  }
	  else {
			adc_value1 = AD4008_single_conversion(&dev2, adc_value1);
//			Logi("VRE-VWE:%.3f - Current:%.5f - VOLT_ADC:%.5f", measureData.potential, (2.5 - (adc_value1*Vref / 65536))*1000000.0/14600.0,collect_volt);
		  	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, 1);
		  	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, 0);
		#ifndef TEST
		  	HAL_Delay(150);
			HAL_UART_Transmit(&huart2, (uint8_t *)("$"), 1, 1000);
			HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, sizeof(rxBuffer));
			__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
		  while(!(systemHandle.devCfg == true))
		  {

			  if (systemHandle.rxBufAvai == true)
			  {
				  if (!strcmp(rxBuffer, "GATT_Connected"))
					{
						Logi("%s", rxBuffer);
						memset(rxBuffer, 0 , sizeof(rxBuffer));
					}
				  else if (strstr(rxBuffer , "{") != NULL) {
						configInfo_t.config_done = false;
						Logi("%s", rxBuffer);
						uart_parse_data(rxBuffer, &configInfo_t);
						Config_dev(&configInfo_t);
						memset(rxBuffer, 0 , sizeof(rxBuffer));
					}
				  else if (strstr(rxBuffer, "Start_device") != NULL)
				  {
					  if(configInfo_t.config_done == true){
						  systemHandle.devCfg = true;
						  systemHandle.startDev = true;
					  }

					  Logi("%s", rxBuffer);
					  Logi("Start %s Scan", configInfo_t.mode_device);

				  }
				  memset(rxBuffer, 0 , sizeof(rxBuffer));
				  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, sizeof(rxBuffer));
				  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
				  systemHandle.rxBufAvai = false;
			  }
		  }
		  memset(rxBuffer, 0 , sizeof(rxBuffer));
		  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t *)rxBuffer, sizeof(rxBuffer));
		  __HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
		#endif
	  }
}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
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

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DAC_CS_Pin|ADC_CS_Pin|LDAC_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_R_Pin|LED_G_Pin|SW_A_Pin|SW_B_Pin
                          |SW_C_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DAC_CS_Pin ADC_CS_Pin LDAC_Pin */
  GPIO_InitStruct.Pin = DAC_CS_Pin|ADC_CS_Pin|LDAC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_R_Pin LED_G_Pin SW_A_Pin SW_B_Pin
                           SW_C_Pin */
  GPIO_InitStruct.Pin = LED_R_Pin|LED_G_Pin|SW_A_Pin|SW_B_Pin
                          |SW_C_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
