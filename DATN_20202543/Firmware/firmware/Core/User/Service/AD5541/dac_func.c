/*
 * dac_func.c
 *
 *  	Created on: Mar 17, 2024
 *      Author: TruongBVD
 */

#include <dac_types.h>
#include "main.h"

extern SPI_HandleTypeDef hspi1;

uint16_t buffer[64];

static void spiWrite(DAC_DeviceTypes_t *dev,
                     const void *data, size_t dataLength)
{
    HAL_GPIO_WritePin(DAC_CS_GPIO_Port, DAC_CS_Pin, 0);
//    memcpy(buffer, header, headerLength);
//    HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, headerLength, HAL_MAX_DELAY);
    memcpy(buffer, data, dataLength);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, dataLength, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(DAC_CS_GPIO_Port, DAC_CS_Pin, 1);
}

static void spiRead(DAC_DeviceTypes_t *dev, const void *header, size_t headerLength,
                    void *data, size_t dataLength)
{
    HAL_GPIO_WritePin(DAC_CS_GPIO_Port, DAC_CS_Pin, 0);
    memcpy(buffer, header, headerLength);
    HAL_SPI_Transmit(&hspi1, (uint8_t *)buffer, headerLength, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, (uint8_t *)buffer, dataLength, HAL_MAX_DELAY);
    memcpy(data, buffer, dataLength);
    HAL_GPIO_WritePin(DAC_CS_GPIO_Port, DAC_CS_Pin, 1);
}

static void spiSetSpeed(DAC_DeviceTypes_t *dev, DACSpiSpeed_t speed)
{
    if (speed == DACSpiSpeedLow)
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

static void reset(DAC_DeviceTypes_t *dev)
{

}

void DACReset(void)
{

}

static void delayms(DAC_DeviceTypes_t *dev, unsigned int delay)
{
    HAL_Delay(delay);
}

DAC_Function_t DACFunc = {
    .spiRead = spiRead,
    .spiWrite = spiWrite,
    .spiSetSpeed = spiSetSpeed,
    .delayms = delayms,
    .reset = reset,
};
