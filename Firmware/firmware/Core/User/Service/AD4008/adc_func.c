/*
 * adc_func.c
 *
 *  	Created on: Mar 17, 2024
 *      Author: TruongBVD
 */
#include <adc_types.h>
#include "main.h"

extern SPI_HandleTypeDef hspi1;

extern uint16_t buffer[64];

static void spiWrite(ADC_DeviceTypes_t *dev,
                     const void *data, size_t dataLength)
{
    HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, 0);
//    memcpy(buffer, header, headerLength);
//    HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, headerLength, HAL_MAX_DELAY);
    memcpy(buffer, data, dataLength);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, dataLength, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, 1);
}

static void spiRead(ADC_DeviceTypes_t *dev,
                    void *data, size_t dataLength)
{
    HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_RESET);
//    memcpy(buffer, header, headerLength);
//    HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, headerLength, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t *)buffer, dataLength, HAL_MAX_DELAY);
    memcpy(data, buffer, dataLength);
    HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET);
}

static void spiSetSpeed(ADC_DeviceTypes_t *dev, ADCSpiSpeed_t speed)
{
    if (speed == ADCSpiSpeedLow)
    {
    	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
        HAL_SPI_Init(&hspi1);
    }
    else
    {
    	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
        HAL_SPI_Init(&hspi1);
    }
}

static void reset(ADC_DeviceTypes_t *dev)
{

}

void ADCReset(void)
{

}

static void delayms(ADC_DeviceTypes_t *dev, unsigned int delay)
{
    HAL_Delay(delay);
}

ADC_Function_t ADCFunc = {
    .spiRead = spiRead,
    .spiWrite = spiWrite,
    .spiSetSpeed = spiSetSpeed,
    .delayms = delayms,
    .reset = reset,
};
